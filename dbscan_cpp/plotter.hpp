#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;

// This function plots an image for given set of data points.
// The clustering is printed to standard out.
//
// Parameters are:
// data:           a vector<vector<double>> with data points as rows and columns as attributes.
// clustering:     vector of cluster labels for given data points
// eps:            epsilon distance
// minPoints:      minimum amount of points to be in the neighborhood of a
//                 data point p for p to be recognized as a core point.
// maxPoints:      maximum amount a points in the neighborhood of a data point
//                 which leads to maximum membership degree of 1 for points with
//                 at least minPtsMax neighbors. This parameter helps to recognize
//                 more degrees of density. Thats's why it is recommended to use
//                 big values.
void visualizeClustering(vector<vector<double>> data, vector<int> clustering, double eps, int minPoints, int maxPoints) {
    // Eliminates multiple values
    set<int> uniqueLabels(clustering.begin(), clustering.end());

    // Color for each cluster
    vector<double> colors;
    for (int i = 0; i < uniqueLabels.size(); i++) {
        colors.push_back(double(i) / double(uniqueLabels.size()));
    }

    // Colored dot for dot
    for (int k : uniqueLabels) {
        if (k == -1) {
            // Noise is shown as black dots
            continue;
        }

        // True/False matrix to know which dots belongs to cluster
        vector<bool> classMemberMask(clustering.size(), false);
        for (int i = 0; i < clustering.size(); i++) {
            if (clustering[i] == k) {
                classMemberMask[i] = true;
            }
        }

        // Points of the cluster or noise
        vector<vector<double>> xy;
        for (int i = 0; i < data.size(); i++) {
            if (classMemberMask[i]) {
                xy.push_back(data[i]);
            }
        }

        // // Plots points, last parameters means form, color and bordercolor
        // for (auto point : xy) {
        //     cout << point[0] << " " << point[1] << endl;
        // }
        cv::Mat image(480, 640, CV_8UC3, cv::Scalar(255, 255, 255));
        for (auto point : xy) {
            cv::circle(image, cv::Point(point[0], point[1]), 5, cv::Scalar(0, 0, 255), -1);
        }
        cv::imwrite("/home/ankhanh/learning/data_mining/output_fig/fuzzyDBSCAN.jpg", image);
    }

    cout << "# Image: /home/ankhanh/learning/data_mining/output_fig/fuzzyDBSCAN.jpg" << endl;
}
