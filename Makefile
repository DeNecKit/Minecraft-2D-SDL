CC = gcc
SRCS = src/main.c
CFLAGS = -Dmain=SDL_main -Wall -Wextra -std=c11 -Wpedantic
CFLAGS += -I./include -I./include/SDL2
LIB = -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
OUT = build/Minecraft-2D.exe

.PHONY: all build run

all: run

${OUT}: ${SRCS}
	make build

build:
	${CC} ${SRCS} -o ${OUT} ${CFLAGS} ${LIB}

run: ${OUT}
	./${OUT}
