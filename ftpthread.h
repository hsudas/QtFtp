#ifndef FTPTHREAD_H
#define FTPTHREAD_H

#include <QtCore>
#include <QFtp>
#include <QUrl>
#include "config.h"

class FtpThread : public QThread
{
    Q_OBJECT

public:
    FtpThread();
    void run();

    QFtp *ftp;
    QStringList sl;

    ~FtpThread();

signals:
    void dosyaListesiOlusturuldu(QStringList);

public slots:
    void doneSlot(bool);
    void listeOlustur(const QUrlInfo &urlInfo);
};

#endif // FTPTHREAD_H
