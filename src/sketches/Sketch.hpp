#pragma once
#include <vector>
#include<cstdint>

class Sketch{
    public:
        virtual void consume(const uint8_t* item, int length, double weight) = 0;
        virtual void saveStateIntoStream() = 0;
        virtual ~Sketch() = 0;
};