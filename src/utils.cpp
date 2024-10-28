#include"utils.hpp"
#include<cmath>
#include<cstdint>
#include<vector>
#include<random>

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

std::vector<uint32_t> range(uint32_t a, uint32_t b){
    if (a > b) return {}; 
    
    std::vector<uint32_t> result(b - a + 1); 
    for (uint32_t i = 0; i < result.size(); ++i) {
        result[i] = a + i; 
    }
    
    return result;
}

std::vector<uint32_t> random_vector(int size, int seed){
    std::vector<uint32_t> seeds(size);
    std::mt19937 seed_generator(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,INT32_MAX);
    for(int i = 0; i < size; i++) {
        seeds[i] = dist(seed_generator);  
    }
    return seeds;
}