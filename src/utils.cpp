#include"utils.hpp"
#include<cmath>
#include<cstdint>
#include<vector>
#include<random>

double relError(double estimated, double real){
    return std::abs(estimated - real) / real;
}

double ullto01Interval(uint64_t hash){
    double divisor = 0.5;
    divisor *= divisor; // -2
    divisor *= divisor; // -4
    divisor *= divisor; // -8
    divisor *= divisor; // -16
    divisor *= divisor; // -32
    divisor *= divisor; // 2^-64
    return ((double) hash) * (divisor);
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

double pow_2(int exponent) {
    if(exponent == 0) { return 1; }
    if(exponent < 0) { return 1.0 / pow_2(-exponent); }

    double result = 1;
    double base = 2;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }
    return result;
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