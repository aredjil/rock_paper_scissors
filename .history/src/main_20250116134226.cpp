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
    f[0] = ;
    f[1] = -lv->c * u[1] + lv->d * u[0] * u[1];
    f[2] = -lv->c * u[1] + lv->d * u[0] * u[1];

    /* Inform PETSc we are done accessing the data */
    PetscCall(VecRestoreArrayRead(U, &u));
    PetscCall(VecRestoreArrayWrite(F, &f));
    PetscFunctionReturn(PETSC_SUCCESS);
};
/*
    Payoff matrix for the game
*/
PetscErrorCode PayOffMatrix() {

};
int main(int argc, char *argv[])
{
    PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));

    PetscCall(PetscFinalize());
    return 0;
}