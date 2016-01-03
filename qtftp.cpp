#include "qtftp.h"
#include "ui_qtftp.h"

QtFtp::QtFtp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFtp)
{
    ui->setupUi(this);

    ui->btnTarih->setText(QDate::currentDate().toString(TARIH_FORMAT));
    connect(ui->btnTarih, SIGNAL(clicked(bool)), this, SLOT(btnTarihTiklandi(bool)));
    connect(ui->btnKaydet, SIGNAL(clicked(bool)), this, SLOT(btnKaydetTiklandi(bool)));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listedenElemanSecildi(QListWidgetItem*)));

    threadCalistir();
}

/*
 * listeden secim yapilinca listedenElemanSecildi(QListWidgetItem *) slotu calisiyor.
 * secilen liste elemaninin ismini txtIsim alanina yaziyor
 */
void QtFtp::listedenElemanSecildi(QListWidgetItem *lwi)
{
    ui->txtIsim->setText(lwi->text());
}

/*
 * btnKaydet e tiklayinca btnKaydetTiklandi(bool) slotu calisiyor. listeden seçim yapilmissa
 * dosyaKaydet(QString, QString) sinyalini veriyor. secim yapilmamissa hata veriyor
 */
void QtFtp::btnKaydetTiklandi(bool b)
{
    if(ui->listWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this,"hata","listeden seçim yapılmadı");
    }
    else
    {
        emit dosyaKaydet(ui->listWidget->selectedItems().at(0)->text(), ui->txtIsim->text());
    }
}

/*
 * takvim widget i kapandigi zaman takvimKapandi(QDate) slotu
 * calisiyor. secilen tarihi takvim butonuna yaziyor
 */
void QtFtp::takvimKapandi(QDate date)
{
    ui->btnTarih->setDisabled(false);
    ui->btnTarih->setText(date.toString(TARIH_FORMAT));
}

/*
 * takvim butonuna tiklandigi zaman takvim nesnesi olusturup baslatir
 */
void QtFtp::btnTarihTiklandi(bool b)
{
    Takvim *takvim = new Takvim(QDate::fromString(ui->btnTarih->text(), TARIH_FORMAT));
    connect(takvim,SIGNAL(takvimKapandi(QDate)),this,SLOT(takvimKapandi(QDate)));
    takvim->show();

    ui->btnTarih->setDisabled(true);
}

/*
 * thread nesnesi olusturur ve baslatir
 */
void QtFtp::threadCalistir()
{
    mThread = new FtpThread();
    connect(mThread,SIGNAL(dosyaListesiOlusturuldu(QStringList)),this, SLOT(dosyaListesiOlusturuldu(QStringList)));
    connect(this,SIGNAL(dosyaKaydet(QString, QString)),mThread, SLOT(dosyaKaydet(QString, QString)));
    mThread->start();
}

/*
 * thread den dosyaListesiOlusturuldu(QStringList) sinyali geldigi zaman
 * dosyaListesiOlusturuldu(QStringList) slotu basliyor. Olusan listeyi
 * arayüzdeki listWidgeta yazdiriyor
 */
void QtFtp::dosyaListesiOlusturuldu(QStringList sl)
{
    ui->listWidget->clear();
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
