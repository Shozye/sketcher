#include "FastExpSketch.hpp"
#include<vector>
#include<limits>
#include<random>
#include<cstring>
#include<string>
#include"../utils.hpp"
#include"../hash/MurmurHash3.h"
#include<iostream>

FastExpSketch::FastExpSketch(int m, int seed) 
    :   
        structure(std::vector<double>(m, std::numeric_limits<double>::max())),
        structure_size(m),
        permutationSwaps(std::vector<unsigned int>(m)),
        max(std::numeric_limits<double>::max())
        {
            permInit = std::vector<uint32_t>(m);
            for(int i = 1; i < m+1; ++i){
                permInit[i-1] = i;
            }

            seeds = std::vector<uint32_t>(m);

            std::mt19937 seed_generator(seed);
            std::uniform_int_distribution<std::mt19937::result_type> dist(1,INT32_MAX); // distribution in range [1, 6]
            for(int i = 0; i < m; i++) seeds[i] = dist(seed_generator);

            rng_seed = 0;            
        }

FastExpSketch::~FastExpSketch(){}

void FastExpSketch::saveStateIntoStream(std::ofstream ofile){

}

int FastExpSketch::rand(int min, int max){
    rng_seed = rng_seed * 1103515245 + 12345;
    auto temp = (unsigned)(rng_seed/65536) % 32768;
    return (temp % (max-min)) + min;
}

void FastExpSketch::consume(const uint8_t* item, int length, double weight){
    double S = 0;
    bool updateMax = false;
    std::memcpy(&rng_seed, item, sizeof(uint64_t));
    int k;
    for (k = 0; k < structure_size; ++k){   
        MurmurHash3_x64_128(&item[0], length, seeds[k], hash_answer);

        double U = ullto01Interval(hash_answer[0]);
        double E = -std::log(U) / weight;
        S += E/(double)(this->structure_size-k);
        if ( S >= this->max ) break;

        uint32_t r = rand(k, structure_size);
        permutationSwaps[k] = r;
        swap(permInit, r, k);
        uint32_t j = permInit[k]-1;

        if(structure[j] == max) updateMax = true;
        structure[j] = std::min(structure[j], S);
    }
    --k;
    for(;k >= 0; --k){
        swap(permInit, permutationSwaps[k], k);
    }
    if(updateMax) { max = vector_max(structure); }
}
        
double FastExpSketch::estimate(){
    double s = 0;
    for(double elem: structure){
        s += elem;
    }
    return (structure_size-1) / s;
}

std::string FastExpSketch::getstructure(){
    std::string output = "";
    for(double elem: structure){
        output += std::to_string(elem) + " ";
    }
    return output;
}