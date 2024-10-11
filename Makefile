BIN=./bin

build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o $(BIN)/renderer

run:
	./$(BIN)/renderer

clean:
	rm $(BIN)/*
