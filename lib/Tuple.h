//
// Created by 舒意恒 on 2018/6/22.
//


#ifndef UCI_CPP_MLALGO_TUPLE_H
#define UCI_CPP_MLALGO_TUPLE_H

#include <vector>

using namespace std;

template<class T>
class Tuple {
public:
    int type; // 类别标号
    vector<T> v;

    Tuple() : type(0) {
    }

    Tuple(const Tuple &tuple) { // 复制构造函数
        this->type = tuple.type;
        this->v = tuple.v;
    }

    Tuple(unsigned int size) : type(0) {
        v.resize(size, 0);
    }

    int size() const {
        return v.size();
    }

    T &operator[](int index) {
        if (index >= v.size()) {
            throw out_of_range("Index exceeds vector length.");
        }
        return v[index];
    }
};

#endif //UCI_CPP_MLALGO_TUPLE_H
