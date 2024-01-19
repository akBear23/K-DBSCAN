//
// Created by 舒意恒 on 2018/6/25.
//

#ifndef UCI_CPP_MLALGO_HCA_H
#define UCI_CPP_MLALGO_HCA_H

#include <vector>
#include <iostream>
#include <fstream>
#include "Tuple.h"
#include "Clustering.h"

#define INF 0x3f3f3f3f
struct Model {
    int i = INF;
    int j = INF;
    double dist = INF;
};

vector<vector<double>> &getDistMat(vector<Tuple<double>> &data, vector<vector<double>> &distMat, string &dist_type) {
    const auto size = data.size();
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            distMat[i][j] = getDist(data[i], data[j], dist_type);
        }
    }
    return distMat;
}

template<class T>
void findClosestClusters(vector<T> &distMat, Model &model) {
    const auto size = distMat.size();
    double minDist = INF;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (distMat[i][j] < minDist && distMat[i][j] != 0) {
                minDist = distMat[i][j];
                model.i = i;
                model.j = j;
                model.dist = distMat[i][j];
            }
        }
    }
}

void HCA(vector<Tuple<double>> &data, string &dist_type, ofstream &ofs) {
    const auto size = data.size();
    Model minModel;
    vector<vector<double>> distMat(size, vector<double>(size));
    auto cnt = 0;

    getDistMat(data, distMat, dist_type);
    while (true) {
        ofs << "No." << (++cnt) << " iteration" << endl;
//        for (int i = 0; i < size; i++) {
//            for (int j = 0; j < size; j++) {
//                ofs << distMat[i][j] << " ";
//            }
//            ofs << endl;
//        }
        findClosestClusters(distMat, minModel);
        if (minModel.dist == 0 || cnt == size - 1) // 找不到距离最近的两个簇时，迭代结束
            break;
        ofs << "Combine cluster " << (minModel.i + 1) << " and " << (minModel.j + 1) << endl;
        ofs << "The distance is " << minModel.dist << endl;
        distMat[minModel.i][minModel.j] = 0; // 更新矩阵
        for (int i = 0; i < size; i++) {
            // 如果合并点p1 与 p2，则只保留 p1、p2 其中之一与其他点的距离，取较小值
            if (distMat[i][minModel.i] <= distMat[i][minModel.j])
                distMat[i][minModel.j] = 0;
            else
                distMat[i][minModel.i] = 0;
            if (distMat[minModel.i][i] <= distMat[minModel.j][i])
                distMat[minModel.j][i] = 0;
            else
                distMat[minModel.i][i] = 0;
        }
    }
}


#endif //UCI_CPP_MLALGO_HCA_H
