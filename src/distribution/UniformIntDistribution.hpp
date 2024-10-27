#pragma once
#include "Distribution.hpp"

class UniformIntDistribution : public Distribution {
public:
    UniformIntDistribution(int weightA, int weightB, uint32_t seed);
    UniformIntDistribution(int weightA, int weightB, uint32_t seed, uint32_t numInitialCalls);

protected:
    double generateWeight() override;

private:
    std::uniform_int_distribution<int> distribution;
};