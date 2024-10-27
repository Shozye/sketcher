#include "UniformRealDistribution.hpp"

UniformRealDistribution::UniformRealDistribution(double weightA, double weightB, uint32_t seed)
    : Distribution(seed), distribution(weightA, weightB) {}

UniformRealDistribution::UniformRealDistribution(double weightA, double weightB, uint32_t seed, uint32_t numInitialCalls)
    : Distribution(seed), distribution(weightA, weightB) {

    for (uint32_t i = 0; i < numInitialCalls; ++i) {
        getWeight();
    }
}

double UniformRealDistribution::generateWeight() {
    return distribution(gen);
}