#include "vtthread.h"

VtThread::VtThread()
{
#ifdef Q_OS_LINUX
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName(VT_ISIM);// /etc/odbc.ini dosyasinda tanimli baglanti
    db.setUserName(Global::config->CNF_VT_USERNAME);
    db.setPassword(Global::config->CNF_VT_PASSWORD);

#elif defined(Q_OS_WIN) || defined(Q_OS_WIN32)
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setUserName(Global::config->CNF_VT_USERNAME);
    db.setPassword(Global::config->CNF_VT_PASSWORD);
    db.setDatabaseName(QString("DRIVER={SQL SERVER};Server=%1;Database=%2;").arg(Global::config->CNF_VT_IP).arg(Global::config->CNF_VT_DATABASE_NAME));
#else

#endif
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

/*
 * kaydet tusuna basilinca dosyaKaydet(QString, QString, QString) slotu cagriliyor.
 * vt ye yeni kayit ekliyor.
 */
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
