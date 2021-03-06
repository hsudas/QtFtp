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
#include <QKeyEvent>

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
    void klasorAgaciOlustur();
    void tusEtkisiz(bool);
    bool ayarlariKontrolEt();
    void alanlariTemizle();
    void vtYenile();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::QtFtp *ui;
    VtThread *vtThread;
    QFileSystemModel *dirModel;
    QString kullaniciAdi;
    QSignalMapper *signalMapper;
    bool yazilacak;
    QString sonAmountText;

signals:
    void clicked(const int &text);

public slots:
    void btnArtiEksiTiklandi(bool);
    void yaziDegisti(QString);
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
    void vtKayitAlindi(SqlSorgu srg);
    void islemBitti_vt(int, float);
    void klasorAgacinaCiftTiklandi(QModelIndex m);
    void klasorAgacinaTiklandi(QModelIndex m);
    void toplamlariAl(float toplamTutar);
};

#endif // QTFTP_H
