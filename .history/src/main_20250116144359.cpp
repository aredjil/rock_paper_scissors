#include <petsc.h>
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

/*
    Probabilities
*/
PetscErrorCode Probabilities(TS ts, PetscReal t, Vec U, Vec F, void *ctx)
{
    const PetscScalar *u;
    PetscScalar *f;
    payoffs *outcome = (payoffs *)ctx;

    PetscFunctionBeginUser;
    /* Tell PETSc we will only read values from U */
    PetscCall(VecGetArrayRead(U, &u));

    /* Tell PETSc we will write values to F */
    PetscCall(VecGetArrayWrite(F, &f));

    /* Evaluate F(t,U) */
    f[0] = (outcome->a * u[0] + outcome->b* u[1] + outcome->c* u[1]) * u[0];
    f[1] = (outcome->e * u[0] + outcome->f* u[1] + outcome->g* u[1]) * u[1];
    f[2] = (outcome->g * u[0] + outcome->h* u[1] + outcome->i* u[1]) * u[2];

    /* Inform PETSc we are done accessing the data */
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
    /* Evaluate dF/dU */
    PetscCall(VecGetArrayRead(U, &u));
    v[0][0] = outcome->a;
    v[0][1] = outcome->b;
    v[0][2] = outcome->c;
    v[1][0] = outcome->d;
    v[1][1] = outcome->e;
    v[1][2] = outcome->f;
    v[2][0] = outcome->g;
    v[2][1] = 1;
    v[2][2] = params->ep;
    PetscCall(VecRestoreArrayRead(U, &u));

    /* Insert matrix values */
    PetscCall(MatSetValues(P, 3, i, 3, i, (PetscScalar *)v, INSERT_VALUES));
    PetscCall(MatAssemblyBegin(P, MAT_FINAL_ASSEMBLY));
    PetscCall(MatAssemblyEnd(P, MAT_FINAL_ASSEMBLY));

    /* If you are experimenting with jacobian testing, try
       - commenting any of the v[][] assignments above
       - commenting out the next call to see the shift value
    */
    // PetscCall(MatZeroEntries(P));

    if (A != P)
    { /* This is true when we use matrix-free or finite differencing */
        PetscCall(MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY));
        PetscCall(MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY));
    }
    PetscFunctionReturn(PETSC_SUCCESS);
};
int main(int argc, char *argv[])
{
    TS ts;
    Mat A;
    Vec U;
    PetscReal x, y, z;
    x = 0.3;
    y = 0.4;
    z = 0.2;

    rpsg parms;

    parms.ep = 0.0;

    PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));

    PetscCall(PetscOptionsGetReal(NULL, NULL, "-ep", &parms.ep, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-x", &x, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-y", &y, NULL));
    PetscCall(PetscOptionsGetReal(NULL, NULL, "-z", &z, NULL));

    PetscCall(VecCreateSeq(PETSC_COMM_SELF, 3, &U));
    PetscCall(VecSetValue(U, 0, x, INSERT_VALUES));
    PetscCall(VecSetValue(U, 1, y, INSERT_VALUES));
    PetscCall(VecSetValue(U, 2, z, INSERT_VALUES));

    PetscCall(MatCreateSeqDense(PETSC_COMM_SELF, 3, 3, NULL, &A));

    /* Create ODE solver context */
    PetscCall(TSCreate(PETSC_COMM_SELF, &ts));

    /* Inform PETSc how to compute the residual of the ODE */
    PetscCall(TSSetRHSFunction(ts, NULL, Probabilities, &parms));

    /* Inform PETSc how to compute the Jacobian.

       The latter can be omitted since PETSc supports finite-differencing
       and matrix-free approximations, see bratu.c for additional information.
       Run with

         '-ts_type beuler -ts_dt 1.0 -snes_test_jacobian -snes_test_jacobian_view -ts_max_steps 1 -snes_max_it 1'

       to check the correctness of the user-provided RHSJacobian function.
       Here we use '-ts_type beuler -ts_dt 1.0' to help debugging, since we know in this case the solver shift is 1.
    */
    PetscCall(TSSetRHSJacobian(ts, A, A, PayOffMatrix, &parms));
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