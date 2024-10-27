#include "UniformIntDistribution.hpp"

UniformIntDistribution::UniformIntDistribution(int weightA, int weightB, uint32_t seed)
    : Distribution(seed), distribution(weightA, weightB) {}

UniformIntDistribution::UniformIntDistribution(int weightA, int weightB, uint32_t seed, uint32_t numInitialCalls)
    : Distribution(seed), distribution(weightA, weightB) {

    for (uint32_t i = 0; i < numInitialCalls; ++i) {
        getWeight();
    }
}

double UniformIntDistribution::generateWeight() {
    return distribution(gen);
}