#pragma once
#include <QWidget>
#include <QString>
#include <QDate>

class QComboBox;
class QDateEdit;
class QCheckBox;
class QSpinBox;
class QPushButton;

class SearchPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SearchPanel(QWidget *parent = nullptr);
    void setAirports(const QStringList &codes);

signals:
    void searchRequested(const QString &from, const QString &to, const QDate &date, bool directOnly, int pax);

private:
    QComboBox *cbFrom_{};
    QComboBox *cbTo_{};
    QDateEdit *date_{};
    QCheckBox *directOnly_{};
    QSpinBox *pax_{};
    QPushButton *btnSearch_{};
};