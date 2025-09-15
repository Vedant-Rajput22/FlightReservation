#pragma once
#include <string>


struct Flight {
    std::string flightNo; 
    std::string from; 
    std::string to; 
    std::string depHHMM; 
    std::string arrHHMM; 
    int distanceKm{}; 
    int baseFare{};
    int seatsTotal{};
    int seatsAvailable{};
    std::string days; 
};