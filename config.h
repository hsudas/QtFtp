#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>

#define TARIH_FORMAT        "yyyy-MM-dd"
#define VT_ISIM             "freetds-baglanti1"

//select işlem turleri
#define _ISLEM_TUM_KAYITLAR  0
#define _ISLEM_ARAMA_SONUCU  1

//vt işlem turleri
#define _ISLEM_KAYDET       3
#define _ISLEM_YENILE       4
#define _ISLEM_ARAMA        5
#define _ISLEM_BASLANGIC    6

//insert islemleri
#define _ISLEM_EKLE_VENDOR_NAME     7
#define _ISLEM_EKLE_DOCUMENT_TYPE   8

//tableWidget sutunlari
#define SUTUN_ID                0
#define SUTUN_DOCUMENT_TYPE     1
#define SUTUN_VENDOR_NAME       2
#define SUTUN_INVOICE_NUMBER    3
#define SUTUN_TOTAL_AMOUNT      4
#define SUTUN_FILE_PATH         5
#define SUTUN_SAVE_DATE         6
#define SUTUN_INVOICE_DATE      7
#define SUTUN_USER_NAME         8

class Config
{
public:

    Config();
    void ayarlariOku();
    void ayarla(QStringList);

    QString CNF_VT_USERNAME;
    QString CNF_VT_PASSWORD;
    QString CNF_VT_IP;
    QString CNF_VT_DATABASE_NAME;
    QString CNF_VT_WIN_DRIVER;
    QString CNF_KLASOR_AGACI_ROOT;

    bool dosyaVar;
    QString ayarDosyasiYolu;
};

#endif // CONFIG_H
