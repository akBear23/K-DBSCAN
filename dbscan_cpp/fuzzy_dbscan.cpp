#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include <sstream>
//#include <bits/stdc++.h>
using namespace std;

// Function to compute the Euclidean distance matrix
vector<vector<double>> computeDistance(vector<vector<double>> data) {
    int lenArrayOfPoints = data.size();
    int dimension = data[0].size();

    vector<vector<double>> distanceMatrix(lenArrayOfPoints, vector<double>(lenArrayOfPoints, 0));
    for (int i = 0; i < lenArrayOfPoints; i++) {
        for (int j = i + 1; j < lenArrayOfPoints; j++) {
            double euclideanDistanceAddition = 0;
            for (int k = 0; k < dimension; k++) {
                euclideanDistanceAddition += pow(data[i][k] - data[j][k], 2);
            }
            euclideanDistanceAddition = sqrt(euclideanDistanceAddition);
            distanceMatrix[i][j] = euclideanDistanceAddition;
            distanceMatrix[j][i] = euclideanDistanceAddition;
        }
    }
    return distanceMatrix;
}
double distance(vector<double> a, vector<double> b) {
    double euclideanDistanceAddition = 0;
    for (int k = 0; k < a.size(); k++) {
        euclideanDistanceAddition += pow(a[k] - b[k], 2);
    }
    euclideanDistanceAddition = sqrt(euclideanDistanceAddition);
    return euclideanDistanceAddition;
}

// Function to compute the membership degree of a data point to a cluster.
double computeMembershipDegree(int numNeighbors, int minPtsMin, int minPtsMax) {
    if (numNeighbors >= minPtsMax) {
        return 1;
    }
    if (numNeighbors <= minPtsMin) {
        return 0;
    }
    return double(numNeighbors - minPtsMin) / double(minPtsMax - minPtsMin);
}

// Function to compute the eps-neighborhood of a data point as a set of indizes.
//
// Parameters are:
// distances:    vector<vector<double>> that is an upper triangular matrix with diagonal 0-entries.
// point:        Index in distance matrix of data point to compute the neighborhood for.
// eps:          Value to define distance of epsilon neighborhood epsilon.
//
// Returns set of neighbor points as indizes into distance matrix.
vector<int> computeNeighbors(vector<vector<double>> distances, int point, double eps) {
    vector<int> neighbors;

    for (int i = 0; i < point; i++) {
        if (distances[i][point] <= eps) {
            neighbors.push_back(i);
        }
    }

    int numPoints = distances.size();
    for (int i = point; i < numPoints; i++) {
        if (distances[point][i] <= eps) {
            neighbors.push_back(i);
        }
    }

    return neighbors;
}

void expandFuzzyCluster(int point, vector<int> neighbors, double eps, int minPtsMin, int minPtsMax, vector<bool>& visited, vector<vector<double>>& memberships, vector<vector<double>> distances, int& currentCluster) {
    set<int> borderPoints;
    set<int> corePoints;
    memberships[point][currentCluster] = computeMembershipDegree(neighbors.size(), minPtsMin, minPtsMax);
    corePoints.insert(point);
    while (!neighbors.empty()) {
        int i = neighbors.back();
        neighbors.pop_back();
        if (!visited[i] && !borderPoints.count(i)) {
            vector<int> neighbors2 = computeNeighbors(distances, i, eps);
            visited[i] = true;
            corePoints.insert(i);
            neighbors.insert(neighbors.end(), neighbors2.begin(), neighbors2.end());
            memberships[i][currentCluster] = computeMembershipDegree(neighbors2.size(), minPtsMin, minPtsMax);
        }
        else {
            borderPoints.insert(i);
        }
    }
    while (!borderPoints.empty()) {
        int i = *borderPoints.begin();
        borderPoints.erase(borderPoints.begin());
        vector<int> neighbors2 = computeNeighbors(distances, i, eps);
        set<int> coreNeighbors;
        for (int j : neighbors2) {
            if (corePoints.count(j)) {
                coreNeighbors.insert(j);
            }
        }
        double biggestMembership = -1;
        for (int j : coreNeighbors) {
            double currentMembership = memberships[j][currentCluster];
            biggestMembership = max(biggestMembership, currentMembership);
        }
        memberships[i][currentCluster] = biggestMembership;
    }
}

vector<int> FuzzyDBSCAN(vector<vector<double>> data, double eps, int minPtsMin, int minPtsMax) {
    vector<vector<double>> distances = computeDistance(data);
    int numPoints = data.size();
    int currentCluster = -1;
    vector<vector<double>> memberships(numPoints, vector<double>(1, -1));
    vector<bool> visited(numPoints, false);
    for (int i = 0; i < numPoints; i++) {
        if (visited[i]) {
            continue;
        }

        vector<int> neighbors = computeNeighbors(distances, i, eps);

        // if data point is core point
        if (neighbors.size() > minPtsMin) {
            visited[i] = true;
            currentCluster++;
            if (currentCluster > 0) {
                for (auto& row : memberships) {
                    row.push_back(-1);
                }
            }

            expandFuzzyCluster(i, neighbors, eps, minPtsMin, minPtsMax, visited, memberships, distances, currentCluster);
        }
    }

    vector<int> clustering(numPoints, -1);
    for (int i = 0; i < numPoints; i++) {
        int cluster = -1;
        double maxMembership = -1;
        for (int j = 0; j <= currentCluster; j++) {
            double currentMembership = memberships[i][j];
            if (currentMembership > maxMembership) {
                cluster = j;
                maxMembership = currentMembership;
            }
        }
        clustering[i] = cluster;
    }
    return clustering;
}

int main() {
    // Example usage
    // vector<vector<double>> data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    ifstream file("C:\\Users\Admin\source\repos\akBear23\K-DBSCAN\data\sample_dataset.csv");
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return 1;
    }
    vector<vector<double>> data;
    string line = "";
    getline(file, line);
    line = "";
    while (getline(file, line)) {
        vector<double> row;
        string feature_type;
        int id; 
        double loc_x, loc_y;
        double value;
        string tmp_str="";
        stringstream ss(line);
        
        getline(ss, feature_type, ',');
        getline(ss, tmp_str, ',');
        id = stoi(tmp_str);
        tmp_str="";
        getline(ss, tmp_str, ',');
        loc_x = stod(tmp_str);
        tmp_str="";
        getline(ss, tmp_str, ',');
        loc_y = stod(tmp_str);
        row.push_back(loc_x);
        row.push_back(loc_y);
        // printf("%f %f\n", loc_x, loc_y);
        data.push_back(row);
    }

    // Close the file
    file.close();

    double eps = 10;
    int minPtsMin = 2;
    int minPtsMax = 5;

    // cout << data[0].size() << endl;
    vector<vector<double>> distaceMatrix = computeDistance(data);
    // for (int i = 0; i < distaceMatrix.size(); i++) {
    //     for (int j = 0; j < distaceMatrix[i].size(); j++) {
    //         cout << distaceMatrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    vector<int> clustering = FuzzyDBSCAN(data, eps, minPtsMin, minPtsMax);
    for (int cluster : clustering) {
        cout << cluster << " ";
    }
    cout << endl;
    return 0;
}

