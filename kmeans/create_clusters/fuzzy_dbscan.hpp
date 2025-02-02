#include <iostream>
#include <vector>
#include <cmath>
// #include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <set>
#include "lib_modified/Tuple.h"
using namespace std;

// Function to compute the Euclidean distance matrix
// double distance(Tuple<double> a, Tuple<double> b) {
//     double euclideanDistanceAddition = 0;
//     for (int k = 0; k < a.size(); k++) {
//         euclideanDistanceAddition += pow(a[k] - b[k], 2);
//     }
//     euclideanDistanceAddition = sqrt(euclideanDistanceAddition);
//     return euclideanDistanceAddition;
// }

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
vector<int> computeNeighbors(vector<DataPoint> data, int point_x, double eps) {
    vector<int> neighbors;
    // freopen("/home/ankhanh/learning/data_mining/output_fig/neighbors.txt", "w", stdout);
    for (int i = 0; i < data.size(); i++){
        // int cnt = 0;
        // cout << "dis: " << distance(data[i], data[point]) << endl;
        if (distance(data[i].point, data[point_x].point) <= eps) {
            neighbors.push_back(i);
            // std::cout << i << ' ';
            // cnt++;
        }
        // std::cout << point << ' ' << cnt << '\n';
        // cout << endl;
    }
    return neighbors;
}

void expandFuzzyCluster(vector<DataPoint> data, int point, vector<int> neighbors, double eps, int minPtsMin, int minPtsMax, vector<bool>& visited, vector<vector<double>>& memberships, int& currentCluster) {
    set<int> borderPoints;
    set<int> corePoints;
    memberships[point][currentCluster] = computeMembershipDegree(neighbors.size(), minPtsMin, minPtsMax);
    corePoints.insert(point);
    while (!neighbors.empty()) {
        int i = neighbors.back();
        neighbors.pop_back();
        if (!visited[i] && !borderPoints.count(i)) {
            vector<int> neighbors2 = computeNeighbors(data, i, eps);
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
        vector<int> neighbors2 = computeNeighbors(data, i, eps);
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

vector<vector<DataPoint>> FuzzyDBSCAN(vector<DataPoint> data, double eps, int minPtsMin, int minPtsMax) {
    int numPoints = data.size();
    int currentCluster = -1;
    vector<vector<double>> memberships(numPoints, vector<double>(1, -1));
    vector<bool> visited(numPoints, false);
    for (int i = 0; i < numPoints; i++) {
        if (visited[i]) {
            continue;
        }

        vector<int> neighbors = computeNeighbors(data, i, eps);

    //     // if data point is core point
        if (neighbors.size() > minPtsMin) {
            visited[i] = true;
            currentCluster++;
            if (currentCluster > 0) {
                for (auto& row : memberships) {
                    row.push_back(-1);
                }
            }

            expandFuzzyCluster(data, i, neighbors, eps, minPtsMin, minPtsMax, visited, memberships, currentCluster);
        }
    }
    vector<vector<DataPoint>> clusters(currentCluster + 1);
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
    //     clustering[i] = cluster;
        
        if(cluster >= 0) clusters[cluster].push_back(data[i]);
    }
    
    // return clustering;
    return clusters;
}
double maxDistances(vector<Tuple<double>> data, vector<vector<int>> clusters){
    double max_distance = 0;
    for (int i = 0; i < clusters.size(); i++) {
        for (int j = 0; j < clusters[i].size(); j++) {
            max_distance = max(max_distance, distance(data[clusters[i][j]], data[clusters[i][0]]));
        }
    }
    return max_distance;
}
// int main() {
//     auto start = std::chrono::high_resolution_clock::now();
//     ifstream file("/home/ankhanh/learning/data_mining/data/sample_dataset.csv");
//     if (!file.is_open()) {
//         cout << "Error opening file" << endl;
//         return 1;
//     }
//     vector<vector<double>> data;
//     string line = "";
//     getline(file, line);
//     line = "";
//     while (getline(file, line)) {
//         vector<double> row;
//         string feature_type;
//         int id; 
//         double loc_x, loc_y;
//         double value;
//         string tmp_str="";
//         stringstream ss(line);
        
//         getline(ss, feature_type, ',');
//         getline(ss, tmp_str, ',');
//         id = stoi(tmp_str);
//         tmp_str="";
//         getline(ss, tmp_str, ',');
//         loc_x = stod(tmp_str);
//         tmp_str="";
//         getline(ss, tmp_str, ',');
//         loc_y = stod(tmp_str);
//         row.push_back(loc_x);
//         row.push_back(loc_y);
//         // printf("%f %f\n", loc_x, loc_y);
//         data.push_back(row);
//     }

//     // Close the file
//     file.close();

//     double eps = 7000;
//     int minPtsMin = 2;
//     int minPtsMax = 5;

//     // freopen("/home/ankhanh/learning/data_mining/output_fig/cluster.txt", "w", stdout);
//     vector<int> clustering = FuzzyDBSCAN(data, eps, minPtsMin, minPtsMax);
//     for (int cluster : clustering) {
//         cout << cluster << " ";
//     }
//     cout << endl;
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration = end - start;
//     std::cout << "Execution time: " << duration.count()/60 << " minutes" << std::endl;
//     return 0;
// }