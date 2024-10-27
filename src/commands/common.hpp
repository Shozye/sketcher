#pragma once
#include"../Arguments.hpp"
#include"../sketches/Sketch.hpp"
#include<memory>
#include"../distribution/Distribution.hpp"

Sketch* createSketch(const Arguments& args, uint sketchIndex);
uint64_t getItemToConsume(const Arguments& args, uint64_t item);
std::unique_ptr<Distribution> getWeightDistribution(const Arguments& args);

constexpr int PRINT_EVERY_X_PERCENT = 10;