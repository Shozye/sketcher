#include"utils.hpp"
#include<cmath>
#include<cstdint>
#include<vector>

double ullto01Interval(uint64_t hash){
    return ((double) hash) * std::pow(2, -64);
}

void swap(std::vector<uint32_t>& vec, int a, int b){
    uint32_t temp = vec[a];
    vec[a] = vec[b];
    vec[b] = temp;
}

double vector_max(std::vector<double> vec){
    double max = vec[0];
    for(uint i = 1; i < vec.size(); ++i){
        if(vec[i] > max){max = vec[i];}
    }
    return max;
}

int argmin(std::vector<int> vec){
    int idx = 0;
    int min = vec[0];

    for(uint i = 1; i < vec.size(); i++){
        if (vec[i] < min){
            idx = i;
            min = vec[i];
        }
    }
    return idx;
}