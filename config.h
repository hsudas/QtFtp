#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>

#define TARIH_FORMAT        "yyyy-MM-dd"
#define VT_ISIM             "freetds-baglanti1"

//select işlem turleri
#define _ISLEM_TUM_KAYITLAR  0
#define _ISLEM_ARAMA_SONUCU  1

//vt işlem turleri
#define _ISLEM_KAYDET               3
#define _ISLEM_YENILE               4
#define _ISLEM_ARAMA                5
#define _ISLEM_BASLANGIC            6
#define _ISLEM_GUNCELLE             7
#define _ISLEM_VENDOR_NAME          8
#define _ISLEM_DOCUMENT_TYPE        9
#define _ISLEM_EKLE_VENDOR_NAME     10
#define _ISLEM_EKLE_DOCUMENT_TYPE   11
#define _ISLEM_SIL                  12

//tableWidget sutunlari
#define SUTUN_AYRINTI           0
#define SUTUN_ID                1
#define SUTUN_DOCUMENT_TYPE     2
#define SUTUN_VENDOR_NAME       3
#define SUTUN_INVOICE_NUMBER    4
#define SUTUN_TOTAL_AMOUNT      5
#define SUTUN_FILE_PATH         6
#define SUTUN_SAVE_DATE         7
#define SUTUN_INVOICE_DATE      8
#define SUTUN_USER_NAME         9
#define SUTUN_TOPLAM            10

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
    QString CNF_KLASOR_ARCHIVE;
    QString CNF_FILE_NAME_FORMAT;
    QString CNF_SHOW_FILE_TREE;

    bool dosyaVar;
    QString ayarDosyasiYolu;
};

#endif // CONFIG_H
