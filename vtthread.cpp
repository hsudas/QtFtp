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
        tumKayitlariGetir(_ISLEM_TUM_KAYITLAR);
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
        tumKayitlariGetir(_ISLEM_BASLANGIC);
        break;

    case _ISLEM_EKLE_VENDOR_NAME:
        vendorNameEkle();
        break;

    case _ISLEM_EKLE_DOCUMENT_TYPE:
        documentTypeEkle();
        break;

    case _ISLEM_GUNCELLE:
        kayitGuncelle();
        break;

    case _ISLEM_VENDOR_NAME:
        vendorNameKaydet();
        break;

    case _ISLEM_DOCUMENT_TYPE:
        documentTypeKaydet();
        break;

    case _ISLEM_SIL:
        kayitSil();
        break;
    default:
        break;
    }
}

void VtThread::kayitSil()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec(QString("DELETE FROM FATURA2 WHERE ID=%1;").arg(kaydedilecekVeri));

         db.close();
         emit islemBitti(_ISLEM_SIL);
    }
}

/**
 * @brief VtThread::documentTypeKaydet : ayrinti2 penceresinde kaydet butonuna tiklayinca burasi calisir
 * documentType verilerini vt ye kaydeder
 */
void VtThread::documentTypeKaydet()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec("DELETE FROM FATURA_DOCUMENT_TYPE;");
        for(int i=0;i<kaydedilecekVeriler.size();i++)
        {
            bool b = query.exec(
                        QString("INSERT INTO FATURA_DOCUMENT_TYPE (DOCUMENT_TYPE) VALUES('%1');")
                        .arg(kaydedilecekVeriler.at(i)));
            if(!b)
            {
                qDebug() << "vt hatasi 1"<<db.lastError();
            }
        }

        db.close();

        emit documentTypeAlindi(kaydedilecekVeriler);
        emit islemBitti(_ISLEM_DOCUMENT_TYPE);
    }
}

/**
 * @brief VtThread::vendorNameKaydet : ayrinti2 penceresinde kaydet butonuna tiklayinca burasi calisir
 * vendorName verilerini vt ye kaydeder
 */
void VtThread::vendorNameKaydet()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        query.exec("DELETE FROM FATURA_VENDOR_NAME;");
        for(int i=0;i<kaydedilecekVeriler.size();i++)
        {
            bool b = query.exec(
                        QString("INSERT INTO FATURA_VENDOR_NAME (VENDOR_NAME) VALUES('%1');")
                        .arg(kaydedilecekVeriler.at(i)));
            if(!b)
            {
                qDebug() << "vt hatasi 1"<<db.lastError();
            }
        }

        db.close();

        emit vendorNameAlindi(kaydedilecekVeriler);
        emit islemBitti(_ISLEM_VENDOR_NAME);
    }
}

/**
 * @brief VtThread::kayitGuncelle : ayrinti ekraninda guncelle butonuna tiklanildigi zaman kayit gunceller
 */
void VtThread::kayitGuncelle()
{
    if (!db.open())
    {
        qDebug() << "vt hatasi"<<db.lastError();
    }
    else
    {
        QSqlQuery query;
        query.setForwardOnly(true);
        bool b = query.exec(QString("UPDATE FATURA2 SET DOCUMENT_TYPE='%1', VENDOR_NAME='%2', INVOICE_NUMBER='%3', TOTAL_AMOUNT='%4', FILE_PATH='%5', INVOICE_DATE='%6' WHERE ID='%7'")
                            .arg(sqlsrg.documentType)
                            .arg(sqlsrg.vendorName)
                            .arg(sqlsrg.invoiceNumber)
                            .arg(sqlsrg.amount)
                            .arg(sqlsrg.filePath)
                            .arg(sqlsrg.invoiceDate)
                            .arg(sqlsrg.id));

        if(!b)
        {
            qDebug() << "vt hatasi 1"<<db.lastError();
        }

        db.close();
    }

    emit islemBitti(_ISLEM_GUNCELLE);
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
        //listeDocumentType.append("");
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
        //listeVendorName.append("");
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

/**
 * @brief VtThread::tumKayitlariGetir : vt deki tum kayitlari getirir
 * @param islem : islem turune göre kullaniciya mesaj verilecek
 */
void VtThread::tumKayitlariGetir(int islem)
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
        query.exec("SELECT ID,DOCUMENT_TYPE, VENDOR_NAME, INVOICE_NUMBER, TOTAL_AMOUNT, FILE_PATH, SAVE_DATE, INVOICE_DATE, USER_NAME FROM FATURA2");
        while (query.next())
        {
            srg.id = query.value(0).toString();
            srg.documentType = query.value(1).toString();
            srg.vendorName = query.value(2).toString();
            srg.invoiceNumber = query.value(3).toString();
            srg.amount = query.value(4).toString();
            srg.filePath = query.value(5).toString();
            srg.saveDate = query.value(6).toString();
            srg.invoiceDate = query.value(7).toString();
            srg.userName = query.value(8).toString();

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

    switch (islem)
    {
    case _ISLEM_BASLANGIC:
        emit islemBitti(_ISLEM_BASLANGIC);
        break;
    case _ISLEM_TUM_KAYITLAR:
        emit islemBitti(_ISLEM_YENILE);
        break;
    case _ISLEM_KAYDET:
        emit islemBitti(_ISLEM_KAYDET);
        break;
    default:
        emit islemBitti(_ISLEM_YENILE);
        break;
    }
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

    tumKayitlariGetir(_ISLEM_KAYDET);
    //emit islemBitti(_ISLEM_KAYDET);
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
