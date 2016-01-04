#include "vtthread.h"

VtThread::VtThread()
{
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName(VT_ISIM);
    db.setUserName(VT_USERNAME);
    db.setPassword(VT_PASSWORD);
}

/*
 * threadin baslama noktasi vt den fatura turlerini alir ve
 * faturaTuruListesiOlustu(QStringList) sinyalini verir
 */
void VtThread::run()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.exec("SELECT TUR FROM ROYAL.ROYAL.FATURA;");

        listeFaturaTuru.clear();
        while (query.next())
        {
            QString name = query.value(0).toString();
            listeFaturaTuru.append(name);
        }

        db.close();

        emit faturaTuruListesiOlustu(listeFaturaTuru);
    }
}

void VtThread::dosyaKaydet(QString tarih, QString faturaTuru, QString isim)
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        bool b = query.exec(QString("INSERT INTO YENI_FATURA (ISIM, TUR, TARIH) VALUES('%1','%2','%3');").arg(isim).arg(faturaTuru).arg(tarih));

        if(!b)
        {
            qDebug() << "vt hatasi 1"<<db.lastError();
        }

        db.close();
    }

    emit islemBitti();
}

VtThread::~VtThread()
{
    db.close();
}
