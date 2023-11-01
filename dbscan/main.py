from fuzzy_dbscan import FuzzyDBSCAN 
import Plotter
import numpy as np
import pandas as pd

def main(eps, minPtsMin, minPtsMax, data, createImage = True): 
    # Create a clustering using fuzzy dbscan
    clustering = FuzzyDBSCAN(data, eps, minPtsMin, minPtsMax)
    
    # Print clustering on standard out
    for clusterid in clustering:
        print(clusterid)
    
    # Show a nice picture if desired
    if createImage:
        Plotter.visualizeClustering(data, clustering, eps, minPtsMin, minPtsMax)

if __name__ == "__main__":
    data = pd.read_csv('/home/ankhanh/learning/data_mining/data/sample_dataset.csv')
    data = np.array(data.iloc[:, 2:4])
    main(eps = 10, minPtsMin = 2, minPtsMax = 5, data = data, createImage = True)