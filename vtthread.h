#ifndef VTTHREAD_H
#define VTTHREAD_H

#include <QtCore>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>

#include "config.h"

class VtThread : public QThread
{
    Q_OBJECT

private:
    QStringList listeFaturaTuru;

public:
    VtThread();
    void run();

signals:
    void faturaTuruListesiOlustu(QStringList);
};

#endif // VTTHREAD_H
