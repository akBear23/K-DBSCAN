//
// Created by 舒意恒 on 2018/5/29.
//
#ifndef KMEANS_H
#define KMEANS_H

#define Epsilon 0.1

#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "Tuple.h"
#include "Clustering.h"

using namespace std;

int getClusterNum(vector<Tuple<double>> &means, Tuple<double> &tuple, const string &dist_type);

Tuple<double> getMeans(vector<Tuple<double>> &cluster);

double getClusterError(vector<vector<Tuple<double>>> &cluster, vector<Tuple<double>> means, string &dist_type);

vector<Tuple<double>> random_means(vector<Tuple<double>> &tuples, unsigned int k);

void print(vector<Tuple<double>> clusters[], ofstream &ofs, int k);

void KMeans(vector<Tuple<double>> &tuples, vector<Tuple<double>> means, ofstream &ofs, int k);

// 根据质心，判断当前元组属于哪个簇
inline int getClusterNum(vector<Tuple<double>> &means, Tuple<double> &tuple, const string &dist_type) {
    auto dist = getDist(means[0], tuple, dist_type);
    auto label = 0; // 簇编号，从0开始
    for (unsigned int i = 1; i < means.size(); i++) {
        const auto tmp = getDist(means[i], tuple, dist_type);
        if (tmp < dist) {
            dist = tmp;
            label = i;
        }
    }
    return label;
}

// 获取当前簇的质心
inline Tuple<double> getMeans(vector<Tuple<double>> &cluster) {
    const int m = static_cast<const int>(cluster.size()); // 样本数
    if (!m)
        throw length_error("There is no sample for calculation.");
    const int n = cluster[0].size(); // 维数
    Tuple<double> res(static_cast<unsigned int>(n));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            res[j] += cluster[i][j];
    for (int j = 0; j < n; j++)
        res[j] /= m;
    return res;
}

// 获得给定簇集的误差
inline double getClusterError(vector<vector<Tuple<double>>> &cluster, vector<Tuple<double>> means,
                              string &dist_type) {
    double res = 0;
    for (unsigned int i = 0; i < means.size(); i++) {
        for (auto &j : cluster[i])
            res += getDist(j, means[i], dist_type);
    }
    return res;
}

inline void print(vector<vector<Tuple<double>>> &clusters, ofstream &ofs, const int k) {
    const int dimen = clusters[0][0].size();
    for (int label = 0; label < k; label++) {
        ofs << "No." << label + 1 << " cluster:" << endl;
        for (unsigned int i = 0; i < clusters[label].size(); i++) {
            ofs << i + 1 << ". (";
            for (auto j = 0; j < dimen; ++j) {
                ofs << clusters[label][i][j] << ((j != dimen - 1) ? ", " : "");
            }
            ofs << ")" << endl;
        }
    }
}

inline vector<vector<Tuple<double>>>
KMeans(vector<Tuple<double>> &tuples, vector<Tuple<double>> means, ofstream &ofs, const string &dist_type, int k = 2) {
    vector<vector<Tuple<double>>> clusters(k); // k个簇
    auto choice = 0; // 用户输入的选择
    // 根据初始质心给簇赋值
    int label = 0;
    for (auto &tuple : tuples) {
        label = getClusterNum(means, tuple, dist_type);
        clusters[label].push_back(tuple);
    }
    double oldError = -1;
    double newError = getClusterError(clusters, means, const_cast<string &>(dist_type));
    // ofs << "The initial sum of the overall error:" << newError << endl;
    int count = 0; // 迭代计数
    while (abs(newError - oldError) >= Epsilon) // 迭代，直到邻近两次函数值相差不到1，即准则函数值不发生明显变化时，算法终止
    {
        // ofs << "No." << ++count << " iteration begins." << endl;
        for (int i = 0; i < k; i++) { // 更新每个簇的中心点
            means[i] = getMeans(clusters[i]);
        }
        oldError = newError;
        newError = getClusterError(clusters, means, const_cast<string &>(dist_type)); // 计算新的准则函数值
        for (auto i = 0; i < k; i++) { // 清空每个簇
            clusters[i].clear();
        }
        // 根据新的质心获得新的簇
        for (auto &tuple : tuples) {
            label = getClusterNum(means, tuple, dist_type);
            clusters[label].push_back(tuple);
        }
        // ofs << "The sum of the overall errors after this iteration: " << newError << endl;
    }
    // 打印每簇数据
    // ofs << "The result is:" << endl;
    // print(clusters, ofs, k);
    // ofs << endl;

    return clusters;
}

inline vector<Tuple<double>> random_means(vector<Tuple<double>> &tuples, unsigned int k) {
    // 随机生成聚类中心
    assert(tuples.size() != 0);
    const int dimen = tuples[0].size(); // 维数
    vector<Tuple<double>> means(k, Tuple<double>(dimen)); // 返回k个随机聚类中心
    Tuple<double> min_bound(dimen);
    Tuple<double> max_bound(dimen);
    for (auto &tuple : tuples) {
        for (int j = 0; j < dimen; j++) {
            min_bound[j] = min(min_bound[j], tuple[j]);
            max_bound[j] = max(max_bound[j], tuple[j]);
        }
    }
    srand(static_cast<unsigned int>(time(nullptr)));
    for (auto &mean : means) {
        for (unsigned int j = 0; j < dimen; j++) {
            double t = rand() % 100 / double(101);
            mean[j] = t * (max_bound[j] - min_bound[j]) + min_bound[j]; // 生成随机数
        }
    }
    return means;
}

#endif // KMEANS_H
