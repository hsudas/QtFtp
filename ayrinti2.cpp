#include "ayrinti2.h"
#include "ui_ayrinti2.h"

Ayrinti2::Ayrinti2(VtThread *vt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ayrinti2)
{
    ui->setupUi(this);

    this->vt = vt;
    connect(ui->btnEkle,SIGNAL(clicked(bool)),this,SLOT(btnEkleTiklandi(bool)));
    connect(ui->btnCikar,SIGNAL(clicked(bool)),this,SLOT(btnCikarTiklandi(bool)));
    connect(ui->btnIptal,SIGNAL(clicked(bool)),this,SLOT(btnIptalTiklandi(bool)));
    connect(ui->btnKaydet,SIGNAL(clicked(bool)),this,SLOT(btnKaydetTiklandi(bool)));
}

Ayrinti2::~Ayrinti2()
{
    delete ui;
}

/**
 * @brief Ayrinti2::closeEvent : pencere kapanirken nesneyi siliyor
 */
void Ayrinti2::closeEvent(QCloseEvent *)
{
    delete this;
}

/**
 * @brief Ayrinti2::doldur : penceredeki listeyi ana ekrandaki verilerle dolduruyor
 * @param sl
 */
void Ayrinti2::doldur(QStringList sl)
{
    ui->listWidget->addItems(sl);
    for(int i = 0; i < sl.size(); i++)
    {
        ui->listWidget->item(i)->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->listWidget->item(i)->setBackground(Qt::lightGray);
    }
}

/**
 * @brief Ayrinti2::btnEkleTiklandi : ekle butonuna tıklayınca burası calisiyor
 * listeye bir satır ekler
 */
void Ayrinti2::btnEkleTiklandi(bool)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setBackground(Qt::lightGray);
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if(ui->listWidget->currentRow() == -1)//hiç bir eleman seçili değil
    {
        ui->listWidget->insertItem(ui->listWidget->count(), item);
    }
    else
    {
        ui->listWidget->insertItem(ui->listWidget->currentRow(), item);
    }
}

/**
 * @brief Ayrinti2::btnCikarTiklandi : cikar butonuna tıklayınca burası calisiyor
 * listeden satır siler
 */
void Ayrinti2::btnCikarTiklandi(bool)
{
    delete ui->listWidget->currentItem();
}

/**
 * @brief Ayrinti2::btnIptalTiklandi : iptal butonuna tiklayinca burasi calisiyor
 * pencereyi kapatır
 */
void Ayrinti2::btnIptalTiklandi(bool)
{
    close();
}

/**
 * @brief Ayrinti2::btnKaydetTiklandi : kaydet butonuna tiklayinca burasi calisiyor
 * vt ye kaydedilecek listeyi olusturup vt thred ini baslatir
 */
void Ayrinti2::btnKaydetTiklandi(bool)
{
    QStringList liste;
    for(int i=0;i<ui->listWidget->count(); i++)
    {
        if(ui->listWidget->item(i)->text() != "")//boş kalan satırlar vt ye kaydedilmesin
        {
            liste<<ui->listWidget->item(i)->text();
        }
    }

    vt->setISLEM(liste, ISLEM);
    vt->start();

    close();
}
