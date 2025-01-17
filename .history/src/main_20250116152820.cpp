#include <petsc.h>
#include "../include/utils.hpp"

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

    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a00", &rewards.a, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a01", &rewards.b, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a02", &rewards.c, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a10", &rewards.d, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a11", &rewards.e, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a12", &rewards.f, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a20", &rewards.g, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a21", &rewards.h, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-a22", &rewards.i, NULL));

    PetscCall(VecCreateSeq(PETSC_COMM_SELF, 3, &U));
    PetscCall(VecSetValue(U, 0, init_cond.p1, INSERT_VALUES));
    PetscCall(VecSetValue(U, 1, init_cond.p2, INSERT_VALUES));
    PetscCall(VecSetValue(U, 2, init_cond.p3, INSERT_VALUES));

    PetscCall(MatCreateSeqDense(PETSC_COMM_SELF, 3, 3, NULL, &A));

    /* Create ODE solver context */
    PetscCall(TSCreate(PETSC_COMM_SELF, &ts));

    PetscCall(TSSetRHSFunction(ts, NULL, Probabilities, &rewards));

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