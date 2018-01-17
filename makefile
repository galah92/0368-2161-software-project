CC = gcc
OBJS = ChessGame.o main.o
EXEC = chessprog
CC_COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
UserCommand.o: UserCommand.c UserCommand.h
	$(CC) $(COMP_FLAG) -c $*.c
ChessGame.o: ChessGame.c ChessGame.h
	$(CC) $(COMP_FLAG) -c $*.c
main.o: main.c ChessGame.o UserCommand.o
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
