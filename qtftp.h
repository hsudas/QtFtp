#ifndef QTFTP_H
#define QTFTP_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QInputDialog>
#include <QFileDialog>

//#include "ftpthread.h"
#include "vtthread.h"
#include "takvim.h"
#include "global.h"
#include "ayrinti.h"
#include "ayrinti2.h"

namespace Ui
{
class QtFtp;
}

class QtFtp : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtFtp(QWidget *parent = 0);
    ~QtFtp();
    void vtThreadCalistir();
    //void islemBitti();
    void klasorAgaciOlustur();
    //void ftpThreadCalistir();
    void tusEtkisiz(bool);
    bool ayarlariKontrolEt();
    void alanlariTemizle();
    void vtYenile();

private:
    Ui::QtFtp *ui;
    VtThread *vtThread;
    //bool vtIslemiBitti;
    QFileSystemModel *dirModel;
    //FtpThread *ftpThread;
    //bool ftpIslemiBitti;
    QString kullaniciAdi;
    QSignalMapper *signalMapper;

signals:
    //void dosyaKaydet_ftp(QString dosyaIsmi, QString yeniIsim);
    //void dosyaKaydet_vt(SqlSorgu s);
    void clicked(const int &text);

public slots:
    void tableWidgetTiklandi(QModelIndex);
    void vendorNameAlindi(QStringList);
    void documentTypeAlindi(QStringList);
    void btnTarihTiklandi(bool);
    void btnYenileTiklandi(bool);
    void btnAraTiklandi(bool b);
    void btnTemizleTiklandi(bool b);
    void btnDosyaAcTiklandi(bool);
    void btnAyrintiTiklandi(int);
    void btnDocumentTypeTiklandi(bool);
    void btnVendorNameTiklandi(bool);
    void takvimKapandi(QDate date, QPushButton *btn);
    void btnKaydetTiklandi(bool);
    //void faturaTuruListesiOlustu(QStringList sl);
    void vtKayitAlindi(SqlSorgu srg);
    void islemBitti_vt(int);
    void klasorAgacinaCiftTiklandi(QModelIndex m);
    void klasorAgacinaTiklandi(QModelIndex m);
    //void listedenElemanSecildi(QListWidgetItem* lwi);
    //void islemBitti_ftp();
    //void dosyaListesiOlusturuldu(QStringList sl);
};

#endif // QTFTP_H
