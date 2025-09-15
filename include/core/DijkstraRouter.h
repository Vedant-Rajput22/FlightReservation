#pragma once
#include "Graph.h"
#include <string>
#include <vector>

struct Leg
{
    const Flight *flight;
};

struct Itinerary
{
    std::vector<Leg> legs;
    int totalFare{};   
    int totalMinutes{}; 
};

class DijkstraRouter
{
public:
    std::vector<std::vector<std::string>> fewestStops(
        const Graph &g,
        const std::string &src,
        const std::string &dst,
        int topK = 5) const;
};