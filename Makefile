OBJDIR = build
OUTNAME = Minecraft-2D
OUT = ${OBJDIR}/${OUTNAME}
RELEASE_FOLDER = ${OBJDIR}/release

.PHONY: all run release

all: ${OUT}

CC = gcc
SRC = ${wildcard src/*.c}
DEP = ${patsubst src/%.c, ${OBJDIR}/%.d, ${SRC}}
-include ${DEP}
OBJ = ${patsubst src/%.c, ${OBJDIR}/%.o, ${SRC}}
CFLAGS = -Wall -Wextra -std=c11 -Wpedantic
CFLAGS += -Wswitch -Wswitch-enum -I./include
CFLAGS_DEBUG = -ggdb
CFLAGS_RELEASE = -O3 -DNDEBUG

ifeq (${OS}, Windows_NT)
	CFLAGS += -Dmain=SDL_main
	CFLAGS += -I./include/SDL2
	CFLAGS_RELEASE += -mwindows
	LIB = -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	MKDIR = powershell mkdir
	CP = powershell cp
else
	CFLAGS += -I/usr/include/SDL2
	LIB = -L./lib -lSDL2 -lSDL2_image -lSDL2_ttf -lm
	MKDIR = mkdir
	CP = cp
endif



${OUT}: ${OBJ} | ${OBJDIR}
	${CC} ${OBJ} -o ${OUT} ${LIB} ${CFLAGS} ${CFLAGS_DEBUG}

${OBJDIR}:
	mkdir ${OBJDIR}

${OBJDIR}/%.o: src/%.c Makefile
	${CC} -c $< -o $@ ${CFLAGS} ${CFLAGS_DEBUG} -MMD -MP

run: ${OUT}
	./${OUT}

clean:
ifeq (${OS}, Windows_NT)
	powershell remove-item ${OBJDIR}/* -include *.o, *.exe, *.d | powershell out-null
	powershell remove-item -r ${RELEASE_FOLDER} | powershell out-null
else
	rm -rf ${OBJDIR}/*.o ${OBJDIR}/*.d ${OUT} ${RELEASE_FOLDER}
endif

release: | ${RELEASE_FOLDER}
	${CC} ${SRC} -o ${RELEASE_FOLDER}/${OUTNAME} ${CFLAGS} ${CFLAGS_RELEASE} ${LIB}
	${CP} -r assets ${RELEASE_FOLDER}
ifeq (${OS}, Windows_NT)
	${CP} ${OBJDIR}/*.dll ${RELEASE_FOLDER}
endif

${RELEASE_FOLDER}: | ${OBJDIR}
	${MKDIR} -p ${RELEASE_FOLDER}
