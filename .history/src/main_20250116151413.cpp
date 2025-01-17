#include <petsc.h>
#include."./include/utils.hpp"
/*
    This code solves a 3 players zero-sum game, rock paper scissors
*/
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

int main(int argc, char *argv[])
{
    TS ts;
    Mat A;
    Vec U;
    init_conf init_cond;
    /*
        Default values for the intial conditions
    */
    init_cond.p1 = 0.3;
    init_cond.p2 = 0.4;
    init_cond.p3 = 0.3;

    payoffs rewards;
    /*
        Default payoffs correspond to the Rock-Paper-Scissors game
    */
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

    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p1", &init_cond.p1, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p2", &init_cond.p2, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-p3", &init_cond.p3, NULL));

    PetscCall(VecCreateSeq(PETSC_COMM_SELF, 3, &U));
    PetscCall(VecSetValue(U, 0, init_cond.p1, INSERT_VALUES));
    PetscCall(VecSetValue(U, 1, init_cond.p2, INSERT_VALUES));
    PetscCall(VecSetValue(U, 2, init_cond.p3, INSERT_VALUES));

    PetscCall(MatCreateSeqDense(PETSC_COMM_SELF, 3, 3, NULL, &A));

    /* Create ODE solver context */
    PetscCall(TSCreate(PETSC_COMM_SELF, &ts));

    /* Inform PETSc how to compute the residual of the ODE */
    PetscCall(TSSetRHSFunction(ts, NULL, Probabilities, &rewards));

    /* Inform PETSc how to compute the Jacobian.

       The latter can be omitted since PETSc supports finite-differencing
       and matrix-free approximations, see bratu.c for additional information.
       Run with

         '-ts_type beuler -ts_dt 1.0 -snes_test_jacobian -snes_test_jacobian_view -ts_max_steps 1 -snes_max_it 1'

       to check the correctness of the user-provided RHSJacobian function.
       Here we use '-ts_type beuler -ts_dt 1.0' to help debugging, since we know in this case the solver shift is 1.
    */
    PetscCall(TSSetRHSJacobian(ts, A, A, PayOffMatrix, &rewards));
    PetscCall(MatDestroy(&A));

    /* Set default solvers and final time (can be overridden at command line)
       Here we use a 4th order Runge-Kutta solver (ode45 in MATLAB).
    */
    PetscCall(TSSetType(ts, TSRK));
    PetscCall(TSRKSetType(ts, TSRK4));
    PetscCall(TSSetMaxTime(ts, 100));
    PetscCall(TSSetExactFinalTime(ts, TS_EXACTFINALTIME_STEPOVER));

    /* Allow command-line customization. Run

         ./lotka-volterra -help | grep '-ts_'

       to see some of the available options.

       This folder also contains a custom ODE solver written in Python, available via:

          '-python -ts_type python -ts_python_type myts.tsodeint'

       If you have an X client available, run with:
         - '-ts_monitor_lg_solution' to plot the solution as time evolves
         - '-ts_monitor_draw_solution_phase 0,0,10,10' to plot the phase diagram
    */
    PetscCall(TSSetFromOptions(ts));

    /* Solve the ODE */
    PetscCall(TSSolve(ts, U));

    /* Cleanup */
    PetscCall(TSDestroy(&ts));
    PetscCall(VecDestroy(&U));
    PetscCall(PetscFinalize());

    PetscCall(PetscFinalize());
    return 0;
}