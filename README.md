# Rock-Paper-Scissors Game Simulation 

The goal of this code is to simualte the dynamics of Rock-Paper-Scissors Game using replicator dynamics. to see an example of replicator dyanmics using python check [click here](https://nadiah.org/2022/11/29/replicator-numerical)

## How to run the code ? 

This code uses [`Petsc`](https://petsc.org/release/install/) for solving the ODE system that describes the dynamics of the game. Visulizations are done with `Python`. To compile the project `cd src`, then `make`, this will produce a binary excutable `main.x`, run it with `./main.x` this will create a folder in the root `data` where the results will be stored. To visulize the solution run `Python plot_results.py`. The resulting figure `dynamics.png` will be saved in the newly created directory `./root/figs` 