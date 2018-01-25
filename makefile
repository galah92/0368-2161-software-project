EXEC 	= chessprog
SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin
CC 		= gcc
CFLAGS 	= -std=c99 -Wall -Wextra -Werror -pedantic-errors -ggdb
SDLINC	= -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDLLIB	= -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

SOURCES	= $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET	= $(BINDIR)/$(EXEC)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDLLIB) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(SDLINC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
