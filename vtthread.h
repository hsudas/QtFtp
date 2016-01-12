#ifndef VTTHREAD_H
#define VTTHREAD_H

#include <QtCore>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>

#include "global.h"

//sql sorgusu için sınıf
class SqlSorgu
{
public:
    QString vendorName;
    QString documentType;
    QString createDate;
    QString saveDate;
    QString invoiceDate;
    QString amount;
    QString invoiceNumber;
    QString filePath;
};

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
    //void faturaTuruListesiOlustu(QStringList);
    void islemBitti();
    void vtKayitAlindi(SqlSorgu);

public slots:
    void dosyaKaydet(SqlSorgu);
};


#endif // VTTHREAD_H
