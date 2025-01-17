#include <petsc.h>
/*
    This code solves a 2 players 
*/
int main(int argc, char *argv[])
{
  PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));
  PetscCall(PetscFinalize());
  return 0;
}