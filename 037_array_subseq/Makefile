CFLAGS = -Wall -Werror -pedantic -std=gnu99
test-subseq: maxSeq.o test-subseq.o
	gcc $(CFLAGS) -o test-subseq $^
%.o: %.c
	gcc $(CFLAGS) -c $<
.PHONY: clean
clean:
	rm -f *~ *.o test-subseq
