#pragma once
#include<string>
#include<stdint.h>

enum CommandType { TEST, SKETCH, ANALYSE, UNKNOWN };

struct Arguments {
    CommandType command;
    std::string sketchName;
    std::string filename;
    int weightDistSeed = 0;
    uint64_t elements = 10000;
    uint64_t sketches = 1;
    std::string weightDistType = "uniform_int";
    double weightDistArg1 = 1.0, weightDistArg2 = 1.0;
    std::string itemDist = "distinct";
    int sketchSize = 400;
    int sketchStartSeed = 42;

    // these are special variables, that are useful for now, to make tests on estimator.
    double qsketchStart = 0.25;
    double qsketchEnd = 0.75;
    int qsketchAmountPoints = 5000;

    void printInfo();
};