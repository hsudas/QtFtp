#ifndef TAKVIM_H
#define TAKVIM_H

#include <QCalendarWidget>
#include <QPushButton>

class Takvim : public QCalendarWidget
{
    Q_OBJECT

public:
    Takvim(QDate date, QPushButton *cagiranBtn);
    ~Takvim();
    void closeEvent (QCloseEvent *);

private:
    QPushButton *cagiranBtn;

public slots:
    void secimYapildi(QDate date);

signals:
    void takvimKapandi(QDate date, QPushButton *btn);
};

#endif // TAKVIM_H
