#Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
PRJ := feedreader
TEST := test

CC := gcc
CFLAGS := -Wall -std=gnu17 -pedantic -lm -g

PROGS := $(PRJ) $(TEST)
COMPILE := $(CC) $(CFLAGS)

OBJDIR := bin

FEEDREADER_OBJS := $(addprefix $(OBJDIR)/, \
						parameters.o feed.o dns_communication.o \
						error.o host.o http_communication.o)
TEST_OBJS := $(addprefix $(OBJDIR)/, tests.o unit_testing.o parameter_tests.o) $(FEEDREADER_OBJS)

#===- Application source files -===
#==-- Linking --==
OBJS := $(OBJDIR)/main.o $(FEEDREADER_OBJS)

$(PRJ): $(OBJS)
	$(COMPILE) $^ -o $@

#==-- Compilation --==
$(OBJDIR)/%.o: src/%.c
	$(COMPILE) -c $< -o $@

#===- Test source files linking -===
#==-- Linking --==
$(TEST): $(TEST_OBJS)
	$(COMPILE) $^ -o $@

#==-- Compilation --==
$(OBJDIR)/%.o: tests/%.c
	$(COMPILE) -c $< -o $@

.PHONY: $(TEST)

clean:
	rm -f $(OBJDIR)/*.o $(PROGS)

all: $(PROGS)
