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
    SqlSorgu sqlsrg;
    int ISLEM;

public:
    VtThread();
    ~VtThread();
    void run();
    void setISLEM(int i){ISLEM = i;}

    static const int ISLEM_TUM_KAYITLAR=0;
    static const int ISLEM_ARAMA_SONUCU=1;

signals:
    //void faturaTuruListesiOlustu(QStringList);
    void islemBitti();
    void vtKayitAlindi(SqlSorgu);

public slots:
    void dosyaKaydet(SqlSorgu);
    void setSqlSorgu(SqlSorgu, int);
    void tumKayitlariGetir();
    void aramaSonuclariniGetir();
};


#endif // VTTHREAD_H
