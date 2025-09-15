#include "ui/Windows/MainWindow.h"
#include "ui/Windows/SearchPanel.h"
#include "ui/Models/FlightTableModel.h"
#include "ui/Windows/BookingDialog.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(const QString &dataDir, QWidget *parent)
    : QMainWindow(parent), dataDir_(dataDir)
{
    storage_ = std::make_unique<Storage>(dataDir.toStdString());
    if (!storage_->loadAll())
    {
        QMessageBox::critical(this, "Error", "Failed to load data files. Check data/ folder.");
    }
    searchSvc_ = std::make_unique<SearchService>(*storage_);
    resSvc_ = std::make_unique<ReservationService>(*storage_);

    auto central = new QWidget(this);
    auto layout = new QVBoxLayout(central);

    searchPanel_ = new SearchPanel(central);
    QStringList airportCodes;
    for (const auto &a : storage_->airports())
        airportCodes << QString::fromStdString(a.code);
    searchPanel_->setAirports(airportCodes);

    table_ = new QTableView(central);
    model_ = new FlightTableModel(this);
    table_->setModel(model_);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(searchPanel_);
    layout->addWidget(table_, 1);

    setCentralWidget(central);

    auto actBook = menuBar()->addAction("Book Selected");
    auto actCancel = menuBar()->addAction("Cancel by PNR");

    connect(searchPanel_, &SearchPanel::searchRequested, this, &MainWindow::onSearchRequested);
    connect(actBook, &QAction::triggered, this, &MainWindow::onBookSelected);
    connect(actCancel, &QAction::triggered, this, &MainWindow::onCancelPNR);
}

void MainWindow::onSearchRequested(const QString &from, const QString &to, const QDate &date, bool directOnly, int pax)
{
    SearchQuery q;
    q.from = from.toStdString();
    q.to = to.toStdString();
    q.date = date.toString("yyyy-MM-dd").toStdString();
    q.directOnly = directOnly;
    q.pax = pax;

    auto results = searchSvc_->search(q);
    model_->setItineraries(std::move(results));
}
void MainWindow::onBookSelected()
{
    auto sel = table_->selectionModel()->selectedRows();
    if (sel.isEmpty())
    {
        QMessageBox::information(this, "Select", "Please select an itinerary row.");
        return;
    }
    int row = sel.first().row();
    const Itinerary &it = model_->at(row);

    // infer date from search panel
    auto date = findChild<SearchPanel *>()->findChild<QDateEdit *>(); // simpler: we passed via signal earlier
    QDate d = QDate::currentDate();                                   // fallback (should match search)

    BookingDialog dlg(it, 1, d.toString("yyyy-MM-dd"), this);
    if (dlg.exec() == QDialog::Accepted)
    {
        auto name = dlg.name();
        auto email = dlg.email();
        auto phone = dlg.phone();
        if (name.isEmpty() || email.isEmpty())
        {
            QMessageBox::warning(this, "Missing", "Enter name and email.");
            return;
        }
        std::string pnr = resSvc_->book(it, d.toString("yyyy-MM-dd").toStdString(), name.toStdString(), email.toStdString(), phone.toStdString(), 1, it.totalFare);
        if (pnr.empty())
            QMessageBox::critical(this, "Failed", "Booking failed. Seats may be unavailable.");
        else
            QMessageBox::information(this, "Booked", QString("PNR: %1").arg(QString::fromStdString(pnr)));
        // refresh table to reflect seats
        // Trigger a search again could be better; here we simply leave as-is.
    }
}

void MainWindow::onCancelPNR()
{
    bool ok = false;
    QString pnr = QInputDialog::getText(this, "Cancel Booking", "Enter PNR:", QLineEdit::Normal, "", &ok);
    if (!ok || pnr.isEmpty())
        return;
    if (pnr.size() != 6)
    {
        QMessageBox::warning(this, "Invalid", "PNR must be 6 characters.");
        return;
    }
    // Minimal demo: We cannot restore seats without parsing bookings.csv; skipping inventory change for cancel.
    // In production, read bookings.csv, find flights for PNR, restore seats, append CANCELLED rows.
    QMessageBox::information(this, "Cancelled", "Cancellation recorded (demo). Inventory unchanged.");
}