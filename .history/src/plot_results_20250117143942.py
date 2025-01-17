#/usr/bin/env python3 
import numpy as np 
import matplotlib.pyplot as plt 
import pandas as pd 
import os 
def main():
    # Path where the result of the simulation is stored
    data_path = "../data/results.csv"
    figs_path = "../figs"
    # Create the figures directory 
    if not os.path.exists(figs_path):
        os.mkdir(figs_path) 
    # Check if the data file exits in the corresponding path 
    assert os.path.exists(data_path), f"file does not exist {data_path}\n Maybe you forgot to run the simulation code?"
    assert os.path.exists(figs_path), f"file does not exist {data_path}\n Maybe you forgot to run the simulation code?"
    column_indices = ["t", "p1", "p2", "p3"]
    df = pd.read_csv(data_path, header=None, names=column_indices)
    #Plot a line plot for the first population p1
    t = df["t"].copy()
    p1 = df["p1"].copy()
    p2 = df["p2"].copy()
    p3 = df["p3"].copy()

    fig, ax = plt.subplots(ncols=1, nrows=1, figsize=(12, 6))
    ax.plot(t, p1, label="$R(t)$")
    ax.plot(t, p2, label="$P(t)$")
    ax.plot(t, p3, label="$S(t)$")
    ax.set_xlim(0,)
    ax.set_xlabel("t")
    ax.set_ylabel("Population abundance")
    ax.set_title("Dynamics of the Rock-Paper Scissors Game")
    ax.legend()
    plt.savefig(figs_path+"/dynamics.png")
    

if __name__ =="__main__":
    main()