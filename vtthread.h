#ifndef VTTHREAD_H
#define VTTHREAD_H

#include <QtCore>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>
#include <QInputDialog>

#include "global.h"

//sql sorgusu için sınıf
class SqlSorgu
{
public:
    QString id;
    QString vendorName;
    QString documentType;
    QString saveDate;
    QString invoiceDate;
    QString invoiceDate2;
    QString amount;
    QString invoiceNumber;
    QString filePath;
    QString userName;
};

class VtThread : public QThread
{
    Q_OBJECT

private:
    //QStringList listeFaturaTuru;
    QSqlDatabase db;
    SqlSorgu sqlsrg;
    QString kaydedilecekVeri;
    QStringList kaydedilecekVeriler;
    int ISLEM;

public:
    VtThread();
    ~VtThread();
    void run();
    void setISLEM(int i){ISLEM = i;}
    void setISLEM(QStringList veri, int i){kaydedilecekVeriler = veri;ISLEM = i;}
    void setISLEM(QString veri, int i){kaydedilecekVeri = veri;ISLEM = i;}
    void setISLEM(SqlSorgu srg, int i){sqlsrg = srg;ISLEM = i;}
    void documentTypeGetir();
    void vendorNameGetir();
    void vendorNameEkle();
    void documentTypeEkle();
    void vendorNameKaydet();
    void documentTypeKaydet();

signals:
    //void faturaTuruListesiOlustu(QStringList);
    void islemBitti(int);
    void vtKayitAlindi(SqlSorgu);
    void vendorNameAlindi(QStringList);
    void documentTypeAlindi(QStringList);

public slots:
    void dosyaKaydet();
    void tumKayitlariGetir(int islem);
    void aramaSonuclariniGetir();
    void kayitGuncelle();
};


#endif // VTTHREAD_H
