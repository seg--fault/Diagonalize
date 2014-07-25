INC= -I$(PETSC_DIR)/include -I$(SLEPC_DIR)/include
LIBS = -L$(PETSC_DIR)/lib -lpetsc -L$(SLEPC_DIR)/lib -lslepc -Wl,-rpath=$(PETSC_DIR)/lib,-rpath=$(SLEPC_DIR)/lib

all: read_matrix.o
	mpic++ -o diagonalize read_matrix.o $(LIBS)

read_matrix.o: read_matrix.cpp read_matrix.h
	mpic++ -c read_matrix.cpp $(INC)
