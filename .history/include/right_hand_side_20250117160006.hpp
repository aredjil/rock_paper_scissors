#ifndef RHS_H
#define RHS_H
/*
    Probabilities
*/
PetscErrorCode Probabilities(TS ts, PetscReal t, Vec U, Vec F, void *ctx)
{
    const PetscScalar *u;
    PetscScalar *f;
    payoffs *outcome = (payoffs *)ctx;

    PetscFunctionBeginUser;
    PetscCall(VecGetArrayRead(U, &u));

    PetscCall(VecGetArrayWrite(F, &f));

    f[0] = (outcome->a * u[0] + outcome->b * u[1] + outcome->c * u[2]) * u[0];
    f[1] = (outcome->d * u[0] + outcome->e * u[1] + outcome->f * u[2]) * u[1];
    f[2] = (outcome->g * u[0] + outcome->h * u[1] + outcome->i * u[2]) * u[2];

    PetscCall(VecRestoreArrayRead(U, &u));
    PetscCall(VecRestoreArrayWrite(F, &f));
    PetscFunctionReturn(PETSC_SUCCESS);
};
#endif //RHS_H
