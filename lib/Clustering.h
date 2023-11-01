//
// Created by 舒意恒 on 2018/6/22.
//
#ifndef UCI_CPP_MLALGO_CLUSTERING_H
#define UCI_CPP_MLALGO_CLUSTERING_H

#include <cmath>
#include <string>

using namespace std;

template<class T>
double getDist(T &v1, T &v2, const string &dist_type);

// 计算两元组间距离
template<class T>
double getDist(T &v1, T &v2, const string &dist_type) {
    double res = 0;
    if (v1.size() != v2.size())
        throw length_error("The length of the two tuples should be the same.");
    if (dist_type == "Manhattan") {
        // 曼哈顿距离
        for (int i = 0; i < v1.size(); i++)
            res += abs(v1[i] - v2[i]);
    } else if (dist_type == "Euclidean") {
        // 欧几里得距离
        for (int i = 0; i < v1.size(); i++)
            res += (v1[i] - v2[i]) * (v1[i] - v2[i]);
        res = sqrt(res);
    } else if (dist_type == "Chebyshev") {
        // 切比雪夫距离
        if (v1.size() < 3) // 向量维度至少为3
            throw length_error("Calculating Chebyshev distance needs tuple dimension greater than or equal to 3.");
        for (int i = 0; i < v1.size(); i++) {
            const auto t = abs(v1[i] - v2[i]);
            res = max(res, t);
        }
    }
    return res;
}

#endif //UCI_CPP_MLALGO_CLUSTERING_H
