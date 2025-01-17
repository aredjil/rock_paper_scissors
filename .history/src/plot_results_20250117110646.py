#/usr/bin/env python3 
import numpy as np 
import maplotlib.pyplot as plt 
import pandas as pd 
import os 
def main():
    # Path where the result of the simulation is stored
    data_path = "../data/results.csv"
    if not os.path.exists(data_path):
        print
    data = pd.read_csv(data_path)



if __name__ =="__main__":
    main()