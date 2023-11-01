#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../lib/KMeans.h"
#include "../lib/Util.h"
#include <chrono>
using namespace std;

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    int choice = 0;
    string inFileName = "/home/ankhanh/learning/data_mining/data/sample_dataset.csv";
    string outFileName = "/home/ankhanh/learning/data_mining/output_fig/cluster1.txt";

    ifstream ifs = openIfs(inFileName);
    ofstream ofs(outFileName);
    
    string dist_type = getDistType();
    vector<Tuple<double>> tuples;
    int k = 10;

    // cout << "Enter the number of clusters:" << endl;
    // cin >> k;

    string line;
    auto cnt = 0;
    ifs >> line; 
    while (ifs >> line) {
        vector<string> nums;
        split(line, ",", nums);
        line.clear();
        Tuple<double> tuple(2);
        // cnt++; // 元组计数
        // if (cnt <= 59) {
        //     tuple.type = 1;
        // } else if (cnt <= 130) {
        //     tuple.type = 2;
        // } else {
        //     tuple.type = 3;
        // }
        for (int i = 2; i < nums.size(); i++) { // string 类型转化为 double 类型
            tuple[i-2] = stringToDouble(nums[i]);
        }
        tuples.push_back(tuple);
    }
    // tuples = normalization(tuples);  // 归一化
    vector<Tuple<double>> means; // k个聚类中心
    // 生成初始质心
    means = random_means(tuples, k);
    vector<vector<Tuple<double>>> clusters = KMeans(tuples, means, ofs, dist_type, k); // 执行 KMeans 算法
    print(clusters, ofs, k); // 打印聚类结果
    cout << "KMeans completed." << endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    ofs << "Execution time: " << duration.count()/60 << " minutes";
    closeFileStream(ifs, ofs);
    return 0;
}