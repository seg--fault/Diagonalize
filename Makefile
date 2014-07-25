INC= -I$(PETSC_DIR)/include -I$(SLEPC_DIR)/include
LIBS = -L$(PETSC_DIR)/lib -lpetsc -L$(SLEPC_DIR)/lib -lslepc -Wl,-rpath=$(PETSC_DIR)/lib,-rpath=$(SLEPC_DIR)/lib

all: main.o read_matrix.o
	mpic++ -o diagonalize main.o read_matrix.o $(LIBS)

main.o: main.cpp read_matrix.h
	mpic++ -c main.cpp $(INC)

read_matrix.o: read_matrix.cpp read_matrix.h
	mpic++ -c read_matrix.cpp $(INC)
