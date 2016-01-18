#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>

#define TARIH_FORMAT        "yyyy-MM-dd"
#define VT_ISIM             "freetds-baglanti1"

//select işlem turleri
#define _ISLEM_TUM_KAYITLAR  0
#define _ISLEM_ARAMA_SONUCU  1

//vt işlem turleri
#define _ISLEM_KAYDET    0
#define _ISLEM_YENILE    1
#define _ISLEM_ARAMA     2

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
