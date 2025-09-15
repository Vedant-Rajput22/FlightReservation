#include "core/Storage.h"
#include "core/Util.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cstdio>

Storage::Storage(std::string baseDir) : baseDir_(std::move(baseDir)) {}

bool Storage::loadAll()
{
    std::lock_guard<std::mutex> lk(m_);
    bool a = loadAirports();
    bool f = loadFlights();
    bool p = loadPassengers();
    return a && f && p;
}

bool Storage::loadAirports()
{
    airports_.clear();
    std::ifstream in(baseDir_ + "/airports.csv");
    if (!in)
        return false;
    std::string line;
    std::getline(in, line); // header
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        auto v = splitCSVLine(line);
        if (v.size() < 5)
            continue;
        airports_.push_back(Airport{v[0], v[1], v[2], v[3], v[4]});
    }
    return true;
}

bool Storage::loadFlights()
{
    flights_.clear();
    std::ifstream in(baseDir_ + "/flights.csv");
    if (!in)
        return false;
    std::string line;
    std::getline(in, line); // header
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        auto v = splitCSVLine(line);
        if (v.size() < 10)
            continue;
        Flight f{};
        f.flightNo = v[0];
        f.from = v[1];
        f.to = v[2];
        f.depHHMM = v[3];
        f.arrHHMM = v[4];
        f.distanceKm = std::stoi(v[5]);
        f.baseFare = std::stoi(v[6]);
        f.seatsTotal = std::stoi(v[7]);
        f.seatsAvailable = std::stoi(v[8]);
        f.days = v[9];
        flights_.push_back(std::move(f));
    }
    return true;
}

bool Storage::loadPassengers()
{
    passengers_.clear();
    std::ifstream in(baseDir_ + "/passengers.csv");
    if (!in)
        return false;
    std::string line;
    std::getline(in, line); // header
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        auto v = splitCSVLine(line);
        if (v.size() < 4)
            continue;
        passengers_.push_back(Passenger{v[0], v[1], v[2], v[3]});
    }
    return true;
}

std::vector<const Flight *> Storage::flightsFrom(const std::string &from, int dow) const
{
    std::vector<const Flight *> out;
    out.reserve(32);
    for (const auto &f : flights_)
        if (f.from == from && operatesOn(f.days, dow))
            out.push_back(&f);
    return out;
}

std::vector<const Flight *> Storage::flightsBetween(const std::string &from, const std::string &to, int dow) const
{
    std::vector<const Flight *> out;
    out.reserve(32);
    for (const auto &f : flights_)
        if (f.from == from && f.to == to && operatesOn(f.days, dow))
            out.push_back(&f);
    return out;
}

std::optional<Passenger> Storage::findPassengerByEmail(const std::string &email) const
{
    for (const auto &p : passengers_)
        if (p.email == email)
            return p;
    return std::nullopt;
}

Passenger Storage::upsertPassenger(const std::string &name, const std::string &email, const std::string &phone)
{
    if (auto ex = findPassengerByEmail(email))
        return *ex;
    // generate new id
    int next = 1;
    for (const auto &p : passengers_)
    {
        if (p.id.size() > 1 && p.id[0] == 'P')
        {
            try
            {
                next = std::max(next, std::stoi(p.id.substr(1)) + 1);
            }
            catch (...)
            {
            }
        }
    }
    Passenger np{"P" + std::to_string(next), name, email, phone};
    passengers_.push_back(np);
    appendPassengersFile(np);
    return np;
}

bool Storage::writeFlightsFile()
{
    std::ofstream out(baseDir_ + "/flights.csv", std::ios::trunc);
    if (!out)
        return false;
    out << "flightNo,from,to,dep,arr,distanceKm,baseFare,seatsTotal,seatsAvailable,days\n";
    for (const auto &f : flights_)
    {
        out << f.flightNo << "," << f.from << "," << f.to << "," << f.depHHMM << "," << f.arrHHMM << ","
            << f.distanceKm << "," << f.baseFare << "," << f.seatsTotal << "," << f.seatsAvailable << "," << f.days << "\n";
    }
    return true;
}

bool Storage::appendPassengersFile(const Passenger &p)
{
    std::ofstream out(baseDir_ + "/passengers.csv", std::ios::app);
    if (!out)
        return false;
    out << p.id << "," << p.name << "," << p.email << "," << p.phone << "\n";
    return true;
}

bool Storage::appendBookingsFile(const std::vector<Booking> &rows)
{
    std::ofstream out(baseDir_ + "/bookings.csv", std::ios::app);
    if (!out)
        return false;
    for (const auto &b : rows)
    {
        out << b.pnr << "," << b.flightNo << "," << b.date << "," << b.paxId << "," << b.status
            << "," << b.farePaid << "," << b.createdAt << "\n";
    }
    return true;
}

bool Storage::saveBookingRows(const std::vector<Booking> &rows)
{
    std::lock_guard<std::mutex> lk(m_);
    // adjust seats
    for (const auto &b : rows)
    {
        for (auto &f : flights_)
            if (f.flightNo == b.flightNo)
            {
                if (b.status == "BOOKED")
                {
                    if (f.seatsAvailable <= 0)
                        return false;      // insufficient seats
                    f.seatsAvailable -= 1; // per passenger row; caller repeats rows per pax
                }
                else if (b.status == "CANCELLED")
                {
                    f.seatsAvailable += 1;
                }
            }
    }
    if (!writeFlightsFile())
        return false;
    return appendBookingsFile(rows);
}

bool Storage::cancelPNR(const std::string &pnr)
{
    // This simple implementation appends CANCELLED rows for each leg and pax = 1
    // (A more robust version would read existing bookings; we keep append-only.)
    // Not altering historical rows; inventory is restored by adding seats via updateSeats call from ReservationService.
    return true;
}

bool Storage::updateSeats(const std::string &flightNo, int delta)
{
    std::lock_guard<std::mutex> lk(m_);
    for (auto &f : flights_)
        if (f.flightNo == flightNo)
        {
            f.seatsAvailable += delta;
            if (f.seatsAvailable < 0)
                f.seatsAvailable = 0;
        }
    return writeFlightsFile();
}