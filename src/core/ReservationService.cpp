#include "core/ReservationService.h"
#include "core/Util.h"

std::string ReservationService::book(const Itinerary &itin, const std::string &date, const std::string &name,
                                     const std::string &email, const std::string &phone, int pax, int totalFare)
{
    auto paxRec = storage_.upsertPassenger(name, email, phone);
    std::string pnr = PNRGenerator::generate();

    std::vector<Booking> rows;
    rows.reserve(itin.legs.size() * (size_t)pax);
    for (int i = 0; i < pax; i++)
    {
        for (auto &leg : itin.legs)
        {
            rows.push_back(Booking{pnr, leg.flight->flightNo, date, paxRec.id, "BOOKED", leg.flight->baseFare, nowIsoLocal()});
        }
    }

    if (!storage_.saveBookingRows(rows))
        return std::string();
    return pnr;
}

bool ReservationService::cancel(const std::string &pnr)
{
    return !pnr.empty();
}