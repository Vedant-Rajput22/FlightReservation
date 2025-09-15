#pragma once
#include <QMainWindow>
#include <QString>
#include <memory>
#include "core/Storage.h"
#include "core/SearchService.h"
#include "core/ReservationService.h"

class QTableView;
class FlightTableModel;
class SearchPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &dataDir, QWidget *parent = nullptr);

private slots:
    void onSearchRequested(const QString &from, const QString &to, const QDate &date, bool directOnly, int pax);
    void onBookSelected();
    void onCancelPNR();

private:
    QString dataDir_;
    std::unique_ptr<Storage> storage_;
    std::unique_ptr<SearchService> searchSvc_;
    std::unique_ptr<ReservationService> resSvc_;

    SearchPanel *searchPanel_{};
    QTableView *table_{};
    FlightTableModel *model_{};
};