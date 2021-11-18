CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,%.o,$(PROGS)) page.o story.o
all: $(PROGS)
cyoa-step1: cyoa-step1.o page.o
	g++ -o $@ $^
cyoa-step2: cyoa-step2.o page.o story.o
	g++ -o $@ $^
cyoa-step3: cyoa-step3.o page.o story.o
	g++ -o $@ $^
%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)
page.o: page.hpp
story.o: story.hpp
