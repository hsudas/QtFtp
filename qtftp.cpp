#include "qtftp.h"
#include "ui_qtftp.h"

QtFtp::QtFtp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFtp)
{
    ui->setupUi(this);
    threadCalistir();
}

/*
 * thread nesnesi olusturur ve baslatir
 */
void QtFtp::threadCalistir()
{
    mThread = new FtpThread();
    connect(mThread,SIGNAL(dosyaListesiOlusturuldu(QStringList)),this, SLOT(dosyaListesiOlusturuldu(QStringList)));
    mThread->start();
}

/*
 * thread den dosyaListesiOlusturuldu(QStringList) sinyali geldigi zaman
 * dosyaListesiOlusturuldu(QStringList) slotu basliyor. Olusan listeyi
 * arayüzdeki listWidgeta yazdiriyor
 */
void QtFtp::dosyaListesiOlusturuldu(QStringList sl)
{
    for(int i = 0; i < sl.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem (sl.at(i));
        ui->listWidget->insertItem(ui->listWidget->currentRow(), item);
    }
}

QtFtp::~QtFtp()
{
    delete mThread;
    delete ui;
}
