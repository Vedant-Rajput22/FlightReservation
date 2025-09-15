#include "core/SearchService.h"
#include "core/Graph.h"
#include "core/Util.h"
#include <algorithm>

SearchService::SearchService(Storage &s) : storage_(s) {}

static int durationMinutes(const Flight &f)
{
    int dep = hhmmToMinutes(f.depHHMM);
    int arr = hhmmToMinutes(f.arrHHMM);
    int dur = arr - dep;
    if (dur < 0)
        dur += 24 * 60;
    return dur;
}
static bool seatOK(const Flight *f, int pax) { return f->seatsAvailable >= pax; }

std::vector<Itinerary> SearchService::enumerateDirect(const SearchQuery &q, int dow) const
{
    std::vector<Itinerary> out;
    auto cands = storage_.flightsBetween(q.from, q.to, dow);
    for (auto *f : cands)
        if (seatOK(f, q.pax))
        {
            Itinerary it;
            it.legs.push_back({f});
            it.totalFare = f->baseFare * q.pax;
            it.totalMinutes = durationMinutes(*f);
            out.push_back(std::move(it));
        }
    return out;
}

static bool validLayover(const Flight *prev, const Flight *next)
{
    int arr = hhmmToMinutes(prev->arrHHMM);
    int dep = hhmmToMinutes(next->depHHMM);
    if (dep < arr)
        dep += 24 * 60;
    int lay = dep - arr;
    return lay >= 45 && lay <= (6 * 60);
}

std::vector<Itinerary> SearchService::enumerateUpTo2Stops(const SearchQuery &q, int dow) const
{
    std::vector<Itinerary> out;
    auto fromLegs = storage_.flightsFrom(q.from, dow);

    for (auto *f1 : fromLegs)
    {
        if (!seatOK(f1, q.pax))
            continue;
        auto midLegs = storage_.flightsFrom(f1->to, dow);
        for (auto *f2 : midLegs)
        {
            if (f2->to != q.to)
                continue;
            if (!seatOK(f2, q.pax))
                continue;
            if (!validLayover(f1, f2))
                continue;
            Itinerary it;
            it.legs = {{f1}, {f2}};
            it.totalFare = (f1->baseFare + f2->baseFare) * q.pax;
            it.totalMinutes = durationMinutes(*f1) + durationMinutes(*f2) +
                              (hhmmToMinutes(f2->depHHMM) - hhmmToMinutes(f1->arrHHMM) + 24 * 60) % (24 * 60);
            out.push_back(std::move(it));
        }
    }

    for (auto *f1 : fromLegs)
    {
        if (!seatOK(f1, q.pax))
            continue;
        auto mid1 = storage_.flightsFrom(f1->to, dow);
        for (auto *f2 : mid1)
        {
            if (!seatOK(f2, q.pax))
                continue;
            if (!validLayover(f1, f2))
                continue;
            auto mid2 = storage_.flightsFrom(f2->to, dow);
            for (auto *f3 : mid2)
            {
                if (f3->to != q.to)
                    continue;
                if (!seatOK(f3, q.pax))
                    continue;
                if (!validLayover(f2, f3))
                    continue;
                Itinerary it;
                it.legs = {{f1}, {f2}, {f3}};
                int lay1 = (hhmmToMinutes(f2->depHHMM) - hhmmToMinutes(f1->arrHHMM) + 24 * 60) % (24 * 60);
                int lay2 = (hhmmToMinutes(f3->depHHMM) - hhmmToMinutes(f2->arrHHMM) + 24 * 60) % (24 * 60);
                it.totalFare = (f1->baseFare + f2->baseFare + f3->baseFare) * q.pax;
                it.totalMinutes = durationMinutes(*f1) + durationMinutes(*f2) + durationMinutes(*f3) + lay1 + lay2;
                out.push_back(std::move(it));
            }
        }
    }

    return out;
}

std::vector<Itinerary> SearchService::search(const SearchQuery &q) const
{
    std::vector<Itinerary> res;
    int dow = dayOfWeek(q.date);

    auto direct = enumerateDirect(q, dow);
    res.insert(res.end(), direct.begin(), direct.end());

    if (!q.directOnly)
    {
        auto conns = enumerateUpTo2Stops(q, dow);
        res.insert(res.end(), conns.begin(), conns.end());
    }

    std::sort(res.begin(), res.end(), [](const Itinerary &a, const Itinerary &b)
              {
if (a.totalFare != b.totalFare) return a.totalFare < b.totalFare;
return a.totalMinutes < b.totalMinutes; });

    if (res.size() > 50)
        res.resize(50);
    return res;
}