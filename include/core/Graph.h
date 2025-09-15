#pragma once
#include "Flight.h"
#include <string>
#include <unordered_map>
#include <vector>


struct Edge {
    std::string to;
    const Flight* flight; 
    int weight; 
};


class Graph {
    public:
        void build(const std::vector<Flight>& flights, int dow);
        const std::vector<Edge>& neighbors(const std::string& code) const;
        private:
        std::unordered_map<std::string, std::vector<Edge>> adj_;
};