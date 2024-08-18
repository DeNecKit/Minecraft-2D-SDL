CC = gcc
SRC = ${wildcard src/*.c}
OBJ = ${patsubst src/%.c, build/%.o, ${SRC}}
CFLAGS = -Dmain=SDL_main -Wall -Wextra -std=c11 -Wpedantic
CFLAGS += -I./include -I./include/SDL2
CFLAGS_RELEASE = -O3 -mwindows
LIB = -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
OUT = build/Minecraft-2D.exe
RELEASE_NAME = Minecraft-2D-${shell git describe --tags}

.PHONY: all build run

all: run

${OUT}: ${SRC} ${wildcard src/*.h}
	powershell new-item -itemtype directory -force -path build | powershell out-null
	make build

build: ${OBJ}
	${CC} ${OBJ} -o ${OUT} ${LIB}

build/%.o: src/%.c
	${CC} -c $< -o $@ ${CFLAGS}

run: ${OUT}
	./${OUT}

clean:
	powershell remove-item build/* -include *.o, *.exe

build-release:
	powershell new-item -itemtype directory -force -path build/releases/${RELEASE_NAME} | powershell out-null
	${CC} ${SRC} -o build/releases/${RELEASE_NAME}/Minecraft-2D.exe ${CFLAGS} ${CFLAGS_RELEASE} ${LIB}
	powershell copy assets -d build/releases/${RELEASE_NAME} -r | powershell out-null
	powershell copy build/*.dll -d build/releases/${RELEASE_NAME} | powershell out-null
