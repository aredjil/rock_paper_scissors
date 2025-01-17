#/usr/bin/env python3 
import numpy as np 
import maplotlib.pyplot as plt 
import pandas as pd 
import os 
def main():
    # Path where the result of the simulation is stored
    data_path = "../data/results.csv"
    if not os.path.exists(data_path):
        print(f"The file {data_path} does not exits")
    data = pd.read_csv(data_path)
    data.head()



if __name__ =="__main__":
    main()