#Compiler vers
CC=gcc


CFLAGS = -Wall -Wextra -Werror -std=c99 
LDFLAGS = 
LDLIBS = `pkg-config --cflags --libs SDL2_image`
OBJ = solver_sudoku.o solver.o

all: solver
DEP = ${OBJ: .o = .d}

solver: ${OBJ}

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
