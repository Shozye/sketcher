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
    double weightA = 1.0, weightB = 1.0;
    std::string itemDist = "distinct";
    int sketchSize = 400;

    void printInfo();
};