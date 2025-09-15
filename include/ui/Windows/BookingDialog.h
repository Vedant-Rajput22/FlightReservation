#pragma once
#include <QDialog>
#include <QString>
#include "core/DijkstraRouter.h"

class QLineEdit;
class QLabel;
class QPushButton;

class BookingDialog : public QDialog
{
    Q_OBJECT
public:
    BookingDialog(const Itinerary &itin, int pax, const QString &date, QWidget *parent = nullptr);

    QString name() const;
    QString email() const;
    QString phone() const;

private:
    QLabel *summary_{};
    QLineEdit *name_{};
    QLineEdit *email_{};
    QLineEdit *phone_{};
    QPushButton *btnOk_{};
    QPushButton *btnCancel_{};
};