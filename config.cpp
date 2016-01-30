#include "config.h"

Config::Config()
{
    ayarlariOku();
}

/**
 * @brief Config::ayarlariOku : uygulama basladiginda ayar dosyasini okur ve ayarlari alir
 */
void Config::ayarlariOku()
{
    ayarDosyasiYolu = QDir::currentPath()+QDir::separator()+"config.txt";
    QFile file(ayarDosyasiYolu);
    if(!file.open(QIODevice::ReadOnly))
    {
        dosyaVar = false;
    }
    else
    {
        dosyaVar = true;
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split("=");
            ayarla(fields);
        }

        file.close();
    }
}

/**
 * @brief Config::ayarla : config dosyasından alinan satirdaki deger değişkene ataniyor
 * @param sl : config dosyasinin satiri
 */
void Config::ayarla(QStringList sl)
{
    if(sl.at(0) == "VT_USERNAME")
    {
        CNF_VT_USERNAME = sl.at(1);
    }
    else if(sl.at(0) == "VT_PASSWORD")
    {
        CNF_VT_PASSWORD = sl.at(1);
    }
    else if(sl.at(0) == "VT_IP")
    {
        CNF_VT_IP = sl.at(1);
    }
    else if(sl.at(0) == "VT_DATABASE_NAME")
    {
        CNF_VT_DATABASE_NAME = sl.at(1);
    }
    else if(sl.at(0) == "SHARED_FOLDER")
    {
        CNF_KLASOR_AGACI_ROOT = sl.at(1);
    }
    else if(sl.at(0) == "ARCHIVE_FOLDER")
    {
        CNF_KLASOR_ARCHIVE = sl.at(1);
    }
    else if(sl.at(0) == "FILE_NAME_FORMAT")
    {
        CNF_FILE_NAME_FORMAT = sl.at(1);
    }
    else if(sl.at(0) == "SHOW_FILE_TREE")
    {
        CNF_SHOW_FILE_TREE = sl.at(1);
    }
}
