#pragma once
#include "Flight.h"
#include <vector>
#include <string>
#include <optional>
#include <mutex>
#include <unordered_map>

struct Passenger
{
    std::string id, name, email, phone;
};

struct Booking
{
    std::string pnr;
    std::string flightNo;
    std::string date;
    std::string paxId;
    std::string status;
    int farePaid{};
    std::string createdAt;
};

class Storage
{
public:
    explicit Storage(std::string baseDir);

    bool loadAll();

    const std::vector<Airport> &airports() const { return airports_; }
    const std::vector<Flight> &flights() const { return flights_; }

    std::vector<const Flight *> flightsFrom(const std::string &from, int dow) const;
    std::vector<const Flight *> flightsBetween(const std::string &from, const std::string &to, int dow) const;

    std::optional<Passenger> findPassengerByEmail(const std::string &email) const;
    Passenger upsertPassenger(const std::string &name, const std::string &email, const std::string &phone);

    bool saveBookingRows(const std::vector<Booking> &rows);
    bool cancelPNR(const std::string &pnr);

    bool updateSeats(const std::string &flightNo, int delta);

    std::string dataDir() const { return baseDir_; }

private:
    std::string baseDir_;
    mutable std::mutex m_;

    std::vector<Airport> airports_;
    std::vector<Flight> flights_;
    std::vector<Passenger> passengers_;

    bool loadAirports();
    bool loadFlights();
    bool loadPassengers();

    bool writeFlightsFile();
    bool appendPassengersFile(const Passenger &p);
    bool appendBookingsFile(const std::vector<Booking> &rows);
};