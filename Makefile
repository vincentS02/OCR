# Makefile
CC=gcc


CFLAGS = `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -Wall 
LDLIBS = `pkg-config --libs sdl2 SDL2_image gtk+-3.0 gmodule-2.0` -lm

OBJ =  image_processing/pixel_operations.o image_processing/grid_detection.o image_processing/manual_rotation.o image_processing/greyscale.o image_processing/gras.o image_processing/contrast.o image_processing/binarisation_3.o image_processing/rotation_auto.o  image_processing/result_solver.o solver/solver_sudoku.o image_processing/filetostr.o main.o neural_network/xor.o neural_network/matrix.o test/tofile.o image_processing/imgtolist.o


DEP =${OBJ: .o = .d}
EXE = main

all: $(EXE)

main: ${OBJ}

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)
	${RM} *.o
	${RM} *.d
	${RM} *.png
	${RM} image_processing/*.o
	${RM} Result/*.png

# END