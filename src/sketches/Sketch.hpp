#pragma once
#include <vector>
#include<cstdint>
#include<fstream>

class Sketch{
    public:
        virtual void consume(const uint8_t* item, int length, double weight) = 0;
        virtual void saveStateIntoStream(std::ofstream ofile) = 0;
        virtual ~Sketch() = 0;
};