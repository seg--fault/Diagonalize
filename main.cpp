#include <iostream>
#include <slepceps.h>

#include "read_matrix.h"

#undef __FUNCT__
#define __FUNCT__ "main"

int main(int argc, char** argv)
{
	//Initialize SLEPc
	SlepcInitialize(&argc, &argv, (char*)0, "HELP");

	//Our two operators
	Mat A, B;

	//Hold errors
	PetscErrorCode ierr;

	//Read in matrix A
	std::cout << "Reading in Matrix A\n";
	ierr = read_matrix(argv[1], A);  CHKERRQ(ierr);

	//Read in matrix B
	std::cout << "Reading in Matrix B\n";
	ierr = read_matrix(argv[2], B);  CHKERRQ(ierr);

	//Our eigensolver
	EPS solver;

	//Initialize it
	ierr = EPSCreate(PETSC_COMM_WORLD, &solver);

	//Set our eigenvalue operators
	ierr = EPSSetOperators(solver, A, NULL);  CHKERRQ(ierr);

	//Set type and which eigenvalues to get
	ierr = EPSSetProblemType(solver, EPS_NHEP);  CHKERRQ(ierr);
	ierr = EPSSetWhichEigenpairs(solver, EPS_SMALLEST_MAGNITUDE);  CHKERRQ(ierr);

	//Set from command line options
	ierr = EPSSetFromOptions(solver);  CHKERRQ(ierr);

	//Solve it
	std::cout << "Solving System\n";
	ierr = EPSSolve(solver);  CHKERRQ(ierr);

	//Store the various results
	EPSType type;
	PetscInt its, maxit, nconv, nev;
	PetscReal error, tol;

	//Print them
	ierr = EPSGetIterationNumber(solver, &its);  CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD, "Number of iterations of the method: %D\n", its);  CHKERRQ(ierr);
	ierr = EPSGetType(solver, &type);  CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD, "Solution method: %s\n\n", type);  CHKERRQ(ierr);
	ierr = EPSGetDimensions(solver, &nev, NULL, NULL);  CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD, "Number of requested eigenvalues: %D\n", nev);  CHKERRQ(ierr);
	ierr = EPSGetTolerances(solver, &tol, &maxit);  CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD, "Stopping condition: tol=%.4G, maxit=%D\n", tol, maxit);  CHKERRQ(ierr);
 	ierr = EPSGetConverged(solver, &nconv);  CHKERRQ(ierr);
 	ierr = PetscPrintf(PETSC_COMM_WORLD, " Number of converged eigenpairs: %D\n\n", nconv);  CHKERRQ(ierr);



	
	return 0;
}
