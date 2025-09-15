#include "ui/Windows/SearchPanel.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QDate>

SearchPanel::SearchPanel(QWidget *parent) : QWidget(parent)
{
    cbFrom_ = new QComboBox(this);
    cbTo_ = new QComboBox(this);
    date_ = new QDateEdit(QDate::currentDate(), this);
    date_->setCalendarPopup(true);
    directOnly_ = new QCheckBox("Direct only", this);
    pax_ = new QSpinBox(this);
    pax_->setRange(1, 9);
    pax_->setValue(1);
    btnSearch_ = new QPushButton("Search", this);

    auto form = new QFormLayout();
    form->addRow("From:", cbFrom_);
    form->addRow("To:", cbTo_);
    form->addRow("Date:", date_);
    form->addRow("Passengers:", pax_);
    form->addRow("", directOnly_);

    auto layout = new QHBoxLayout(this);
    layout->addLayout(form, 1);
    layout->addWidget(btnSearch_, 0);
    setLayout(layout);

    connect(btnSearch_, &QPushButton::clicked, this, [this]
            { emit searchRequested(cbFrom_->currentText(), cbTo_->currentText(), date_->date(), directOnly_->isChecked(), pax_->value()); });
}

void SearchPanel::setAirports(const QStringList &codes)
{
    cbFrom_->clear();
    cbTo_->clear();
    cbFrom_->addItems(codes);
    cbTo_->addItems(codes);
    if (codes.size() >= 2)
    {
        cbTo_->setCurrentIndex(1);
    }
}