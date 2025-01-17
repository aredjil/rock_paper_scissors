#ifndef UTILS_H
#define UTILS_H
#include<petsc.h>

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
    f[0] = (outcome->a * u[0] + outcome->b * u[1] + outcome->c * u[1]) * u[0];
    f[1] = (outcome->d * u[0] + outcome->e * u[1] + outcome->f * u[1]) * u[1];
    f[2] = (outcome->g * u[0] + outcome->h * u[1] + outcome->i * u[1]) * u[2];

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
    v[2][1] = outcome->h;
    v[2][2] = outcome->i;
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
#endif // UTILS_H