EXEC			:= chessprog
CC				:= gcc
CFLAGS			:= -std=c99 -Wall -Wextra -Werror -pedantic-errors -ggdb
SDLINC_NOVA		:= -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDLLIB_NOVA		:= -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main
SDLINC_DARWIN	:= -I/usr/local/SDL/include -D_REENTRANT
SDLLIB_DARWIN	:= -L/usr/local/SDL/lib -Wl,-rpath,/usr/local/SDL/lib -Wl,-install_name,--enable-new-dtags -lSDL2 -lSDL2main
SRCDIR			:= src
OBJDIR			:= obj
BINDIR			:= bin

# detecting all src files
SOURCES	= $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET	= $(BINDIR)/$(EXEC)

# detecting OS
OSTYPE := $(shell uname -s)
OSNAME := $(shell uname -n)
ifeq ($(OSNAME), nova)
	SDLINC = $(SDLINC_NOVA)
	SDLLIB = $(SDLLIB_NOVA)
else ifeq ($(OSTYPE), Darwin)
	SDLINC = $(SDLINC_DARWIN)
	SDLLIB = $(SDLLIB_DARWIN)
	CFLAGS += -Wno-builtin-memcpy-chk-size
else
	# assuming nova for now
	SDLINC = $(SDLINC_NOVA)
	SDLLIB = $(SDLLIB_NOVA)
endif

.PHONY: build clean test

default : all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDLLIB) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(SDLINC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

test:
	@./$(BINDIR)/$(EXEC) < tst/1.input > tst/1.output
	@diff -q tst/1.correct tst/1.output > /dev/null || echo "1 failed"
	@./$(BINDIR)/$(EXEC) < tst/2.input > tst/2.output
	@diff -q tst/2.correct tst/2.output > /dev/null || echo "2 failed"
	@./$(BINDIR)/$(EXEC) < tst/3.input > tst/3.output
	@diff -q tst/3.correct tst/3.output > /dev/null || echo "3 failed"
	@./$(BINDIR)/$(EXEC) < tst/4.input > tst/4.output
	@diff -q tst/4.correct tst/4.output > /dev/null || echo "4 failed"
