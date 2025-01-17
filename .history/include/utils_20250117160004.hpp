#ifndef UTILS_H
#define UTILS_H
#include<petsc.h>
#include <filesystem>
#include <fstream>
#include <string>
#include<iostream>
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
/*
    Struct to store the payoffs
*/
typedef struct PAYOFFS
{
    PetscReal a; /*A_00*/
    PetscReal b; /*A_01*/
    PetscReal c; /*A_02*/
    PetscReal d; /*A_10*/
    PetscReal e; /*A_11*/
    PetscReal f; /*A_12*/
    PetscReal g; /*A_20*/
    PetscReal h; /*A_21*/
    PetscReal i; /*A_22*/
} payoffs;
/**
 * Initial configuration
 */
typedef struct INIT_CONFG
{
    PetscReal p1;
    PetscReal p2;
    PetscReal p3;
} init_conf;


#endif // UTILS_H