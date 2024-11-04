#include <iostream>
#include <string>
#include <vector>
#include "Arguments.hpp"
#include "commands/AnalyseCommand.hpp"
#include "commands/SketchCommand.hpp"
#include "commands/TestCommand.hpp"
#include<iomanip>
#include<memory>

CommandType getCommandType(const std::string& command) {
    if (command == "test") return TEST;
    if (command == "sketch") return SKETCH;
    if (command == "analyse") return ANALYSE;
    return UNKNOWN;
}

void displayUsage() {
    std::cout << "Usage:\n"
              << "  ./sketcher test -name <sketch_name> [OPTIONS]\n"
              << "  ./sketcher sketch <filename> -name <sketch_name> [OPTIONS] [!IN DEVELOPMENT!]\n"
              << "  ./sketcher analyse <filename>\n [!IN DEVELOPMENT!]";
}

Arguments parseArguments(int argc, char* argv[]) {
    if (argc < 2) {
        displayUsage();
        exit(1);
    }

    Arguments args;
    args.command = getCommandType(argv[1]);
    if (args.command == UNKNOWN) {
        std::cerr << "Error: Unknown command.\n";
        displayUsage();
        exit(1);
    }

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

               if (arg == "-name" && i + 1 < argc) {
            args.sketchName = argv[++i];
        } else if (arg == "-elems" && i + 1 < argc) {
            args.elements = std::stol(argv[++i]);
        } else if (arg == "-sketches" && i + 1 < argc) {
            args.sketches = std::stol(argv[++i]);
        } else if (arg == "-weightDist" && i + 1 < argc) {
            args.weightDistType = argv[++i];
            // i know that they parse as the same... i stil feel it is better
            if (i + 2 < argc && args.weightDistType == "uniform_int") {
                args.weightDistArg1 = std::stod(argv[++i]);
                args.weightDistArg2 = std::stod(argv[++i]);
            } else if (i + 2 < argc && args.weightDistType == "normal") {
                args.weightDistArg1 = std::stod(argv[++i]);
                args.weightDistArg2 = std::stod(argv[++i]);
            } else if (i + 2 < argc && args.weightDistType == "uniform_real") {
                args.weightDistArg1 = std::stod(argv[++i]);
                args.weightDistArg2 = std::stod(argv[++i]);
            } else {
                std::cerr << "Error: Invalid weight distribution parameters.\n";
                displayUsage();
                exit(1);
            }
        } else if (arg == "-itemDist" && i + 1 < argc) {
            args.itemDist = argv[++i];
        } else if (arg == "-weightSeed" && i + 1 < argc) {
            args.weightDistSeed = std::stoi(argv[++i]);
        } else if (arg == "-sketchSize" && i + 1 < argc) {
            args.sketchSize = std::stoi(argv[++i]);
        } else if (arg == "-sketchSeed" && i + 1 < argc) {
            args.sketchStartSeed = std::stoi(argv[++i]);
        } else if (arg == "-qsketchStart" && i + 1 < argc) {
            args.qsketchStart = std::stod(argv[++i]);
        } else if (arg == "-qsketchEnd" && i + 1 < argc) {
            args.qsketchEnd = std::stod(argv[++i]);
        } else if (arg == "-qsketchAmountPoints" && i + 1 < argc) {
            args.qsketchAmountPoints = std::stoi(argv[++i]);
        } else if (args.command == SKETCH && i == 2) { // handle filename for 'sketch'
            args.filename = arg;
        } else if (args.command == ANALYSE && i == 2) { // handle filename for `analyse`
            args.filename = arg;
        } else {
            std::cerr << "Error: Invalid argument " << arg << "\n";
            displayUsage();
            exit(1);
        }
    }

    if (args.command == SKETCH && args.filename.empty()) {
        std::cerr << "Error: Filename required for sketch command.\n";
        displayUsage();
        exit(1);
    }
    if (args.command == ANALYSE && args.filename.empty()) {
        std::cerr << "Error: Filename required for analyse command.\n";
        displayUsage();
        exit(1);
    }
    if ((args.command == TEST || args.command == SKETCH) && args.sketchName.empty()) {
        std::cerr << "Error: Sketch name required for test and sketch commands.\n";
        displayUsage();
        exit(1);
    }

    std::random_device rd;
    if(args.weightDistSeed == 0){
        args.weightDistSeed = rd();
    }

    return args;
}


int main(int argc, char* argv[]) {
    std::cout << std::fixed << std::setprecision(4);

    Arguments args = parseArguments(argc, argv);
    args.printInfo();

    switch (args.command) {
        case TEST:
            runTestCommand(args);
            break;

        case SKETCH:
            runSketchCommand(args);
            break;

        case ANALYSE:
            runAnalyseCommand(args);
            break;

        default:
            std::cerr << "Unknown command.\n";
            exit(1);
    }

    return 0;
}