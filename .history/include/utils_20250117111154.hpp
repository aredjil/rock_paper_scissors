#ifndef UTILS_H
#define UTILS_H
#include<petsc.h>
#include <filesystem>
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
            file<<"t, p1, p2, p3"<<"\n"; 

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
        file<<"t, p1, p2, p3"<<"\n"; // set the indexing 

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
/*
    Probabilities
*/
PetscErrorCode Probabilities(TS ts, PetscReal t, Vec U, Vec F, void *ctx)
{
    const PetscScalar *u;
    PetscScalar *f;
    payoffs *outcome = (payoffs *)ctx;

    PetscFunctionBeginUser;
    PetscCall(VecGetArrayRead(U, &u));

    PetscCall(VecGetArrayWrite(F, &f));

    f[0] = (outcome->a * u[0] + outcome->b * u[1] + outcome->c * u[2]) * u[0];
    f[1] = (outcome->d * u[0] + outcome->e * u[1] + outcome->f * u[2]) * u[1];
    f[2] = (outcome->g * u[0] + outcome->h * u[1] + outcome->i * u[2]) * u[2];

    PetscCall(VecRestoreArrayRead(U, &u));
    PetscCall(VecRestoreArrayWrite(F, &f));
    PetscFunctionReturn(PETSC_SUCCESS);
};
/*
    Payoff matrix of the game
*/
PetscErrorCode PayOffMatrix(TS ts, PetscReal t, Vec U, Mat A, Mat P, void *ctx)
{
    const PetscScalar *u;
    PetscScalar v[3][3];
    PetscInt i[] = {0, 1, 2};
    payoffs *outcome = (payoffs *)ctx;

    PetscFunctionBeginUser;

    PetscCall(VecGetArrayRead(U, &u));
    v[0][0] = outcome->a;
    v[0][1] = outcome->b;
    v[0][2] = outcome->c;
    v[1][0] = outcome->d;
    v[1][1] = outcome->e;
    v[1][2] = outcome->f;
    v[2][0] = outcome->g;
    v[2][1] = outcome->h;
    v[2][2] = outcome->i;
    PetscCall(VecRestoreArrayRead(U, &u));

    PetscCall(MatSetValues(P, 3, i, 3, i, (PetscScalar *)v, INSERT_VALUES));
    PetscCall(MatAssemblyBegin(P, MAT_FINAL_ASSEMBLY));
    PetscCall(MatAssemblyEnd(P, MAT_FINAL_ASSEMBLY));

    if (A != P)
    { 
        PetscCall(MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY));
        PetscCall(MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY));
    }
    PetscFunctionReturn(PETSC_SUCCESS);
};
#endif // UTILS_H