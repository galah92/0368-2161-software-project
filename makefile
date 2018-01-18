TARGET = chessprog
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic-errors
SOURCES   = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

.PHONY: default all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS)



# TODO: need to find a way to include SDL in compilation as well
# SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
# SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

# $(TARGET): $(OBJS)
# 	$(CC) $(OBJS) $(SDL_LIB) -o $@
# main.o: main.c ChessGame.o UserCommand.o
# 	$(CC) $(CFLAGS) $(SDL_COMP_FLAG) -c $*.c
