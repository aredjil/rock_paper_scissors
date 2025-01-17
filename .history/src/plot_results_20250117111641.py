#/usr/bin/env python3 
import numpy as np 
import matplotlib.pyplot as plt 
import pandas as pd 
import os 
def main():
    # Path where the result of the simulation is stored
    data_path = "../data/results.csv"
    # Check if the data file exits in the corresponding path 
    assert os.path.exists(data_path), f"file does not exist {data_path}\n Maybe you forgot to run the simulation code?"
    column_indices = ["t", "p1", "p2", "p3"]
    df = pd.read_csv(data_path, header=None, names=column_indices)
    #Plot a line 

if __name__ =="__main__":
    main()