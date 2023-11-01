#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <chrono>
#include "../lib/KMeans.h"

using namespace std;
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // // Load data from CSV file
    ifstream file("/home/ankhanh/learning/data_mining/data/sample_dataset.csv");
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
        data.push_back(row);
    }

    // Close the file
    file.close();
    
    return 0;

}