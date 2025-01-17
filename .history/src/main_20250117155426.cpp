#include <petsc.h>
#include "../include/utils.hpp"
#include <iostream>

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

int main(int argc, char *argv[])
{
    TS ts; // Time steping object 
    Mat A; // Matrix object 
    Vec U; // State vector 
    const int n{3}; // size
    init_conf init_cond;
    // Default intial conditions
    // p1 + p2 + p3 = 1
    init_cond.p1 = 1.0/4; // Intial frequencyrequ of Rock players 
    init_cond.p2 = 1.0/4;//  Intial frequency of Paper players
    init_cond.p3 = 1.0/3;//  Intial frequency of Scissor players
    // The frequency must add up to 1. 
    payoffs rewards;
    // Default enteries for the payoff matrix
    //  First row
    rewards.a = 0.0; // a00
    rewards.b = -1.0; // a01
    rewards.c = 1.0;// a02
    // Second row
    rewards.d = 1.0; // a10 
    rewards.e = 0.0; //
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