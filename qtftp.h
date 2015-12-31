#ifndef QTFTP_H
#define QTFTP_H

#include <QMainWindow>

#include <QDebug>
#include "ftpthread.h"

namespace Ui {
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

public slots:
    void dosyaListesiOlusturuldu(QStringList sl);

};

#endif // QTFTP_H
