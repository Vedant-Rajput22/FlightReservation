#pragma once
#include "Storage.h"
#include "DijkstraRouter.h"
#include "PNRGenerator.h"


class ReservationService {
public:
explicit ReservationService(Storage& s) : storage_(s) {}
std::string book(const Itinerary& itin, const std::string& date, const std::string& name,
const std::string& email, const std::string& phone, int pax, int totalFare);


bool cancel(const std::string& pnr);


private:
Storage& storage_;
};