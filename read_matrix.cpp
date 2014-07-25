#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

#include "read_matrix.h"

int read_matrix(std::string filename, Mat& matrix_handle)
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

	//The current line
	char current_line[256];
	
	//Hold the greatest indices
	long long int 	max_M = 0;
	long long int 	max_N = 0;

	//Fetch the first line
	infile.getline(current_line, 256);

	//Keep going until we run out of file
	while(infile.good())
	{
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
		//Fetch another line
		infile.getline(current_line, 256);
	}

	//Hold PETSc/SLEPc errors
	PetscErrorCode ierr;

	//Debugging...
	//std::cout << "Max_M: " << max_M << "	Max_N: " << max_N << std::endl;

	//Initialize our SLEPc matrix
	ierr = MatCreate(PETSC_COMM_WORLD, &matrix_handle);  CHKERRQ(ierr);
	ierr = MatSetSizes(matrix_handle, PETSC_DECIDE, PETSC_DECIDE, max_M+1, max_N+1);  CHKERRQ(ierr);
	ierr = MatSetFromOptions(matrix_handle);  CHKERRQ(ierr);
	ierr = MatSetUp(matrix_handle);  CHKERRQ(ierr);

	//Hack to reset our file
	infile.close();
	infile.open(filename.c_str(), std::ifstream::in);

	//Fetch the first line
	infile.getline(current_line, 256);

	//Read through again
	while(infile.good())
	{

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

			//Debugging...
			//std::cout << "ELEMENT[" << M[0] << "," << N[0] << "]=" << element[0] << std::endl;

			//Insert it into the matrix
			ierr = MatSetValues(matrix_handle, 1, M, 1, N, element, INSERT_VALUES);  CHKERRQ(ierr);
		}

		//Fetch another line
		infile.getline(current_line, 256);
	}

	//Finish assembling our matrix
	ierr = MatAssemblyBegin(matrix_handle, MAT_FINAL_ASSEMBLY);  CHKERRQ(ierr);
	ierr = MatAssemblyEnd(matrix_handle, MAT_FINAL_ASSEMBLY);  CHKERRQ(ierr);

	//Close our file
	infile.close();

	//Nothing went wrong
	return 0;
}
