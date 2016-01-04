#ifndef FTPTHREAD_H
#define FTPTHREAD_H

#include <QtCore>
#include <QFtp>
#include <QUrl>

#include "config.h"

class FtpThread : public QThread
{
    Q_OBJECT

private:
    QFtp *ftp;
    QStringList sl;
    QUrl url;

public:
    FtpThread();
    ~FtpThread();
    void run();

signals:
    void dosyaListesiOlusturuldu(QStringList);
    void islemBitti();

public slots:
    void doneSlot(bool);
    void listeOlustur(const QUrlInfo &urlInfo);
    void dosyaKaydet(QString dosyaIsmi, QString yeniIsim);
};

#endif // FTPTHREAD_H
