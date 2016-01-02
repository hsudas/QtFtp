#ifndef TAKVIM_H
#define TAKVIM_H

#include <QCalendarWidget>

class Takvim : public QCalendarWidget
{
    Q_OBJECT

public:
    Takvim(QDate date);
    ~Takvim();
    void closeEvent (QCloseEvent *event);

public slots:
    void secimYapildi(QDate date);

signals:
    void takvimKapandi(QDate date);
};

#endif // TAKVIM_H
