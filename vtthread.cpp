#include "vtthread.h"

VtThread::VtThread()
{
    qRegisterMetaType<SqlSorgu>("SqlSorgu");

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
        SqlSorgu srg;
        QSqlQuery query;
        query.exec("SELECT DOCUMENT_TYPE, VENDOR_NAME, INVOICE_NUMBER, TOTAL_AMOUNT, FILE_PATH, SAVE_DATE, INVOICE_DATE FROM FATURA2");
        while (query.next())
        {
            srg.documentType=query.value(0).toString();
            srg.vendorName=query.value(1).toString();
            srg.invoiceNumber=query.value(2).toString();
            srg.amount=query.value(3).toString();
            srg.filePath=query.value(4).toString();
            srg.saveDate=query.value(5).toString();
            srg.saveDate=query.value(5).toString();

            emit vtKayitAlindi(srg);
        }

        /*
        QSqlQuery query;
        query.exec("SELECT TUR FROM ROYAL.ROYAL.FATURA;");

        listeFaturaTuru.clear();
        while (query.next())
        {
            QString name = query.value(0).toString();
            listeFaturaTuru.append(name);
        }

        db.close();

        emit faturaTuruListesiOlustu(listeFaturaTuru);*/

    }
}

/*
 * kaydet tusuna basilinca dosyaKaydet(SqlSorgu) slotu cagriliyor.
 * vt ye yeni kayit ekliyor.
 */
void VtThread::dosyaKaydet(SqlSorgu srg)
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        bool b = query.exec(
                    QString("INSERT INTO FATURA2 (DOCUMENT_TYPE, VENDOR_NAME, INVOICE_NUMBER, TOTAL_AMOUNT, FILE_PATH, INVOICE_DATE, SAVE_DATE) VALUES('%1','%2','%3','%4','%5','%6','%7');")
                    .arg(srg.documentType)
                    .arg(srg.vendorName)
                    .arg(srg.invoiceNumber)
                    .arg(srg.amount)
                    .arg(srg.filePath)
                    .arg(srg.invoiceDate)
                    .arg(srg.saveDate));
        /*
        bool b = query.exec(QString("INSERT INTO YENI_FATURA (ISIM, TUR, TARIH) VALUES('%1','%2','%3');").arg(isim).arg(faturaTuru).arg(tarih));
        */
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
