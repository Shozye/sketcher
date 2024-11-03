#pragma once
#include <vector>
#include<cstdint>
#include "Sketch.hpp"
#include<string>

class QSketch : public Sketch{
    public:
        QSketch(int m, int bytes, int seed);
        void consume(const uint8_t* item, int length, double weight) override final;
        void saveStateIntoStream(std::ofstream ofile) override final;
        double estimateNewton();
        double estimate(double x);
        std::string getstructure();
        ~QSketch();
    private: 
        int structure_size;
        int rmin;
        int rmax;
        std::vector<int> structure;
        std::vector<uint32_t> seeds;
        uint32_t jstar;

        uint64_t rng_seed;
        std::vector<uint32_t> permInit;
        std::vector<uint32_t> permutationSwaps;
        double HELPER_FOR_ESTIMATION = 0;

        uint64_t hash_answer[2];
        int rand(int, int);
        double dffunc(uint32_t k, double w);
        double ffunc(uint32_t k, double w);
        double Newton(uint32_t k, double c0);
        double InitialValue();

};