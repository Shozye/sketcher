#include "NormalDistribution.hpp"

NormalDistribution::NormalDistribution(double mean, double variance, uint32_t seed)
    : Distribution(seed), distribution(mean, variance) {}

NormalDistribution::NormalDistribution(double mean, double variance, uint32_t seed, uint32_t numInitialCalls)
    : Distribution(seed), distribution(mean, variance) {

    for (uint32_t i = 0; i < numInitialCalls; ++i) {
        getWeight();
    }
}

double NormalDistribution::generateWeight() {
    return distribution(gen);
}