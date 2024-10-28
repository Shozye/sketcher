#include "QSketch.hpp"
#include<vector>
#include<limits>
#include<random>
#include<cstring>
#include<string>
#include"../utils.hpp"
#include"../hash/MurmurHash3.h"
#include<iostream>

QSketch::QSketch(int m, int bytes, int seed) 
    :   
        structure_size(m), // to niby nie ale tak do serializacji/deserializacji
        rmin(-std::pow(2, bytes*8 - 1) + 1), // to potrzebne do serializacji/deserializacji
        rmax(std::pow(2, bytes*8-1) - 1), // to potrzebne do serializacji/deserializacji
        structure(std::vector<int>(m, rmin)), // to bardzo bardzo potrzebne do serializacji/deserializacji
        seeds(random_vector(seed, m)), // seedy sa bardzo potrzebne by pamiętać te same elementy.
        jstar(0), // niby nie trzeba, mozna inicjalizowac z argmin (structure) ale niech zostanie. :) 

        rng_seed(0), // rng_seed nie jest potrzebny bo jest ustawiany tak naprawde przy każdym consume
        permInit(range(1,m)), // nie jest potrzebny, jest permutacją która się cofa na swoje miejsce przy każdym consume
        permutationSwaps(std::vector<unsigned int>(m)) // nie jest potrzebny, nie obchodzi nas co tam jest dopoki nie zaczniemy inicjalizowac jej w consume
        {
         
        }

QSketch::~QSketch(){}

void QSketch::saveStateIntoStream(std::ofstream ofile){

}


void QSketch::consume(const uint8_t* item, int length, double weight){
    std::memcpy(&rng_seed, item, sizeof(uint64_t));
    int j;
    double r = 0;
    for (j = 0; j < structure_size; ++j){   
    
        MurmurHash3_x64_128(&item[0], length, seeds[j], hash_answer);
        double U = ullto01Interval(hash_answer[0]);
        double E = std::log(U)/((double)(this->structure_size-j)*weight);
        r = r - E;
        int y = (int)std::floor(-std::log2(r));
        if ( y <= structure[jstar] ) break; /* early stop */


        uint32_t k = rand(j, structure_size);
        permutationSwaps[j] = k;
        swap(permInit, k, j);
        
        if (y > structure[permInit[j]]) {
            structure[permInit[j]] = std::min(rmax, std::max(y, rmin));
            if(permInit[j] == jstar){
                jstar = argmin(structure);
            }
        }
    }
    --j;
    for(;j >= 0; --j){
        swap(permInit, permutationSwaps[j], j);
    }
}

std::string QSketch::getstructure(){
    std::string output = "";
    for(int elem: structure){
        output += std::to_string(elem) + " ";
    }
    return output;
}

double QSketch::estimate(double x){
    double s = 0;
    for(int elem: structure){
        s += std::pow(2, (-(double)elem)-x);
    }
    return (structure_size-1) / s;
}
    


int QSketch::rand(int min, int max){
    rng_seed = rng_seed * 1103515245 + 12345;
    auto temp = (unsigned)(rng_seed/65536) % 32768;
    return (temp % (max-min)) + min;
}


double QSketch::ffunc(uint32_t k, double w) 
{
    double res = 0;
    double e = 2.718282;
    for (uint i = 0; i < k; ++i) {
        double x = pow(2.0, -this->structure[i] - 1);
        double ex = pow(e, w * x);
        res += x * (2.0 - ex) / (ex - 1.0);
    }
    return res;
}


double QSketch::dffunc(uint32_t k, double w) 
{
    double res = 0;
    double e = 2.718282;
    for (uint i = 0; i < k; ++i) {
        double x = pow(2.0, -this->structure[i] - 1);
        double ex = pow(e, w * x);
        res += -x * x * ex * pow(ex - 1, -2);
    }
    return res;
}

double QSketch::Newton(uint32_t k, double c0) 
{
    double err = 1e-5;
    double c1 = c0 - ffunc(k, c0) / dffunc(k, c0);

    while (abs (c1 - c0) > err) {
        c0 = c1;
        c1 = c0 - (double)ffunc(k, c0) / dffunc(k, c0);
    }
    return c1;
}

double QSketch::InitialValue(int m)
{
    double c0 = 0.0;
    double tmp_sum = 0.0;
 
    for(int i=0; i<m; i++) { 
        tmp_sum += pow(2, -this->structure[i]); 
    }

    c0 = (double)(m-1) / tmp_sum;
    return c0;
}


