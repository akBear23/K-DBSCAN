#include <iostream>
#include <vector>
#include "fuzzy_dbscan_ver1.hpp"
#include <chrono>
// #include "plotter.hpp"

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // Load data from CSV file
    ifstream file("/home/ankhanh/learning/data_mining/data/beijing_POI_18f.csv");
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
        loc_x = stof(tmp_str);
        // loc_x /= 1000;
        tmp_str="";
        getline(ss, tmp_str, ',');
        loc_y = stof(tmp_str);
        // loc_y /= 1000;
        row.push_back(loc_x);
        row.push_back(loc_y);
        // printf("%f %f\n", loc_x, loc_y);
        data.push_back(row);
    }

    // Close the file
    file.close();

    int eps = 3000;
    int minPtsMin = 4;
    int minPtsMax = 10;
    
    std::vector<int> clustering = FuzzyDBSCAN(data, eps, minPtsMin, minPtsMax);

    freopen("/home/ankhanh/learning/data_mining/output_fig/cluster1.txt", "w", stdout);
    // Print clustering on standard out
    for (int clusterid : clustering) {
        std::cout << clusterid << std::endl;
    }

    // // Show a nice picture if desired
    // // visualizeClustering(data, clustering, 10, 2, 5);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count()/60 << " minutes" << std::endl;
    return 0;
}