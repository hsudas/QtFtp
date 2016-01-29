#include "ayrinti.h"
#include "ui_ayrinti.h"

Ayrinti::Ayrinti(VtThread *vt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ayrinti)
{
    ui->setupUi(this);

    this->vt = vt;
    connect(ui->btnInvoiceDate, SIGNAL(clicked(bool)), this, SLOT(btnTarihTiklandi(bool)));
    connect(ui->btnKaydet, SIGNAL(clicked(bool)), this, SLOT(btnKaydetTiklandi(bool)));
    connect(ui->btnIptal, SIGNAL(clicked(bool)), this, SLOT(btnIptalTiklandi(bool)));
    connect(ui->btnSil, SIGNAL(clicked(bool)), this, SLOT(btnSilTiklandi(bool)));
}

Ayrinti::~Ayrinti()
{
    delete ui;
}

/**
 * @brief Ayrinti::btnSilTiklandi : sil butonuna tiklandiginda buraya geliyor.
 * vt den kaydi siler
 */
void Ayrinti::btnSilTiklandi(bool)
{
    if(!QMessageBox::question(this, "","record will be deleted ?","OK","Cancel"))
    {
        vt->setISLEM(id, _ISLEM_SIL);
        vt->start();

        close();
        delete this;
    }
}

/**
 * @brief Ayrinti::btnIptalTiklandi : iptal butonuna tıklandığı zaman ekranı kapatır
 */
void Ayrinti::btnIptalTiklandi(bool)
{
    close();
    delete this;
}

/**
 * @brief Ayrinti::btnKaydetTiklandi : kaydet butonuna tıklandığı zaman sqlsorgu sınıfını olusturur
 * vt threadini baslatir
 */
void Ayrinti::btnKaydetTiklandi(bool)
{
    SqlSorgu sqlsorgu;
    sqlsorgu.id = id;
    sqlsorgu.vendorName = ui->cbVendorName->currentText();
    sqlsorgu.documentType = ui->cbDocumentType->currentText();
    sqlsorgu.amount = ui->txtTotalAmount->text();
    sqlsorgu.invoiceNumber = ui->txtInvoiceNumber->text();
    QDate date = QDate::fromString(ui->btnInvoiceDate->text(),"yyyy-MM-dd");
    sqlsorgu.invoiceDate = date.toString("MM/dd/yyyy");
    sqlsorgu.filePath = ui->txtFilePath->text();
    sqlsorgu.saveDate = QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss");
    //sqlsorgu.userName = kullaniciAdi;

    vt->setISLEM(sqlsorgu, _ISLEM_GUNCELLE);
    vt->start();

    close();
    delete this;
}

/**
 * @brief Ayrinti::doldur : tablewidget tan alınan verileri ayrinti ekranındaki alanlara yerlestirir
 */
void Ayrinti::doldur()
{
    ui->txtFilePath->setText(filePath);
    ui->txtInvoiceNumber->setText(invoiceNumber);
    ui->txtTotalAmount->setText(amount);
    ui->btnInvoiceDate->setText(invoiceDate);
    ui->cbDocumentType->addItems(listDocumentType);
    ui->cbVendorName->addItems(listVendorName);

    int index = ui->cbDocumentType->findText(documentType);
    ui->cbDocumentType->setCurrentIndex(index);
    index = ui->cbVendorName->findText(vendorName);
    ui->cbVendorName->setCurrentIndex(index);
}

/**
 * @brief Ayrinti::btnTarihTiklandi : tarih butonuna tıklandığı zaman takvim sınıfını calistirir
 */
void Ayrinti::btnTarihTiklandi(bool)
{
    QPushButton *cagiranBtnTarih = qobject_cast<QPushButton*>(this->sender());
    Takvim *takvim = new Takvim(QDate::fromString(ui->btnInvoiceDate->text(), TARIH_FORMAT), cagiranBtnTarih);
    connect(takvim,SIGNAL(takvimKapandi(QDate, QPushButton *)),this,SLOT(takvimKapandi(QDate, QPushButton *)));
    takvim->show();

    cagiranBtnTarih->setDisabled(true);
}

/**
 * @brief QtFtp::takvimKapandi : takvim widget i kapandigi zaman takvimKapandi slotu calisiyor
 * secilne tarihi cagiran butona yaziyor
 * @param date : secilen tarih
 * @param btn : takvim widget'ini acan buton
 */
void Ayrinti::takvimKapandi(QDate date, QPushButton *btn)
{
    btn->setDisabled(false);
    btn->setText(date.toString(TARIH_FORMAT));
}
