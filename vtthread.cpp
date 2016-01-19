#include "vtthread.h"

VtThread::VtThread()
{
    qRegisterMetaType<SqlSorgu>("SqlSorgu");
    //ISLEM = _ISLEM_TUM_KAYITLAR;

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
 * threadin baslama noktasi ISLEM e gore vt islemi yapar
 */
void VtThread::run()
{
    switch (ISLEM)
    {
    case _ISLEM_TUM_KAYITLAR:
        tumKayitlariGetir();
        break;

    case _ISLEM_ARAMA_SONUCU:
        aramaSonuclariniGetir();
        break;

    case _ISLEM_KAYDET:
        dosyaKaydet();
        break;

    case _ISLEM_BASLANGIC:
        documentTypeGetir();
        vendorNameGetir();
        tumKayitlariGetir();
        break;

    case _ISLEM_EKLE_VENDOR_NAME:
        vendorNameEkle();
        break;

    case _ISLEM_EKLE_DOCUMENT_TYPE:
        documentTypeEkle();
        break;

    default:
        break;
    }
}

/**
 * @brief VtThread::documentTypeEkle
 * ana thread den gelen veriyi vt ye kaydeder
 */
void VtThread::documentTypeEkle()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        bool b = query.exec(
                    QString("INSERT INTO FATURA_DOCUMENT_TYPE (DOCUMENT_TYPE) VALUES('%1');")
                    .arg(kaydedilecekVeri));
        if(!b)
        {
            qDebug() << "vt hatasi 1"<<db.lastError();
        }

        db.close();

        documentTypeGetir();
        emit islemBitti(_ISLEM_EKLE_DOCUMENT_TYPE);
    }
}

/**
 * @brief VtThread::vendorNameEkle
 * ana thread den gelen veriyi vt ye kaydeder
 */
void VtThread::vendorNameEkle()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        bool b = query.exec(
                    QString("INSERT INTO FATURA_VENDOR_NAME (VENDOR_NAME) VALUES('%1');")
                    .arg(kaydedilecekVeri));
        if(!b)
        {
            qDebug() << "vt hatasi 1"<<db.lastError();
        }

        db.close();

        vendorNameGetir();
        emit islemBitti(_ISLEM_EKLE_VENDOR_NAME);
    }
}

/**
 * @brief VtThread::documentTypeGetir
 * vt document_type tablosundan değerleri getirir
 */
void VtThread::documentTypeGetir()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QStringList listeDocumentType;
        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec("SELECT DOCUMENT_TYPE FROM FATURA_DOCUMENT_TYPE");
        while (query.next())
        {
            listeDocumentType.append(query.value(0).toString());
        }

        db.close();

        emit documentTypeAlindi(listeDocumentType);
    }
}

/**
 * @brief VtThread::vendorNameGetir
 * vt vendor_name tablosundan değerleri getirir
 */
void VtThread::vendorNameGetir()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QStringList listeVendorName;
        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec("SELECT VENDOR_NAME FROM FATURA_VENDOR_NAME");
        while (query.next())
        {
            listeVendorName.append(query.value(0).toString());
        }

        db.close();

        emit vendorNameAlindi(listeVendorName);
    }
}

/*
 * vt deki tum kayitlari getirir
 */
void VtThread::tumKayitlariGetir()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        SqlSorgu srg;
        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec("SELECT DOCUMENT_TYPE, VENDOR_NAME, INVOICE_NUMBER, TOTAL_AMOUNT, FILE_PATH, SAVE_DATE, INVOICE_DATE, USER_NAME FROM FATURA2");
        while (query.next())
        {
            srg.documentType=query.value(0).toString();
            srg.vendorName=query.value(1).toString();
            srg.invoiceNumber=query.value(2).toString();
            srg.amount=query.value(3).toString();
            srg.filePath=query.value(4).toString();
            srg.saveDate=query.value(5).toString();
            srg.invoiceDate=query.value(6).toString();
            srg.userName=query.value(7).toString();

            emit vtKayitAlindi(srg);
        }

        db.close();

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

    emit islemBitti(_ISLEM_YENILE);
}

