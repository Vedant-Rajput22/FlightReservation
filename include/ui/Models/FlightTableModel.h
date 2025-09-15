#pragma once
#include <QAbstractTableModel>
#include <QString>
#include <vector>
#include "core/DijkstraRouter.h"

class FlightTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FlightTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex & = QModelIndex()) const override { return (int)items_.size(); }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return 6; }

    QVariant data(const QModelIndex &idx, int role) const override;
    QVariant headerData(int section, Qt::Orientation, int role) const override;

    void setItineraries(std::vector<Itinerary> v)
    {
        beginResetModel();
        items_ = std::move(v);
        endResetModel();
    }
    const Itinerary &at(int r) const { return items_.at((size_t)r); }

private:
    std::vector<Itinerary> items_;
};