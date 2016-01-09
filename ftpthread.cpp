//#include "ftpthread.h"

//FtpThread::FtpThread()
//{
//    ftp = new QFtp(this);

//    connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(listeOlustur(QUrlInfo)));
//    connect(ftp, SIGNAL(done(bool)),this, SLOT(doneSlot(bool)));

//    url.setHost(FTP_HOST);
//    url.setUserName(FTP_USERNAME);
//    url.setPassword(FTP_PASSWORD);
//}

///*
// * dosyaKaydet(QString, QString) sinyali dosyaKaydet(QString, QString) slotunu cagiriyor
// * dosyaKaydet(QString, QString) slotu listWidget ta secilen elemani txtIsim alanina yazilan
// * isimle arsiv klasorune kaydediyor
// */
//void FtpThread::dosyaKaydet(QString dosyaIsmi, QString yeniIsim)
//{
//    if(ftp->state() == QFtp::Unconnected)
//    {
//        ftp->connectToHost(url.host(), url.port(21));
//        ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
//        ftp->cd(FTP_KLASOR_TARAYICI);
//    }

//    QString isim = QString("/%1/%2").arg(FTP_KLASOR_ARSIV).arg(yeniIsim);
//    ftp->rename(dosyaIsmi, isim);

//    sl.clear();
//    ftp->list();

//    ftp->close();

//    emit islemBitti();
//}

///*
// * ftp işlemi bitince done(bool) sinayli geliyor.
// * done(bool) sinyali doneSlot(bool) slotuna bagli.
// * doneSlot(bool) slotu dosyaListesiOlusturuldu(QStringList)
// * sinyalini veriyor
// */
//void FtpThread::doneSlot(bool d)
//{
//    emit dosyaListesiOlusturuldu(sl);
//}

///*
// * ftp->list() metodu her klasor için listInfo(QUrlInfo)
// * signalini veriyor. listInfo(QUrlInfo) sinyali listeOlustur(QUrlInfo)
// * slotuna baglı. klasor icindeki dosyalardan bir liste hazırlıyor
// */
//void FtpThread::listeOlustur(const QUrlInfo &urlInfo)
//{
//    if(urlInfo.isFile())
//    {
//        sl.append(urlInfo.name());
//    }
//}

///*
// * threadin baslama noktasi
// */
//void FtpThread::run()
//{
//    ftp->connectToHost(url.host(), url.port(21));
//    ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
//    ftp->cd(FTP_KLASOR_TARAYICI);
//    ftp->list();

//    ftp->close();
//}

//FtpThread::~FtpThread()
//{
//    ftp->close();
//}
