#pragma once
#include<cstdint>
#include<vector>

double ullto01Interval(uint64_t hash);
void swap(std::vector<uint32_t>& vec, int a, int b);
double vector_max(std::vector<double>);
int argmin(std::vector<int>);

std::vector<uint32_t> range(uint32_t a, uint32_t b);
std::vector<uint32_t> random_vector(int size, int seed);