CC = gcc
CFLAGS = -Wall

SRCPATH = ./src/
C = $(SRCPATH)main.c

EXE = ./bin/Polygon.exe
MKDIR = mkdir -p ./bin
BUILD = $(MKDIR) & $(CC) $(CFLAGS) $(C) -o $(EXE) -lSDL2

build:
	$(BUILD)

run:
	./$(EXE)
