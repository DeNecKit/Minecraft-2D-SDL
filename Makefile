CC = gcc
SRC = ${wildcard src/*.c}
OBJ = ${patsubst src/%.c, build/%.o, ${SRC}}
CFLAGS = -Dmain=SDL_main -Wall -Wextra -std=c11 -Wpedantic
CFLAGS += -I./include -I./include/SDL2
LIB = -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
OUT = build/Minecraft-2D.exe

.PHONY: all build run

all: run

${OUT}: ${SRC} ${wildcard src/*.h}
	make build

build: ${OBJ}
	${CC} ${OBJ} -o ${OUT} ${LIB}

build/%.o: src/%.c
	${CC} -c $< -o $@ ${CFLAGS}

run: ${OUT}
	./${OUT}
