/*
Copyright (c) 2014, 임경현 (dlarudgus20)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
