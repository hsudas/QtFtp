#ifndef QTFTP_H
#define QTFTP_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QListWidgetItem>

#include "ftpthread.h"
#include "takvim.h"

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
    void threadCalistir();
    FtpThread *mThread;

private:
    Ui::QtFtp *ui;

signals:
    void dosyaKaydet(QString dosyaIsmi, QString yeniIsim);

public slots:
    void dosyaListesiOlusturuldu(QStringList sl);
    void btnTarihTiklandi(bool b);
    void takvimKapandi(QDate date);
    void btnKaydetTiklandi(bool b);
    void listedenElemanSecildi(QListWidgetItem* lwi);
};

#endif // QTFTP_H
