#include "qtftp.h"
#include "ui_qtftp.h"

QtFtp::QtFtp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFtp)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    ui->setupUi(this);

    //ayar dosyasi yoksa uygulama kapanacak
    if(!Global::config->dosyaVar)
    {
        QMessageBox::information(0, "hata", "ayar dosyası yok \n "+Global::config->ayarDosyasiYolu);
        exit(0);
    }
    else
    {
        ui->btnTarih->setText(QDate::currentDate().toString(TARIH_FORMAT));
        connect(ui->btnTarih, SIGNAL(clicked(bool)), this, SLOT(btnTarihTiklandi(bool)));
        connect(ui->btnKaydet, SIGNAL(clicked(bool)), this, SLOT(btnKaydetTiklandi(bool)));
        //connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listedenElemanSecildi(QListWidgetItem*)));
        connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(klasorAgacinaCiftTiklandi(QModelIndex)));

        vtIslemiBitti = false;
        //ftpIslemiBitti = false;

        //ftpThreadCalistir();
        vtThreadCalistir();
        klasorAgaciOlustur();
    }
}

/*
 * paylasilan klasorun icindeki dosyalari treeview a ekliyor
 */
void QtFtp::klasorAgaciOlustur()
{
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath((QDir::currentPath()));
    ui->treeView->setModel(dirModel);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    //dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->treeView->setRootIndex((dirModel)->index(Global::config->CNF_KLASOR_AGACI_ROOT));
}

/*
 * klasorAgaci treeView ında bir dosyaya cift tiklandigi zaman
 * klasorAgacinaCiftTiklandi(QModelIndex) slotu calisiyor. cift tiklanan
 * dosyayi aciyor
 */
void QtFtp::klasorAgacinaCiftTiklandi(QModelIndex m)
{
    QString url = dirModel->fileInfo(m).filePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

/*
 * btnKaydet e tiklayinca btnKaydetTiklandi(bool) slotu calisiyor. listeden seçim yapilmissa
 * dosyaKaydet_ftp(QString, QString) ve dosyaKaydet_vt(QString, QString, Qstring) sinyalini veriyor.
 * secim yapilmamissa ve isim alani bossa hata veriyor
 */
void QtFtp::btnKaydetTiklandi(bool b)
{
    //    if(ui->listWidget->selectedItems().isEmpty())
    //    {
    //        QMessageBox::warning(this,"hata","listeden seçim yapılmadı");
    //    }
    if( ui->txtIsim->text().isEmpty())
    {
        QMessageBox::warning(this,"hata","isim alanı boş");
    }
    else
    {
        //emit dosyaKaydet_ftp(ui->listWidget->selectedItems().at(0)->text(), ui->txtIsim->text());
        emit dosyaKaydet_vt(ui->btnTarih->text(), ui->cbFaturaTuru->currentText(), ui->txtIsim->text());
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
 * vtthread nesnesi olusturur ve baslatir
 */
void QtFtp::vtThreadCalistir()
{
    vtThread = new VtThread();
    connect(vtThread, SIGNAL(islemBitti()), this, SLOT(islemBitti_vt()));
    connect(vtThread, SIGNAL(faturaTuruListesiOlustu(QStringList)), this, SLOT(faturaTuruListesiOlustu(QStringList)));
    connect(this, SIGNAL(dosyaKaydet_vt(QString, QString, QString)), vtThread, SLOT(dosyaKaydet(QString, QString, QString)));

    vtThread->start();
}

/*
 * vtthread faturaTuruListesiOlustu(QStringList) sinyalini verdigi zaman
 * faturaTuruListesiOlustu(QStringList) slotu calisiyor. vt den alinan fatura turu listesini
 * combobox'a yerlestiriyor
 */
void QtFtp::faturaTuruListesiOlustu(QStringList sl)
{
    ui->cbFaturaTuru->clear();
    ui->cbFaturaTuru->addItems(sl);
}

/*
 * vt threadi işini bitirdiği zaman islemBitti() sinyalini veriyor. islemBitti() sinyali
 * islemBitti_vt() slotunu çağırıyor
 */
void QtFtp::islemBitti_vt()
{
    vtIslemiBitti = true;
    islemBitti();
}

/*
 * ftp ve vt threadleri işlerini bitirince islemBitti() cagriliyor ve ekrana messageBox cikariliyor
 */
void QtFtp::islemBitti()
{
    if(vtIslemiBitti)
    {
        QMessageBox::information(this,"bilgi","işlem bitti");
        vtIslemiBitti = false;
    }
    else
    {
        qDebug()<<"hata";
    }
    /*
    if(vtIslemiBitti && ftpIslemiBitti)
    {
        QMessageBox::information(this,"bilgi","işlem bitti");
        vtIslemiBitti = false;
        ftpIslemiBitti = false;
    }
    else
    {
        qDebug()<<"hata";
    }
    */
}

QtFtp::~QtFtp()
{
    delete vtThread;
    //delete ftpThread;
    delete ui;
}

/*
 * listeden secim yapilinca listedenElemanSecildi(QListWidgetItem *) slotu calisiyor.
 * secilen liste elemaninin ismini txtIsim alanina yaziyor
 */
//void QtFtp::listedenElemanSecildi(QListWidgetItem *lwi)
//{
//    ui->txtIsim->setText(lwi->text());
//}

/*
 * ftpthread nesnesi olusturur ve baslatir
 */
//void QtFtp::ftpThreadCalistir()
//{
//    ftpThread = new FtpThread();
//    connect(ftpThread, SIGNAL(islemBitti()), this, SLOT(islemBitti_ftp()));
//    connect(ftpThread,SIGNAL(dosyaListesiOlusturuldu(QStringList)),this, SLOT(dosyaListesiOlusturuldu(QStringList)));
//    connect(this,SIGNAL(dosyaKaydet_ftp(QString, QString)),ftpThread, SLOT(dosyaKaydet(QString, QString)));
//    ftpThread->start();
//}

/*
 * thread den dosyaListesiOlusturuldu(QStringList) sinyali geldigi zaman
 * dosyaListesiOlusturuldu(QStringList) slotu basliyor. Olusan listeyi
 * arayüzdeki listWidgeta yazdiriyor
 */
//void QtFtp::dosyaListesiOlusturuldu(QStringList sl)
//{
//    ui->listWidget->clear();
//    for(int i = 0; i < sl.size(); i++)
//    {
//        QListWidgetItem *item = new QListWidgetItem (sl.at(i));
//        ui->listWidget->insertItem(ui->listWidget->currentRow(), item);
//    }
//}

/*
 * ftp threadi işini bitirdigi zaman islemBitti() sinyalini veriyor. islemBitti() sinyali
 * islemBitti_ftp() slotunu çağırıyor
 */
//void QtFtp::islemBitti_ftp()
//{
//    ftpIslemiBitti = true;
//    islemBitti();
//}
