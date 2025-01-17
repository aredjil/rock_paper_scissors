#include <petsc.h>
/*
    This code solves a 2 players zero-sum game, rock paper scissors
*/
/*
    Payoff matrix for the game 
*/
int main(int argc, char *argv[])
{
  PetscCall(PetscInitialize(&argc, &argv, NULL, NULL));

  PetscCall(PetscFinalize());
  return 0;
}