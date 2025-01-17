#include <petsc.h>
#include "../include/utils.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

std::ofstream create_data_file(const std::string dir_path = "../data", const std :: string file_path = "../data/results.csv")
{
    std::ofstream file;
    if (!std::filesystem::exists(dir_path))
    {
        // Create the directory if it does not already exists
        if (std::filesystem::create_directory(dir_path))
        {
            // Create the CSV file inside the newly created directory
            file(file_path);
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
        file(file_path);
    }
    return file;
}
/**
  Callback function to save the results to a file
 */
PetscErrorCode Monitor(TS ts, PetscInt stepid, PetscReal t, Vec U, void *ctx)
{
    const PetscScalar *u;
    std;;ofstream file; 
    create_data_file();
    PetscFunctionBeginUser;
    PetscCall(VecGetArrayRead(U, &u));
    std::cout << "Time step: " << t << "\n";
    std::cout << "__________________" << "\n";
    std::cout << "p1: " << u[0] << "\n";
    std::cout << "p2: " << u[1] << "\n";
    std::cout << "p3: " << u[2] << "\n";
    std::cout << "__________________" << "\n";
    PetscCall(VecRestoreArrayRead(U, &u));
    PetscFunctionReturn(PETSC_SUCCESS);
}

int main(int argc, char *argv[])
{
    TS ts;
    Mat A;
    Vec U;
    const int n{3}; // size
    init_conf init_cond;
    // Default intial conditions
    init_cond.p1 = 0.3;
    init_cond.p2 = 0.4;
    init_cond.p3 = 0.3;
    payoffs rewards;
    // Default enteries for the payoff matrix
    //  First row
    rewards.a = 0.0;
    rewards.b = -1.0;
    rewards.c = 1.0;
    // Second row
    rewards.d = 1.0;
    rewards.e = 0.0;
    rewards.f = -1.0;
    // Third row
    rewards.g = -1.0;
    rewards.h = 1.0;
    rewards.i = 0.0;

    PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));
    // Get the initial distrubution of the species 1, 2, and 3
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p1", &init_cond.p1, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p2", &init_cond.p2, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p3", &init_cond.p3, NULL));

    // Get the payoff matrix enteries from the user
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a00", &rewards.a, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a01", &rewards.b, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a02", &rewards.c, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a10", &rewards.d, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a11", &rewards.e, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a12", &rewards.f, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a20", &rewards.g, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a21", &rewards.h, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a22", &rewards.i, NULL));
    // Create vector and intilize it with intial conditions
    PetscCall(VecCreateSeq(PETSC_COMM_SELF, n, &U));
    PetscCall(VecSetValue(U, 0, init_cond.p1, INSERT_VALUES));
    PetscCall(VecSetValue(U, 1, init_cond.p2, INSERT_VALUES));
    PetscCall(VecSetValue(U, 2, init_cond.p3, INSERT_VALUES));
    // Create the matrix
    PetscCall(MatCreateSeqDense(PETSC_COMM_SELF, n, n, NULL, &A));
    PetscCall(TSCreate(PETSC_COMM_SELF, &ts));
    PetscCall(TSSetRHSFunction(ts, NULL, Probabilities, &rewards));
    PetscCall(TSSetRHSJacobian(ts, A, A, PayOffMatrix, &rewards));
    PetscCall(MatDestroy(&A));

    PetscCall(TSMonitorSet(ts, Monitor, NULL, NULL));

    PetscCall(TSSetType(ts, TSRK));
    PetscCall(TSRKSetType(ts, TSRK4));
    PetscCall(TSSetMaxTime(ts, 100));
    PetscCall(TSSetExactFinalTime(ts, TS_EXACTFINALTIME_STEPOVER));

    PetscCall(TSSetFromOptions(ts));

    PetscCall(TSSolve(ts, U));

    PetscCall(TSDestroy(&ts));
    PetscCall(VecDestroy(&U));
    PetscCall(PetscFinalize());
    PetscCall(PetscFinalize());
    return 0;
}