#pragma once
#include <string>
#include <vector>
#include <ctime>


int hhmmToMinutes(const std::string& hhmm); 
std::string minutesToHHMM(int minutes); 
int dayOfWeek(const std::string& yyyy_mm_dd); 
bool operatesOn(const std::string& days, int dow);
std::string nowIsoLocal(); 

std::vector<std::string> splitCSVLine(const std::string& line);
std::string trim(const std::string& s);