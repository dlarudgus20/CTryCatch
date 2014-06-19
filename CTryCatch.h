#ifndef CTryCatch_h_
#define CTryCatch_h_

#include <setjmp.h>

void _ctc_pushJmp_(jmp_buf *buf);
jmp_buf *THROW(int e);

#define TRY											\
	{												\
		jmp_buf _bufExcep_;							\
		int _excep_;								\
		_ctc_pushJmp_(&_bufExcep_);					\
		_excep_ = setjmp(_bufExcep_);				\
		if (_excep_ == 0)
#define CATCH(e)									\
		else if (_excep_ == e)
#define END_CATCH									\
		else THROW(_excep_);						\
	}

#endif // CTryCatch_h_