NAME="Life.exe"
DEPS= mainMenu.o game.o
CC=g++
CFLAGS=-Wall
LDLIBS=-lallegro-5.0.10-monolith-md

build: $(DEPS)
	$(CC) $(CFLAGS) -o $(NAME) $^ $(LDFLAGS)
	
clean:
	rm -f $(DEPS) $(NAME)
