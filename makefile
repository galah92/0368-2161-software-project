EXEC 		= chessprog
SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin
CC			= gcc
CFLAGS		= -std=c99 -Wall -Wextra -Werror -pedantic-errors -ggdb
SDLINC_NOVA = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDLLIB_NOVA = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main
SDLINC_MAC	= -I/usr/local/SDL/include -D_REENTRANT
SDLLIB_MAC	= -L/usr/local/SDL/lib -Wl,-rpath,/usr/local/SDL/lib -Wl,-install_name,--enable-new-dtags -lSDL2 -lSDL2main

SOURCES	= $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET	= $(BINDIR)/$(EXEC)

.PHONY: all nova mac clean

default all: nova

nova:	SDLINC = $(SDLINC_NOVA)
nova:	SDLLIB = $(SDLLIB_NOVA)
mac:	SDLINC = $(SDLINC_MAC)
mac:	SDLLIB = $(SDLLIB_MAC)

all nova mac: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDLLIB) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(SDLINC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
