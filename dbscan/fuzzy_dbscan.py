import numpy as np
import pandas as pd

# This function executes a fuzzy dbscan algorithm.
# Parameters are:
# eps:            epsilon distance
# minPtsMin:      minimum amount of points to be in the neighborhood of a
#                 data point p for p to be recognized as a core point.
# mintPtsMax:     maximum amount a points in the neighborhood of a data point
#                 which leads to maximum membership degree of 1 for points with
#                 at least minPtsMax neighbors. This parameter helps to recognize
#                 more degrees of density. Thats's why it is recommended to use
#                 big values.
# data:           A numpy.ndarray with data points as rows and columns as attributes
#
# Returns a vector of cluster ids, one cluster id for each data point.

def FuzzyDBSCAN(data, eps, minPtsMin, minPtsMax):
    distances = computeDistance(data)
    numPoints = data.shape[0]
    currentCluster = -1
    memberships = [[-1] for i in range(numPoints)]
    visited = [False] * numPoints
    for i in range(numPoints):
        if(visited[i]):
            continue

        neighbors = computeNeighbors(distances, i, eps)

        # if data point is core point
        if(len(neighbors) > minPtsMin):
            visited[i] = True
            currentCluster = currentCluster + 1
            if currentCluster > 0:
                for row in memberships:
                    row.append(-1)
            
            expandFuzzyCluster(i, neighbors, eps, minPtsMin, minPtsMax, visited, memberships, distances, currentCluster)
    
    clustering = []
    for i in range(numPoints):
        cluster = -1
        maxMembership = -1 
        for j in range(currentCluster+1):
            currentMembership = memberships[i][j]
            if(currentMembership > maxMembership):
                cluster = j
                maxMembership = currentMembership
        clustering.append(cluster)
    return clustering 
            
def expandFuzzyCluster(point, neighbors, eps, minPtsMin, minPtsMax, visited, memberships, distances, currentCluster):
    borderPoints = set()
    corePoints = set()
    memberships[point][currentCluster] = computeMembershipDegree(len(neighbors), minPtsMin, minPtsMax)
    corePoints.add(point)
    while neighbors:
        i = neighbors.pop()
        if not visited[i] and not(i in borderPoints):
            neighbors2 = computeNeighbors(distances, i, eps)
            visited[i] = True
            corePoints.add(i)
            neighbors = neighbors.union(neighbors2)
            memberships[i][currentCluster] = computeMembershipDegree(len(neighbors2), minPtsMin, minPtsMax)
        else:
            borderPoints.add(i)
    while borderPoints:
        i = borderPoints.pop()
        neighbors2 = computeNeighbors(distances, i, eps)
        coreNeighbors = neighbors2.intersection(corePoints)
        biggestMembership = -1
        while coreNeighbors:
            j = coreNeighbors.pop()
            currentMembership = memberships[j][currentCluster]
            biggestMembership = max(biggestMembership, currentMembership)
        
        memberships[i][currentCluster] = biggestMembership
    return

# Function to compute the eps-neighborhood of a data point as a set of indizes.
#
# Parameters are:
# distances:    numpy.ndarray that is an upper triangular matrix with diagonal 0-entries.
# point:        Index in distance matrix of data point to compute the neighborhood for.
# eps:          Value to define distance of epsilon neighborhood epsilon.
#
# Returns set of neighbor points as indizes into distance matrix.
def computeNeighbors(distances, point, eps):
    neighbors = set()

    for i in range(point):
        if distances[i][point] <= eps:
            neighbors.add(i)
    
    numPoints = distances.shape[1]
    for i in range(point, numPoints):
        if distances[point][i] <= eps:
            neighbors.add(i)

    return neighbors

# Function to compute the membership degree of a data point to a cluster.
def computeMembershipDegree(numNeighbors, minPtsMin, minPtsMax):
    if(numNeighbors >= minPtsMax):
        return 1
    if(numNeighbors <= minPtsMin):
        return 0
    return float(numNeighbors - minPtsMin) / float(minPtsMax - minPtsMin)

# Function to compute the Euclidean distance matrix
def computeDistance(data):
    lenArrayOfPoints = len(data)
    dimension = len(data[0])

    distanceMatrix = []
    for i in range(lenArrayOfPoints):
        distanceCollector = [[]]
        for j in range(lenArrayOfPoints):
            if i >= j: 
                distanceCollector[0].extend([0])
            else:
                euclideanDistanceAddition = 0
                for k in range(dimension):
                    euclideanDistanceAddition = euclideanDistanceAddition + (data[i][k] - data[j][k])**2
                euclideanDistanceAddition = euclideanDistanceAddition**.5
                distanceCollector[0].extend([euclideanDistanceAddition])
        distanceMatrix.extend(distanceCollector)
    distanceMatrix = np.array(distanceMatrix)
    return distanceMatrix

data = pd.read_csv('/home/ankhanh/learning/data_mining/data/sample_dataset.csv')
new_data = np.array(data.iloc[:, 2:4])
distanceMatrix = computeDistance(new_data)
# print(repr(distanceMatrix[1]))
clustering = FuzzyDBSCAN(new_data, 10, 2, 5)
for clusterid in clustering:
        print(clusterid, end = ' ')
