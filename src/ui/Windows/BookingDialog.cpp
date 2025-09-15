#include "ui/Windows/BookingDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

static QString summarize(const Itinerary &it, int pax, const QString &date)
{
    QStringList legs;
    for (auto &l : it.legs)
        legs << QString::fromStdString(l.flight->flightNo + " " + l.flight->from + "→" + l.flight->to +
                                       " (" + l.flight->depHHMM + "-" + l.flight->arrHHMM + ")");
    return QString("Date: %1\nPassengers: %2\n%3\nFare: %4")
        .arg(date)
        .arg(pax)
        .arg(legs.join("\n"))
        .arg(it.totalFare);
}

BookingDialog::BookingDialog(const Itinerary &it, int pax, const QString &date, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Confirm Booking");
    summary_ = new QLabel(summarize(it, pax, date), this);
    name_ = new QLineEdit(this);
    email_ = new QLineEdit(this);
    phone_ = new QLineEdit(this);
    btnOk_ = new QPushButton("Confirm", this);
    btnCancel_ = new QPushButton("Cancel", this);

    auto form = new QVBoxLayout();
    form->addWidget(summary_);

    auto rowN = new QHBoxLayout();
    rowN->addWidget(new QLabel("Name:"));
    rowN->addWidget(name_);
    form->addLayout(rowN);
    auto rowE = new QHBoxLayout();
    rowE->addWidget(new QLabel("Email:"));
    rowE->addWidget(email_);
    form->addLayout(rowE);
    auto rowP = new QHBoxLayout();
    rowP->addWidget(new QLabel("Phone:"));
    rowP->addWidget(phone_);
    form->addLayout(rowP);

    auto buttons = new QHBoxLayout();
    buttons->addStretch();
    buttons->addWidget(btnOk_);
    buttons->addWidget(btnCancel_);
    form->addLayout(buttons);

    setLayout(form);

    connect(btnOk_, &QPushButton::clicked, this, [this]
            { accept(); });
    connect(btnCancel_, &QPushButton::clicked, this, [this]
            { reject(); });
}

QString BookingDialog::name() const { return name_->text(); }
QString BookingDialog::email() const { return email_->text(); }
QString BookingDialog::phone() const { return phone_->text(); }