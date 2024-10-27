#pragma once
#include <vector>
#include<cstdint>
#include "Sketch.hpp"
#include<string>

class FastExpSketch : public Sketch{
    public:
        FastExpSketch(int m, int seed);
        void consume(const uint8_t* item, int length, double weight) override final;
        void saveStateIntoStream(std::ofstream ofile) override final;
        double estimate();
        std::string getstructure();
        ~FastExpSketch();
    private: 
        std::vector<double> structure;
        int structure_size;
        std::vector<uint32_t> permInit;
        std::vector<uint32_t> permutationSwaps;
        double max;
        std::vector<uint32_t> seeds;
        uint64_t rng_seed;
        int rand(int, int);
        uint64_t hash_answer[2];
};