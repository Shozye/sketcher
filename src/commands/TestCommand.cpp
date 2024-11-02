#include "TestCommand.hpp"
#include <iostream>
#include"../sketches/Sketch.hpp"
#include"../sketches/FastExpSketch.hpp"
#include"../sketches/QSketch.hpp"
#include<vector>
#include<random>
#include <functional>
#include<chrono>
#include"../distribution/Distribution.hpp"
#include <memory>
#include"common.hpp"
#include"../utils.hpp"

void analyseFastExpSketch(const std::vector<Sketch*>& sketches, double totalWeight) {
    double total_estimate = 0;
    double total_relative_error = 0;

    for (uint i = 0; i < sketches.size(); i++) {
        FastExpSketch* sketch = dynamic_cast<FastExpSketch*>(sketches[i]);

        auto est = sketch->estimate();
        double error = relError(est, totalWeight);

        total_relative_error += error;
        total_estimate += est;
    }

    double avg_estimate = total_estimate / sketches.size();
    double avg_error = total_relative_error / sketches.size();

    std::cout << "[Real_Total_Weight]= " << totalWeight << std::endl;
    std::cout << "[Average_Estimate]= " << avg_estimate << "elems" << std::endl;
    std::cout << "[Average_Relative_Error]= " << avg_error << std::endl;
}

void analyseQSketch(const std::vector<Sketch*>& sketches, uint64_t totalWeight) {
    double total_estimate = 0;
    double total_relative_error = 0;

    for (uint i = 0; i < sketches.size(); i++) {
        QSketch* sketch = dynamic_cast<QSketch*>(sketches[i]);

        auto est = sketch->estimateNewton();
        double error = relError(est, totalWeight);
        total_relative_error += error;
        total_estimate += est;
    }

    double avg_estimate = total_estimate / sketches.size();
    double avg_error = total_relative_error / sketches.size();

    std::cout << "[Real_Total_Weight]= " << totalWeight << std::endl;
    std::cout << "[Average_Estimate]= " << avg_estimate << "elems" << std::endl;
    std::cout << "[Average_Relative_Error]= " << avg_error << std::endl;
}


void analyseQSketchForEstimation(const std::vector<Sketch*>& sketches, uint64_t totalWeight, const Arguments& args) {
    double START = args.qsketchStart;
    double END = args.qsketchEnd;
    int AMOUNT_POINTS = args.qsketchAmountPoints; 
    // generating linspace that starts at START, ends at END, has AMOUNT_POINTS+1 points.

    std::vector<double> total_estimates(AMOUNT_POINTS+1, 0);
    std::vector<double> total_relative_errors(AMOUNT_POINTS+1, 0);

    for (uint i = 0; i < sketches.size(); i++) {
        QSketch* sketch = dynamic_cast<QSketch*>(sketches[i]);

        for(int part_index=0; part_index <= AMOUNT_POINTS; part_index++){
            auto est = sketch->estimate(part_index * (END-START) / (double) AMOUNT_POINTS);
            double error = relError(est, totalWeight);

            total_relative_errors[part_index] += error;
            total_estimates[part_index] += est;
        }
    }

    std::cout << "[START_END_AMOUNTPOINTS]= " << START << " " << END << " " << AMOUNT_POINTS << std::endl; 
    std::cout << "[Real_Total_Weight]= " << totalWeight << std::endl;
    std::cout << "[Average_Estimates]= ";
    for(int x = 0; x <= AMOUNT_POINTS; x++)
        std::cout << total_estimates[x] / sketches.size() << " ";
    std::cout << std::endl;

    std::cout << "[Average_Relative_Errors]= ";
    for(int x = 0; x <= AMOUNT_POINTS; x++)
        std::cout << total_relative_errors[x] / sketches.size() << " ";
    std::cout << std::endl;

}



void runTestCommand(const Arguments& args) {
    auto dist = getWeightDistribution(args);
    std::vector<Sketch*> sketches;
    for(uint sketch_index = 0; sketch_index < args.sketches; sketch_index++){
        sketches.push_back(createSketch(args, sketch_index));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for(uint64_t itemIndex = 1; itemIndex <= args.elements; itemIndex++){
        uint64_t item_to_consume = getItemToConsume(args, itemIndex);
        double weight = dist->getWeight();
        for(Sketch* sketch: sketches){
            sketch -> consume((uint8_t*)&item_to_consume, 8, weight);
        }

        if(args.elements > PRINT_EVERY_X_PERCENT && itemIndex % (args.elements / PRINT_EVERY_X_PERCENT) == 0){
            auto end = std::chrono::high_resolution_clock::now();
            auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << PRINT_EVERY_X_PERCENT * (itemIndex / (args.elements / PRINT_EVERY_X_PERCENT)) << "% of input consumed by sketches in " << int_ms.count() << " ms" << std::endl;
        }
    }


    if (args.sketchName == "fastexpsketch") {
        analyseFastExpSketch(sketches, dist->getTotalWeight());
    } else if(args.sketchName == "qsketch") {
        analyseQSketch(sketches, dist->getTotalWeight());
        analyseQSketchForEstimation(sketches, dist->getTotalWeight(), args);
    }

    for(auto sketch: sketches){
        delete sketch;
    }
}