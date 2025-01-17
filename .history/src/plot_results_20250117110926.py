#/usr/bin/env python3 
import numpy as np 
import matplotlib.pyplot as plt 
import pandas as pd 
import os 
def main():
    # Path where the result of the simulation is stored
    data_path = "../data/results.csv"
    assert os.path.exists(data_path), f""

    data = pd.read_csv(data_path)
    data.head() # View the head of the DataFrame :) 

if __name__ =="__main__":
    main()