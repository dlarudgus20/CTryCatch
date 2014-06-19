#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "CTryCatch.h"

static struct
{
	jmp_buf **arBuf;
	size_t count, size;
} ExceptionJmpStack = { NULL, 0, 1 };

static void FreeJmpStack(void)
{
	if (ExceptionJmpStack.arBuf != NULL) free(ExceptionJmpStack.arBuf);
}

void _ctc_pushJmp_(jmp_buf *buf)
{
	if (ExceptionJmpStack.arBuf == NULL)
	{
		ExceptionJmpStack.arBuf = (jmp_buf **)malloc(sizeof(jmp_buf *));
		atexit(FreeJmpStack);
	}
	else if (ExceptionJmpStack.count == ExceptionJmpStack.size)
	{
		ExceptionJmpStack.arBuf = (jmp_buf **)realloc(
			ExceptionJmpStack.arBuf, sizeof(jmp_buf *) * (ExceptionJmpStack.size + 1));
		ExceptionJmpStack.size++;
	}

	ExceptionJmpStack.arBuf[ExceptionJmpStack.count] = buf;
	ExceptionJmpStack.count++;
}

jmp_buf *THROW(int e)
{
	if (ExceptionJmpStack.count != 0)
	{
		ExceptionJmpStack.count--;
		longjmp(*ExceptionJmpStack.arBuf[ExceptionJmpStack.count], e);
	}
	else
	{
		fprintf(stderr, "CTryCatch::<<PANIC>> uncaught exception '%d'\n", e);
		abort();
	}
}
