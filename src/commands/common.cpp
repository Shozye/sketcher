#include"common.hpp"
#include"../sketches/FastExpSketch.hpp"
#include"../sketches/QSketch.hpp"
#include<iostream>
#include"../distribution/NormalDistribution.hpp"
#include"../distribution/UniformIntDistribution.hpp"
#include"../distribution/UniformRealDistribution.hpp"

Sketch* createSketch(const Arguments& args, uint sketchIndex){
    if (args.sketchName == "fastexpsketch"){
        return new FastExpSketch(args.sketchSize, args.sketchStartSeed + sketchIndex);
    } else if (args.sketchName == "qsketch"){
        return new QSketch(args.sketchSize, 1, args.sketchStartSeed + sketchIndex);
    } else {
        std::cout << "Error: invalid sketch name" << args.sketchName << std::endl;
        exit(1);
    }
}

uint64_t getItemToConsume(const Arguments& args, uint64_t item){
    if(args.itemDist == "distinct"){
        return item;
    } 
    else if (args.itemDist == "repeated"){
        return 42;
    } else {
        std::cout << "Error: invalid item distribution" << args.itemDist << std::endl;
        exit(1);
    }
}

std::unique_ptr<Distribution> getWeightDistribution(const Arguments& args){
    std::unique_ptr<Distribution> weightDistribution;

    if (args.weightDistType == "normal") {
        weightDistribution = std::make_unique<NormalDistribution>(args.weightDistArg1, args.weightDistArg2, args.weightDistSeed);
    } else if (args.weightDistType == "uniform_int") {
        weightDistribution = std::make_unique<UniformIntDistribution>(static_cast<int>(args.weightDistArg1), static_cast<int>(args.weightDistArg2), args.weightDistSeed);
    } else if (args.weightDistType == "uniform_real") {
        weightDistribution = std::make_unique<UniformRealDistribution>(args.weightDistArg1, args.weightDistArg2, args.weightDistSeed);
    } else {
        std::cout << "Error: invalid weight distribution:" << args.weightDistType << std::endl;
        exit(1);
    }
    return weightDistribution;
}