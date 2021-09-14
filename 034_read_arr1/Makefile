CFLAGS = -Wall -Werror -pedantic -std=gnu99
test: test.o
	gcc $(CFLAGS) -o test $^
test.o: test.c
	gcc $(CFLAGS) -c $^
.PHONY: clean
clean:
	rm -f *~ *.o test
