#pragma once
#include "Distribution.hpp"

class NormalDistribution : public Distribution {
public:
    NormalDistribution(double mean, double variance, uint32_t seed);
    NormalDistribution(double mean, double variance, uint32_t startingSeed, uint32_t numInitialCalls);

protected:
    double generateWeight() override;

private:
    std::normal_distribution<double> distribution;
};