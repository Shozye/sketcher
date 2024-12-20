#include"Arguments.hpp"
#include<iostream>

void Arguments::printInfo() {
    std::cout << "Arguments(command: ";
    
    // Print command type
    switch (this->command) {
        case TEST:
            std::cout << "Test";
            break;
        case SKETCH:
            std::cout << "Sketch";
            break;
        case ANALYSE:
            std::cout << "Analyse";
            break;
        default:
            std::cout << "Unknown";
            break;
    }

    // Print other member variables
    std::cout << ", sketchName: " << this->sketchName
              << ", filename: " << this->filename
              << ", elements: " << this->elements
              << ", sketches: " << this->sketches
              << ", weightDistType: " << this->weightDistType
              << ", weightDistArg1: " << this->weightDistArg1
              << ", weightDistArg2: " << this->weightDistArg2
              << ", itemDist: " << this->itemDist
              << ", sketchSize: " << this->sketchSize
              << ")" << std::endl;
}
