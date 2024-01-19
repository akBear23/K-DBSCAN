#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../lib_modified/KMeans.h"
#include "../lib_modified/Util.h"
#include <chrono>
#include "../dbscan_cpp/fuzzy_dbscan_ver1_copy.hpp"
using namespace std;
// int eps = 5000;
// int eps = 10;
// int k = 100;   
// double eps = 0.03;
double eps = 1;
// int k = 3;
int minPtsMin = 2;
int minPtsMax = 4;
int k = 100; 
vector<double> group_dis_list(k, 0);

std::vector<DataPoint> dataPoints;
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    int choice = 0;
    string inFileName = "/home/ankhanh/learning/data_mining/data/random_dataset_500_20.csv";
    string outFileName = "/home/ankhanh/learning/data_mining/output_fig/random_dataset_500_20_cluster.txt";
    // string inFileName = "/home/ankhanh/learning/data_mining/data/Toronto_x_y_alphabet_version_02.csv";
    // string outFileName = "/home/ankhanh/learning/data_mining/output_fig/Toronto_x_y_alphabet_version_02_cluster_normalized.txt";

    ifstream ifs = openIfs(inFileName);
    ofstream ofs(outFileName);
    
    string dist_type = getDistType();
    // vector<Tuple<double>> tuples;
    string line;
    auto cnt = 0;
    getline(ifs, line); 
    while (getline(ifs, line)) {
        vector<string> nums;
        split(line, ",", nums);
        line.clear();
        Tuple<double> tuple(2);
        DataPoint tmp;
        tmp.feature_type = nums[0];
        tmp.id = std::stoi(nums[1]);
        for (int i = 2; i < nums.size(); i++) { 
            tuple[i-2] = stringToDouble(nums[i]);
        }
        tmp.point = tuple;
        dataPoints.push_back(tmp);
    }
    
    // dataPoints = normalization(dataPoints);  
    vector<Tuple<double>> means; 
    means = random_means(dataPoints, k);
    vector<vector<DataPoint>> groups = KMeans(dataPoints, means, ofs, dist_type, k); // 执行 KMeans 算法
    // print(clusters, ofs, k); 
    cout << "KMeans completed." << endl;

    // // const int dimen = groups[0][0].size();
    vector<vector<vector<DataPoint>>> clusters(k);
    vector<int> numclu(k);
    int max_num_clu = 0;
    // vector<vector<Tuple<double>>> test;

    for (int label = 0; label < k; label++) {
        // ofs << "No." << label + 1 << " cluster:" << endl;
        clusters[label] = FuzzyDBSCAN(groups[label], eps, minPtsMin, minPtsMax);

        numclu[label] = clusters[label].size();
        max_num_clu = max(max_num_clu, numclu[label]);


        // ofs << "Number of clusters: " << numclu[label] << endl;
        // ofs << clusters[label].size() << endl;
        // // print clusters
        // for(int i=0; i<numclu[label]; i++) {
        //     for(int j=0; j<clusters[label][i].size(); j++){
        //         ofs << clusters[label][i][j][0] << ' ' << clusters[label][i][j][1] << endl;
        //     }
        // }
        // // print groups
        // for (unsigned int i = 0; i < groups[label].size(); i++) {
        //     ofs << i + 1 << ". (";
        //     for (auto j = 0; j < dimen; ++j) {
        //         ofs << groups[label][i][j] << ((j != dimen - 1) ? ", " : "");
        //     }
        //     ofs << ")" << endl;
        // }
    }
    // ofs << "Number of clusters: " << max_num_clu << endl;
    // return 0;
    double dis_r_cluster[k+5][max_num_clu+5];
    for(int i=0; i<k; i++) for(int j=0; j<=max_num_clu; j++) dis_r_cluster[i][j] = 0;
    for(int i=0; i<k; i++) {
        for(int j=0; j<numclu[i]; j++) {
            for(int l=0; l<clusters[i][j].size(); l++) {
                // cout << clusters[i][j][l][0] << ' ' << clusters[i][j][l][1] << endl;
                dis_r_cluster[i][j] = max(dis_r_cluster[i][j], distance(clusters[i][j][l].point, clusters[i][j][0].point));
            }
        }
    }
    // Step 4: Merge border clusters
    // vector flag indicate that cluster c in group a flag[a][c]
    vector<vector<int>> flag(k, vector<int>(max_num_clu, 0));
    int counter = 1;
    for (int a = 0; a < k-1 && !groups[a].empty(); a++) for (int b = 1; b < k && !groups[b].empty(); b++){
        if(dis_K(groups, a, b, group_dis_list) <= eps){
            for(int c = 0; c < numclu[a]; c++) for (int d = 0; d < numclu[b]; d++){
                double dis_db = dis_D(clusters[a], clusters[b], c, d, dis_r_cluster[a][c], dis_r_cluster[b][d]);
                if(dis_db <= eps){
                    if(flag[a][c] == 0 && flag[b][d] == 0){
                        flag[a][c] = counter;
                        flag[b][d] = counter;
                        counter++;
                    }
                    else{
                        flag[a][c] = max(flag[a][c], flag[b][d]);
                        flag[b][d] = max(flag[a][c], flag[b][d]);
                    }
                }
            }
        }
    }
    vector<vector<DataPoint>> groups2(counter);
    for(int a=0; a<k; a++) for(int c=0; c<numclu[a]; c++) if(flag[a][c] != 0){
        groups2[flag[a][c]].insert(groups2[flag[a][c]].end(), clusters[a][c].begin(), clusters[a][c].end());
    }

    vector<vector<DataPoint>> final_clusters;
    int total_clusters = 0;
    for(int i=1; i<=counter-1; i++){
        final_clusters = FuzzyDBSCAN(groups2[i], eps, minPtsMin, minPtsMax);
        if(final_clusters.size() < 1) continue;
        for(int j=0; j<final_clusters.size(); j++){
            total_clusters++;
            ofs << "Clusters " << total_clusters << endl;
            for(int l=0; l<final_clusters[j].size(); l++){
                ofs << final_clusters[j][l].feature_type << "," << final_clusters[j][l].id << "," << final_clusters[j][l].point[0] << "," << final_clusters[j][l].point[1] << endl;
            }
        }
    }
    for(int a=0; a<k; a++) for(int c=0; c<numclu[a]; c++) if(flag[a][c] == 0){
        total_clusters++;
        ofs << "Clusters not merged " << total_clusters << endl;
        for(int l=0; l<clusters[a][c].size(); l++){
            ofs << clusters[a][c][l].feature_type << "," << clusters[a][c][l].id << "," << clusters[a][c][l].point[0] << "," << clusters[a][c][l].point[1] << endl;
        }
    }
    cout << "DBSCAN in groups completed." << endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    ofs << "Execution time: " << duration.count()/60 << " minutes";
    closeFileStream(ifs, ofs);
    return 0;
}