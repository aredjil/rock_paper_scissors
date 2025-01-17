#include <petsc.h>
#include "../include/utils.hpp"

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

    PetscCall(VecCreateSeq(PETSC_COMM_SELF, n, &U));
    PetscCall(VecSetValue(U, 0, init_cond.p1, INSERT_VALUES));
    PetscCall(VecSetValue(U, 1, init_cond.p2, INSERT_VALUES));
    PetscCall(VecSetValue(U, 2, init_cond.p3, INSERT_VALUES));

    PetscCall(MatCreateSeqDense(PETSC_COMM_SELF, n, n, NULL, &A));

    PetscCall(TSCreate(PETSC_COMM_SELF, &ts));

    PetscCall(TSSetRHSFunction(ts, NULL, Probabilities, &rewards));

    PetscCall(TSSetRHSJacobian(ts, A, A, PayOffMatrix, &rewards));
    PetscCall(MatDestroy(&A));

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