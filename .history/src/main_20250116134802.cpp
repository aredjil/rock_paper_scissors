#include <petsc.h>
/*
    This code solves a 2 players zero-sum game, rock paper scissors
*/
/*
    Paramaters struct
*/
typedef struct RPSG
{
    PetscReal ep; /* Epsilon */
} rpsg;
/*
    Probabilities
*/
PetscErrorCode Probabilities(TS ts, PetscReal t, Vec U, Vec F, void *ctx)
{
    const PetscScalar *u;
    PetscScalar *f;
    rpsg *params = (rpsg *)ctx;

    PetscFunctionBeginUser;
    /* Tell PETSc we will only read values from U */
    PetscCall(VecGetArrayRead(U, &u));

    /* Tell PETSc we will write values to F */
    PetscCall(VecGetArrayWrite(F, &f));

    /* Evaluate F(t,U) */
    f[0] = (params->ep * u[0] - u[1] + u[2]) * u[0];
    f[1] = (u[0] + params->ep * u[1] - u[2]) * u[1];
    f[2] = (-u[0] + u[1] + params->ep * u[2]) * u[2];

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
    rpsg *params = (rpsg *)ctx;


    PetscFunctionBeginUser;
    /* Evaluate dF/dU */
    PetscCall(VecGetArrayRead(U, &u));
    v[0][0] = lv->a - lv->b * u[1];
    v[0][1] = -lv->b * u[0];
    v[0][2] =;
    v[1][0] = lv->d * u[1];
    v[1][1] = -lv->c + lv->d * u[0];
    v[1][2] =;
    v[2][0]
    v[2][1]
    v[2][2]
    PetscCall(VecRestoreArrayRead(U, &u));

    /* Insert matrix values */
    PetscCall(MatSetValues(P, 2, i, 2, i, (PetscScalar *)v, INSERT_VALUES));
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
    PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));

    PetscCall(PetscFinalize());
    return 0;
}