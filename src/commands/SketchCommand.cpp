#include "SketchCommand.hpp"
#include <iostream>
#include "../distribution/Distribution.hpp"
#include<memory>
#include"../sketches/Sketch.hpp"
#include"common.hpp"
#include<chrono>
#include<fstream>

void runSketchCommand(const Arguments& args) {
    // Check if a save file exists to restore any previous state
    std::string mainFileName = args.filename;
    std::vector<Sketch*> sketches;
    std::unique_ptr<Distribution> dist;
    uint64_t starting_item = 1;

    // if (std::ifstream mainFile{mainFileName, std::ios::binary}) {
        // Deserialize state from main save file if it exists
        // mainFile >> deserialize args;      // Restore arguments state
        // mainFile >> deserialize sketches;  // Restore sketch state
        // mainFile >> deserialize *dist;     // Restore distribution state
        // mainFile >> deserialize starting_item 
    // } else {
        dist = getWeightDistribution(args);
        for (uint sketch_index = 0; sketch_index < args.sketches; sketch_index++) {
            sketches.push_back(createSketch(args, sketch_index));
        }
    // }

    auto start = std::chrono::high_resolution_clock::now();

    for (uint64_t item = starting_item; item <= args.elements; item++) {
        uint64_t item_to_consume = getItemToConsume(args, item);
        double weight = dist->getWeight();
        
        for (Sketch* sketch : sketches) {
            sketch->consume((uint8_t*)&item_to_consume, 8, weight);
        }

        if (item % (args.elements / PRINT_EVERY_X_PERCENT) == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << PRINT_EVERY_X_PERCENT * (item / (args.elements / PRINT_EVERY_X_PERCENT))
                      << "% of input consumed by sketches in " << int_ms.count() << " ms" << std::endl;

            std::string tempFileName = args.filename + ".tmp";
            std::ofstream tempFile(tempFileName, std::ios::binary);

            if (!tempFile.is_open()) {
                std::cerr << "Error: Unable to open temporary save file for writing." << std::endl;
                continue; // Skip this save attempt if temp file can't be opened
            }

            // Save sketches and distribution states to the temp file. placeholders for now
            // tempFile << serialize(args)
            // tempFile << serialize(sketches);
            // tempFile << serialize(*dist);

            tempFile.close();

            // atomic rename
            if (std::rename(tempFileName.c_str(), mainFileName.c_str()) != 0) {
                std::cerr << "Error: Could not rename temp file to main save file." << std::endl;
            }
        }
    }

    for (auto sketch : sketches) {
        delete sketch;
    }
}