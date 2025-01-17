#ifndef JACOBIAN_H
#define JACOBIAN_H
#include<petsc.h>
/**
  Callback function to save the results to a file
 */
PetscErrorCode Monitor(TS ts, PetscInt stepid, PetscReal t, Vec U, void *ctx)
{
    const PetscScalar *u;
    std::ofstream file; 
    file = create_data_file();

    PetscFunctionBeginUser;
    PetscCall(VecGetArrayRead(U, &u));
    file<<t<<","<<u[0]<<","<<u[1]<<","<<u[2]<<"\n";
    PetscCall(VecRestoreArrayRead(U, &u));
    // Close the file 
    file.close();
    PetscFunctionReturn(PETSC_SUCCESS);
}
#endif // JACOBIAN_H

