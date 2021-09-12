CFLAGS = -Wall -Werror -std=gnu99 -pedantic
test: test.o
	gcc $(CFLAGS) -o test $^
test.o: test.c
	gcc $(CFLAGS) -c $^
.PHONY: clean
clean:
	rm -f *~ *.o test
