#include "ftpthread.h"


FtpThread::FtpThread()
{
    ftp = new QFtp(this);

    connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(listeOlustur(QUrlInfo)));
    connect(ftp, SIGNAL(done(bool)),this, SLOT(doneSlot(bool)));
}

/*
 * ftp işlemi bitince done(bool) sinayli geliyor.
 * done(bool) sinyali doneSlot(bool) slotuna bagli.
 * doneSlot(bool) slotu dosyaListesiOlusturuldu(QStringList)
 * sinyalini veriyor
 */
void FtpThread::doneSlot(bool d)
{
    emit dosyaListesiOlusturuldu(sl);
}

/*
 * ftp->list() metodu her klasor için listInfo(QUrlInfo)
 * signalini veriyor. listInfo(QUrlInfo) sinyali listeOlustur(QUrlInfo)
 * slotuna baglı. klasor icindeki dosyalardan bir liste hazırlıyor
 */
void FtpThread::listeOlustur(const QUrlInfo &urlInfo)
{
    if(urlInfo.isFile())
    {
        sl.append(urlInfo.name());
    }
}

/*
 * threadin baslama noktasi
 */
void FtpThread::run()
{
    //baglanilacak ftpnin ozellikleri
    QUrl url;
    url.setHost(HOST);
    url.setUserName(USERNAME);
    url.setPassword(PASSWORD);

    ftp->connectToHost(url.host(), url.port(21));
    ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
    ftp->cd(KLASOR_ARSIV);
    ftp->list();

    ftp->close();
}

FtpThread::~FtpThread()
{
    ftp->close();
}
