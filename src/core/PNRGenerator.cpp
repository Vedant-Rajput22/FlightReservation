#include "core/PNRGenerator.h"
#include <random>
#include <cstring>

std::string PNRGenerator::generate()
{
    static const char *ALPHA = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<> d(0, (int)std::strlen(ALPHA) - 1);
    std::string s;
    s.reserve(6);
    for (int i = 0; i < 6; i++)
        s.push_back(ALPHA[d(rng)]);
    return s;
}