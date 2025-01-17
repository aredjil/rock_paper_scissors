#ifndef MONITOR_H
#define MONITOR_H
#include<petsc.h>
#include <fstream>
#include <string>
#include<iostream>

/*
    Create a results file and data directory where the results will be stored
*/
std::ofstream create_data_file(const std::string &dir_path = "../data", const std :: string &file_path = "../data/results.csv")
{
    std::ofstream file;
    if (!std::filesystem::exists(dir_path))
    {
        // Create the directory if it does not already exists
        if (std::filesystem::create_directory(dir_path))
        {
            // Create the CSV file inside the newly created directory
            file.open(file_path, std::ios::app);

        }
        else
        {
            // Return an error if failed to create a directory
            std::cerr << "Failed to create directory: " << dir_path << '\n';
        }
    }
    else
    {
        // If the directory already exists create the CSV file
        file.open(file_path, std::ios::app);

    }
    return file;
}
/**
  Callback function to save the results to a file
 */
PetscErrorCode Monitor(TS ts, PetscInt stepid, PetscReal t, Vec U, void *ctx)
{
    const PetscScalar *u;
    std::ofstream file; 
    file = create_data_file();

    PetscFunctionBeginUser;
    PetscCall(VecGetArrayRead(U, &u));
    file<<t<<","<<u[0]<<","<<u[1]<<","<<u[2]<<"\n";
    PetscCall(VecRestoreArrayRead(U, &u));
    // Close the file 
    file.close();
    PetscFunctionReturn(PETSC_SUCCESS);
}
#endif // JACOBIAN_H

