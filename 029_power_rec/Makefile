CFLAGS = -Wall -Werror -std=gnu99 -pedantic
test-power: test-power.o power.o
	gcc $(CFLAGS) -o test-power $^
%.o: %.c
	gcc -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f *.o *~ *.txt test-power
