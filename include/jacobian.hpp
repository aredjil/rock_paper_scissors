#ifndef JACOBIAN_H
#define JACOBIAN_H
#include<petsc.h>
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
#endif // JACOBIAN_H