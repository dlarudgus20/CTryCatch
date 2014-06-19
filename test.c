#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CTryCatch.h"

void foo()
{
	// throw 2 or 3
	int i = rand() % 2 + 2;
	printf("foo(): throw %d!\n", i);
	THROW(i);
}

int main()
{
	srand((unsigned)time(NULL));

	TRY
	{
		TRY
		{
			foo();
		}
		CATCH (2)
		{
			puts("catch 2");
			THROW(1);
		}
		CATCH (3)
		{
			puts("catch 3");
			THROW(99);
		}
		END_CATCH
	}
	CATCH (1)
	{
		puts("catch 1");
	}
	CATCH (2)
	{
		puts("catch 2");
	}
	CATCH (99)
	{
		puts("catch 99");
	}
	END_CATCH

	getchar();
}
