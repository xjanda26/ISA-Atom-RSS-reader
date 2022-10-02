#Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
PRJ=feedreader
TEST=test

CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -g

PROGS=$(PRJ) $(TEST)
COMPILE=$(CC) $(CFLAGS)

all: $(PROGS)

feedreader_obejcts = parameters.o
test_objects = tests.o unit_testing.o parameter_tests.o $(feedreader_obejcts)

#===- Application source files linking -===
$(PRJ): main.o $(feedreader_obejcts)
	$(COMPILE) -o $(PRJ) main.o $(feedreader_obejcts)

main.o: src/main.c
	$(COMPILE) -c src/main.c

parameters.o: src/parameters.h src/parameters.c
	$(COMPILE) -c src/parameters.c

#===- Test source files linking -===
$(TEST): $(test_objects)
	$(COMPILE) -o $(TEST) $(test_objects)

tests.o: tests/tests.c tests/parameter_tests.h
	$(COMPILE) -c tests/tests.c

unit_testing.o: tests/unit_testing.c tests/unit_testing.h
	$(COMPILE) -c tests/unit_testing.c

parameter_tests.o: tests/parameter_tests.c tests/parameter_tests.h src/parameters.h
	$(COMPILE) -c tests/parameter_tests.c

clean: 
	rm -f *.o $(PROGS)
