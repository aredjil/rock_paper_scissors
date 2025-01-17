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
    df = pd.read_csv(data_path, index_col=False)
    print(data.head()) # View the head of the DataFrame :) 

if __name__ =="__main__":
    main()