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

void analyseFastExpSketch(const std::vector<Sketch*>& sketches, double totalWeight) {
    double total_estimate = 0;
    double total_relative_error = 0;
    int AMOUNT_SKETCHES = sketches.size();

    std::cout << "realTotalWeight: " << totalWeight << std::endl;

    for (int i = 0; i < AMOUNT_SKETCHES; i++) {
        FastExpSketch* sketch = dynamic_cast<FastExpSketch*>(sketches[i]);

        auto est = sketch->estimate();
        double error = std::abs(est - totalWeight) / totalWeight;
        std::cout << "FastExpSketch[" << i << "].estimate() = " << est << "elems, RelativeError=" << error *100 << "%" << std::endl;

        total_relative_error += error;
        total_estimate += est;
    }

    double avg_estimate = total_estimate / AMOUNT_SKETCHES;
    double avg_error = total_relative_error / AMOUNT_SKETCHES;
    double error = std::abs(avg_estimate - totalWeight) / totalWeight;
    std::cout << "Average Estimate       = " << avg_estimate << "elems" << std::endl;
    std::cout << "Average Relative Error = " << avg_error * 100 << "%" << std::endl;
    std::cout << "Relative Error of Average Estimate = " << error * 100 << "%" << std::endl;
}

void analyseQSketch(const std::vector<Sketch*>& sketches, uint64_t totalWeight) {
    double total_estimate = 0;
    double total_relative_error = 0;
    int AMOUNT_SKETCHES = sketches.size();

    std::cout << "realTotalWeight: " << totalWeight << std::endl;

    for (int i = 0; i < AMOUNT_SKETCHES; i++) {
        QSketch* sketch = dynamic_cast<QSketch*>(sketches[i]);

        auto est = sketch->estimateNewton();

        double error = std::abs(est - totalWeight) / totalWeight;
        //std::cout << "QSketch[" << i << "].estimate() = " << est << "elems, RelativeError=" << error *100 << "%" << std::endl;
        total_relative_error += error;
        total_estimate += est;
    }

    double avg_estimate = total_estimate / AMOUNT_SKETCHES;
    double avg_error = total_relative_error / AMOUNT_SKETCHES;
    double error = std::abs(avg_estimate - totalWeight) / totalWeight;
    std::cout << "Average Estimate       = " << avg_estimate << "elems" << std::endl;
    std::cout << "Average Relative Error = " << avg_error * 100 << "%" << std::endl;
    std::cout << "Relative Error of Average Estimate = " << error * 100 << "%" << std::endl;
}


void analyseQSketchForEstimation(const std::vector<Sketch*>& sketches, uint64_t totalWeight) {
    int HOW_MANY_PARTS = 10000;
    std::vector<double> total_estimates(HOW_MANY_PARTS+1, 0);
    std::vector<double> total_relative_errors(HOW_MANY_PARTS+1, 0);

    std::cout << "realTotalWeight: " << totalWeight << std::endl;

    for (uint i = 0; i < sketches.size(); i++) {
        QSketch* sketch = dynamic_cast<QSketch*>(sketches[i]);

        for(int x=0; x <= HOW_MANY_PARTS; x++){
            auto est = sketch->estimate(x / (double)HOW_MANY_PARTS);
            double error = std::abs(est - totalWeight) / totalWeight;
            //std::cout << "QSketch[" << i << "].estimate("<<x/(double)HOW_MANY_PARTS<<") = " << est << "elems, RelativeError=" << error *100 << "%" << std::endl;
            total_relative_errors[x] += error;
            total_estimates[x] += est;
        }
    }
    std::cout << "Average relative errors: ";
    for(int x = 0; x <= HOW_MANY_PARTS; x++){
        double avg_error = total_relative_errors[x] / sketches.size();
        std::cout << avg_error * 100 << "% ";
    }
    std::cout << std::endl;

    std::cout << "Average estimates: ";
    for(int x = 0; x <= HOW_MANY_PARTS; x++){
        double avg_estimate = total_estimates[x] / sketches.size();
        std::cout << avg_estimate << " ";
    }
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
        analyseQSketchForEstimation(sketches, dist->getTotalWeight());
    }

    for(auto sketch: sketches){
        delete sketch;
    }
}