#pragma once
#include <random>

class Distribution {
public:
    explicit Distribution(uint32_t seed) : gen(seed), totalWeight(0), numGetCalls(0) {}
    virtual ~Distribution() = default;

    double getWeight() {
        double weight = generateWeight();
        while (weight <= 0){
            weight = generateWeight();
        }

        totalWeight += weight;
        numGetCalls++;
        return weight;
    }

    uint32_t getNumGetCalls() const { return numGetCalls; }
    double getTotalWeight() const { return totalWeight; }
protected:
    std::mt19937 gen;
    virtual double generateWeight() = 0;

private:
    double totalWeight;   
    uint32_t numGetCalls ;   
};
