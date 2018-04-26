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

.PHONY: build clean

default : all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDLLIB) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(SDLINC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
