#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

#include "read_matrix.h"

int read_matrix(std::string filename, Mat matrix_handle)
{
	//File Handle
	std::ifstream infile;

	//Open up our file
	infile.open(filename.c_str(), std::ifstream::in);

	//Check to see if it is open
	if(!infile.is_open())
	{
		std::cout << "ERROR: Could not open file " << filename << std::endl;
		return 65; //PETSc error code 65 is "couldn't open file"
	}

	//The number of matrix elements we have
	unsigned int n_elements = 0;

	//The current line
	char current_line[256];
	
	//Initialize three arrays, our indices and elements
	long long int 	max_M;
	long long int 	max_N;

	//Figure out the size of the matrix to initialize
	while(infile.good())
	{
		//Fetch a line
		infile.getline(current_line, 256);

		//If it isn't a comment
		if(current_line[0] != '#')
		{
			//A token
			char* token;

			//Temporary storage for comparison
			long long int temp;

			//Get our first token and convert it
			token = strtok(current_line, " \n");
			temp = atoll(token);

			//Compare with our max
			if(temp > max_M)
				max_M = temp;
			
			//Get another without a pointer
			token = strtok(NULL, " \n");
			temp = atoll(token);

			//Compare
			if(temp > max_N)
				max_N = temp;
		}
	}

	//Hold PETSc/SLEPc errors
	PetscErrorCode ierr;

	//Initialize our SLEPc matrix
	ierr = MatCreate(PETSC_COMM_WORLD, &matrix_handle);  CHKERRQ(ierr);
	ierr = MatSetSizes(matrix_handle, PETSC_DECIDE, PETSC_DECIDE, max_M, max_N);  CHKERRQ(ierr);
	ierr = MatSetFromOptions(matrix_handle);  CHKERRQ(ierr);
	ierr = MatSetUp(matrix_handle);  CHKERRQ(ierr);

	//Go back to the beginning of our file
	infile.seekg(std::ios::beg);

	//Read through again
	while(infile.good())
	{
		//Fetch a line
		infile.getline(current_line, 256);

		//If it isn't a comment
		if(current_line[0] != '#')
		{
			//A token
			char* token;
			PetscInt M[1];
			PetscInt N[1];
			PetscScalar element[1];

			//Get our first token and convert it
			token = strtok(current_line, " \n");
			M[0] = atoll(token);

			//Get another without a pointer
			token = strtok(NULL, " \n");
			N[0] = atoll(token);

			//Get our element
			token = strtok(NULL, " \n");
			element[0] = atof(token);

			//Insert it into the matrix
			ierr = MatSetValues(matrix_handle, 1, M, 1, N, element, INSERT_VALUES);  CHKERRQ(ierr);
		}
	}

	//Finish assembling our matrix
	ierr = MatAssemblyBegin(matrix_handle, MAT_FINAL_ASSEMBLY);  CHKERRQ(ierr);
	ierr = MatAssemblyEnd(matrix_handle, MAT_FINAL_ASSEMBLY);  CHKERRQ(ierr);
	
	//Close our file
	infile.close();

	//Nothing went wrong
	return 0;
}
