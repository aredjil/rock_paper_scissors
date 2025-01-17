#include <petsc.h>
#include "../include/utils.hpp"
#include "../include/monitor.hpp"
#include "jacobian.hpp"
#include "right_hand_side.hpp"

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
    rewards.b = -1.0;// a01
    rewards.c = 1.0;//  a02
    // Second row
    rewards.d = 1.0; // a10 
    rewards.e = 0.0; // a11
    rewards.f = -1.0; // a12
    // Third row 
    rewards.g = -1.0; // a20
    rewards.h = 1.0; // a21
    rewards.i = 0.0; // a22
    // Intilize petsc 
    PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));
    // Get the initial distrubution of the players R, P, S from command line 
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p1", &init_cond.p1, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p2", &init_cond.p2, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p3", &init_cond.p3, NULL));

    // Get the payoff matrix enteries from the command line 
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a00", &rewards.a, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a01", &rewards.b, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a02", &rewards.c, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a10", &rewards.d, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a11", &rewards.e, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a12", &rewards.f, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a20", &rewards.g, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a21", &rewards.h, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a22", &rewards.i, NULL));
    // Intilize the state vector U 
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
    // Monitor to save the states at each time step to a file ../data/results.csv
    // See ../include/utils.hpp for details 
    PetscCall(TSMonitorSet(ts, Monitor, NULL, NULL));
    // Specify the type of the solver 
    PetscCall(TSSetType(ts, TSRK));
    PetscCall(TSRKSetType(ts, TSRK4));
    PetscCall(TSSetMaxTime(ts, 100)); // Number of time steps 
    PetscCall(TSSetExactFinalTime(ts, TS_EXACTFINALTIME_STEPOVER));
    // Get other options from the command line 
    PetscCall(TSSetFromOptions(ts));
    // Solve the system of ODEs 
    PetscCall(TSSolve(ts, U));
    // Finlize the Petsc code
    PetscCall(TSDestroy(&ts));
    PetscCall(VecDestroy(&U));
    PetscCall(PetscFinalize());
    PetscCall(PetscFinalize());
    return 0;
}