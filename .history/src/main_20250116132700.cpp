#include <petsc.h>
/*
    This code solves a 
*/
int main(int argc, char *argv[])
{
  PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));
  PetscCall(PetscFinalize());
  return 0;
}