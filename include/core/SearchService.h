#pragma once
#include "Storage.h"
#include "DijkstraRouter.h"

struct SearchQuery
{
    std::string from;
    std::string to;
    std::string date;
    int pax{1};
    bool directOnly{false};
};

class SearchService
{
public:
    explicit SearchService(Storage &s);
    std::vector<Itinerary> search(const SearchQuery &q) const;

private:
    Storage &storage_;

    std::vector<Itinerary> enumerateDirect(const SearchQuery &q, int dow) const;
    std::vector<Itinerary> enumerateUpTo2Stops(const SearchQuery &q, int dow) const;
};