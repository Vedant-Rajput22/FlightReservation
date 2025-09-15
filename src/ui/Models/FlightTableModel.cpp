#include "ui/Models/FlightTableModel.h"
#include <QString>

QVariant FlightTableModel::data(const QModelIndex &idx, int role) const
{
    if (!idx.isValid() || role != Qt::DisplayRole)
        return {};
    const auto &it = items_.at((size_t)idx.row());
    switch (idx.column())
    {
    case 0:
    { // Route
        QStringList legs;
        for (const auto &l : it.legs)
            legs << QString::fromStdString(l.flight->from + "→" + l.flight->to);
        return legs.join(" ");
    }
    case 1:
    { // Dep-Arr
        const auto &f0 = it.legs.front().flight;
        const auto &fn = it.legs.back().flight;
        return QString::fromStdString(f0->depHHMM + " → " + fn->arrHHMM);
    }
    case 2:
        return QString::number((int)it.legs.size() - 1); // Stops
    case 3:
        return QString::number(it.totalMinutes) + " min"; // Duration
    case 4:
    { // Flights
        QStringList nos;
        for (auto &l : it.legs)
            nos << QString::fromStdString(l.flight->flightNo);
        return nos.join(", ");
    }
    case 5:
        return QString::number(it.totalFare); // Price
    }
    return {};
}

QVariant FlightTableModel::headerData(int section, Qt::Orientation o, int role) const
{
    if (o == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return "Route";
        case 1:
            return "Time";
        case 2:
            return "Stops";
        case 3:
            return "Duration";
        case 4:
            return "Flights";
        case 5:
            return "Fare";
        }
    }
    return {};
}