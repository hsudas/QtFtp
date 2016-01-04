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
    QSqlDatabase db;

public:
    VtThread();
    ~VtThread();
    void run();

signals:
    void faturaTuruListesiOlustu(QStringList);
    void islemBitti();

public slots:
    void dosyaKaydet(QString tarih, QString faturaTuru, QString isim);
};

#endif // VTTHREAD_H
