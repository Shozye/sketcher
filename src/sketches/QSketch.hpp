#pragma once
#include <vector>
#include<cstdint>
#include "Sketch.hpp"
#include<string>

class QSketch : public Sketch{
    public:
        QSketch(int m, int bytes, int seed);
        void consume(const uint8_t* item, int length, double weight) override final;
        void saveStateIntoStream() override final;
        double estimateNewton();
        double estimate(double x);
        std::string getstructure();
        ~QSketch();
    private: 
        std::vector<int> structure;
        int structure_size;
        std::vector<uint32_t> permInit;
        std::vector<uint32_t> permutationSwaps;
        std::vector<uint32_t> seeds;
        uint64_t rng_seed;
        uint64_t hash_answer[2];
        int rmin;
        int rmax;
        uint32_t jstar;
        int rand(int, int);
        double dffunc(uint32_t k, double w);
        double ffunc(uint32_t k, double w);
        double Newton(uint32_t k, double c0);
        double InitialValue(int m);

};