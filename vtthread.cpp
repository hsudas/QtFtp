#include "vtthread.h"

VtThread::VtThread()
{

}

/*
 * threadin baslama noktasi vt den fatura turlerini alir ve
 * faturaTuruListesiOlustu(QStringList) sinyalini verir
 */
void VtThread::run()
{
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName(VT_ISIM);
    db.setUserName(VT_USERNAME);
    db.setPassword(VT_PASSWORD);
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

VtThread::~VtThread()
{
    db.close();
}
