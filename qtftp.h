#ifndef QTFTP_H
#define QTFTP_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QInputDialog>

//#include "ftpthread.h"
#include "vtthread.h"
#include "takvim.h"
#include "global.h"

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

private:
    Ui::QtFtp *ui;
    VtThread *vtThread;
    //bool vtIslemiBitti;
    QFileSystemModel *dirModel;
    //FtpThread *ftpThread;
    //bool ftpIslemiBitti;
    QString kullaniciAdi;

signals:
    //void dosyaKaydet_ftp(QString dosyaIsmi, QString yeniIsim);
    //void dosyaKaydet_vt(SqlSorgu s);

public slots:
    void tableWidgetTiklandi(QModelIndex);
    void vendorNameEkle(bool);
    void documentTypeEkle(bool);
    void vendorNameAlindi(QStringList);
    void documentTypeAlindi(QStringList);
    void btnTarihTiklandi(bool b);
    void btnYenileTiklandi(bool b);
    void btnAraTiklandi(bool b);
    void btnTemizleTiklandi(bool b);
    void takvimKapandi(QDate date);
    void btnKaydetTiklandi(bool b);
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
