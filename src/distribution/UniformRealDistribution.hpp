#pragma once
#include "Distribution.hpp"

class UniformRealDistribution : public Distribution {
public:
    UniformRealDistribution(double weightA, double weightB, uint32_t seed);
    UniformRealDistribution(double weightA, double weightB, uint32_t seed, uint32_t numInitialCalls);

protected:
    double generateWeight() override;

private:
    std::uniform_real_distribution<double> distribution;
};