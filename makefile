# yes, it need to be edited..

test: CTryCatch.h CTryCatch.c test.c
	$(CC) $(CFLAGS) CTryCatch.c test.c -o $@
