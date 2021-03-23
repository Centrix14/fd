CC = gcc
CFLAGS = -Wall -O0 -g -o
LIB = so.c -ldl

tt1:
	$(CC) $(CFLAGS) t1 t1.c $(LIB)

test: tt1

run:
	./t1

clear:
	rm t1

share:
	$(CC) -c -Wall -pie test_so.c
	$(CC) -shared -o test.so test_so.o