/*
 * kaydet tusuna basilinca dosyaKaydet() slotu cagriliyor.
 * vt ye yeni kayit ekliyor.
 */
void VtThread::dosyaKaydet()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        bool b = query.exec(
                    QString("INSERT INTO FATURA2 (DOCUMENT_TYPE, VENDOR_NAME, INVOICE_NUMBER, TOTAL_AMOUNT, FILE_PATH, INVOICE_DATE, SAVE_DATE, USER_NAME) VALUES('%1','%2','%3','%4','%5','%6','%7','%8');")
                    .arg(sqlsrg.documentType)
                    .arg(sqlsrg.vendorName)
                    .arg(sqlsrg.invoiceNumber)
                    .arg(sqlsrg.amount)
                    .arg(sqlsrg.filePath)
                    .arg(sqlsrg.invoiceDate)
                    .arg(sqlsrg.saveDate)
                    .arg(sqlsrg.userName));
        /*
        bool b = query.exec(QString("INSERT INTO YENI_FATURA (ISIM, TUR, TARIH) VALUES('%1','%2','%3');").arg(isim).arg(faturaTuru).arg(tarih));
        */
        if(!b)
        {
            qDebug() << "vt hatasi 1"<<db.lastError();
        }

        db.close();
    }

    tumKayitlariGetir();
    emit islemBitti(_ISLEM_KAYDET);
}

/*
 * vt den arama sonuclari getirir
 */
void VtThread::aramaSonuclariniGetir()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QString sorgu = QString("SELECT DOCUMENT_TYPE, VENDOR_NAME, INVOICE_NUMBER, TOTAL_AMOUNT, FILE_PATH, SAVE_DATE, INVOICE_DATE, USER_NAME FROM FATURA2 WHERE INVOICE_DATE >= '%1' AND INVOICE_DATE <= '%2'").arg(sqlsrg.invoiceDate).arg(sqlsrg.invoiceDate2);
        if(!sqlsrg.documentType.isEmpty())
        {
            sorgu.append(QString(" AND DOCUMENT_TYPE='%1'").arg(sqlsrg.documentType));
        }
        if(!sqlsrg.amount.isEmpty())
        {
            sorgu.append(QString(" AND TOTAL_AMOUNT='%1'").arg(sqlsrg.amount));
        }
        if(!sqlsrg.filePath.isEmpty())
        {
            sorgu.append(QString(" AND FILE_PATH='%1'").arg(sqlsrg.filePath));
        }
        if(!sqlsrg.invoiceNumber.isEmpty())
        {
            sorgu.append(QString(" AND INVOICE_NUMBER='%1'").arg(sqlsrg.invoiceNumber));
        }
        if(!sqlsrg.vendorName.isEmpty())
        {
            sorgu.append(QString(" AND VENDOR_NAME='%1'").arg(sqlsrg.vendorName));
        }
        /*
        if(!sqlsrg.userName.isEmpty())
        {
            sorgu.append(QString(" AND USER_NAME='%1'").arg(sqlsrg.userName));
        }
        */

        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec(sorgu);
        while (query.next())
        {
            sqlsrg.documentType=query.value(0).toString();
            sqlsrg.vendorName=query.value(1).toString();
            sqlsrg.invoiceNumber=query.value(2).toString();
            sqlsrg.amount=query.value(3).toString();
            sqlsrg.filePath=query.value(4).toString();
            sqlsrg.saveDate=query.value(5).toString();
            sqlsrg.invoiceDate=query.value(6).toString();
            sqlsrg.userName=query.value(7).toString();

            emit vtKayitAlindi(sqlsrg);
        }

        db.close();
    }

    emit islemBitti(_ISLEM_ARAMA);
}

VtThread::~VtThread()
{
    db.close();
}
