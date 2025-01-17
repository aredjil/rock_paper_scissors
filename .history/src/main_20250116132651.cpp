#include <petsc.h>
/*

*/
int main(int argc, char *argv[])
{
  PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));
  PetscCall(PetscFinalize());
  return 0;
}