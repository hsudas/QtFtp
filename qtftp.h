#ifndef QTFTP_H
#define QTFTP_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include <QDesktopServices>

#include "ftpthread.h"
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
    void ftpThreadCalistir();
    void vtThreadCalistir();
    void islemBitti();
    void klasorAgaciOlustur();

private:
    Ui::QtFtp *ui;
    FtpThread *ftpThread;
    VtThread *vtThread;
    bool vtIslemiBitti;
    bool ftpIslemiBitti;
    QFileSystemModel *dirModel;

signals:
    void dosyaKaydet_ftp(QString dosyaIsmi, QString yeniIsim);
    void dosyaKaydet_vt(QString tarih, QString faturaTuru, QString isim);

public slots:
    void dosyaListesiOlusturuldu(QStringList sl);
    void btnTarihTiklandi(bool b);
    void takvimKapandi(QDate date);
    void btnKaydetTiklandi(bool b);
    void listedenElemanSecildi(QListWidgetItem* lwi);
    void faturaTuruListesiOlustu(QStringList sl);
    void islemBitti_vt();
    void islemBitti_ftp();
    void klasorAgacinaCiftTiklandi(QModelIndex m);
};

#endif // QTFTP_H
