#include "qtftp.h"
#include "ui_qtftp.h"

QtFtp::QtFtp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFtp)
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    ui->setupUi(this);

    //ayar dosyasi yoksa uygulama kapanacak
    if(!Global::config->dosyaVar)
    {
        QMessageBox::information(0, "hata", "ayar dosyası yok\n"+Global::config->ayarDosyasiYolu);
        exit(0);
    }
    else
    {
        //ayarlarda eksiklik varsa uygulamadan çıksın
        if(!ayarlariKontrolEt())
        {
            exit(0);
        }

        //kullanici adini soran dialog
        QInputDialog inputDialog;
        inputDialog.setOptions(QInputDialog::NoButtons);
        bool ok;

        do
        {
            QString text = inputDialog.getText(this ,"","Kullanıcı Adı", QLineEdit::Normal,"", &ok);
            kullaniciAdi = text;
            if(!ok)//iptale tiklanirsa uygulamadan ciksin
            {
                exit(0);
            }
        }
        while(kullaniciAdi.isEmpty());//kullanici adi girilene kadar sorsun

        if(Global::config->CNF_SHOW_FILE_TREE == "0")
        {
            ui->treeView->hide();
        }

        ui->btnTarih->setText(QDate::currentDate().toString(TARIH_FORMAT));
        ui->btnTarih2->setText(QDate::currentDate().toString(TARIH_FORMAT));
        connect(ui->btnTarih, SIGNAL(clicked(bool)), this, SLOT(btnTarihTiklandi(bool)));
        connect(ui->btnTarih2, SIGNAL(clicked(bool)), this, SLOT(btnTarihTiklandi(bool)));
        connect(ui->btnKaydet, SIGNAL(clicked(bool)), this, SLOT(btnKaydetTiklandi(bool)));
        connect(ui->btnYenile, SIGNAL(clicked(bool)), this, SLOT(btnYenileTiklandi(bool)));
        connect(ui->btnAra, SIGNAL(clicked(bool)), this, SLOT(btnAraTiklandi(bool)));
        connect(ui->btnTemizle, SIGNAL(clicked(bool)), this, SLOT(btnTemizleTiklandi(bool)));
        connect(ui->btnVendorName, SIGNAL(clicked(bool)), this, SLOT(btnVendorNameTiklandi(bool)));
        connect(ui->btnDocumentType, SIGNAL(clicked(bool)), this, SLOT(btnDocumentTypeTiklandi(bool)));
        connect(ui->btnDosyaAc, SIGNAL(clicked(bool)), this, SLOT(btnDosyaAcTiklandi(bool)));
        connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(klasorAgacinaCiftTiklandi(QModelIndex)));
        connect(ui->treeView, SIGNAL(clicked(QModelIndex)),this, SLOT(klasorAgacinaTiklandi(QModelIndex)));
        connect(ui->tableWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tableWidgetTiklandi(QModelIndex)));
        connect(ui->txtTotalAmount,SIGNAL(textEdited(QString)), this, SLOT(yaziDegisti(QString)));
        connect(ui->btnArtiEksi, SIGNAL(clicked(bool)), this, SLOT(btnArtiEksiTiklandi(bool)));

        vtThreadCalistir();
        klasorAgaciOlustur();

        ui->tableWidget->setColumnCount(SUTUN_TOPLAM);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<""<<"id"<<"Document Type"<<"Vendor Name"<<"Invoice Number"<<"Total Amount"<<"File Path"<<"Save Date"<<"Invoice Date"<<"User Name");
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget->hideColumn(1);
        ui->tableWidget->setColumnWidth(0,30);
    }

    qApp->installEventFilter(this);

    yazilacak = false;
    sonAmountText = ui->txtTotalAmount->text();
}

/**
 * @brief QtFtp::eventFilter : amount alanında sadece sayılar ve geri tusu calismasi icin heng tusa basildigini kontrol ediyor
 */
bool QtFtp::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->txtTotalAmount && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if((key->key() >= 48 && key->key() <= 57) || (key->key() == Qt::Key_Backspace) || (key->key() == Qt::Key_Delete))
        {
            yazilacak = true;
        }
        else
        {
            yazilacak = false;
        }
    }

    return QObject::eventFilter(obj, event);
}

/**
 * @brief QtFtp::btnArtiEksiTiklandi : tutarin pozitif negatif oldugunu anlamak için
 * tıklandıgı zaman butonun uzerindeki isareti degistiriyor
 */
void QtFtp::btnArtiEksiTiklandi(bool)
{
    if(ui->btnArtiEksi->text() == "+")
    {
        ui->btnArtiEksi->setText("-");
    }
    else
    {
        ui->btnArtiEksi->setText("+");
    }
}

/**
 * @brief QtFtp::yaziDegisti : txtAmount degisince burası calisir
 * txtAmount yazilan sayıya virgul ekler
 * @param yeniYazi
 */
void QtFtp::yaziDegisti(QString yeniYazi)
{
    int cursorPos = ui->txtTotalAmount->cursorPosition();

    if(yazilacak)
    {
        //qDebug()<<"number 0 : "<<yeniYazi<<" -- txt : "<<ui->txtTotalAmount->text();

        QString solText = ui->txtTotalAmount->text().left(ui->txtTotalAmount->text().lastIndexOf("."));
        QString sagText = ui->txtTotalAmount->text().mid(ui->txtTotalAmount->text().lastIndexOf(".")+1);

        //qDebug()<<"number 1 : "<<solText<<" -- "<<sagText;

        int once = solText.size();
        //qDebug()<<"once : "<<once;


        solText.replace(QString(","), QString(""));
        //qDebug()<<"number 2 : "<<solText;

        double solDouble = solText.toDouble();

        //qDebug()<<"number 3 : "<<solDouble;
        QString solTextAyrilmis = QLocale(QLocale::English).toString(solDouble, 'f', 0);
        //qDebug()<<"number 4 : "<<solTextAyrilmis;

        ui->txtTotalAmount->setText(solTextAyrilmis+"."+sagText);

        sonAmountText= ui->txtTotalAmount->text();

        int sonra = solTextAyrilmis.size();
        //qDebug()<<"sonra : "<<sonra;

        if(yeniYazi.size() > 5 && sonra > once)
        {
            ui->txtTotalAmount->setCursorPosition(cursorPos+1);
        }
        else if(sonra < once)
        {
            ui->txtTotalAmount->setCursorPosition(cursorPos-1);
        }
        else
        {
            ui->txtTotalAmount->setCursorPosition(cursorPos);
        }
    }
    else
    {
        ui->txtTotalAmount->setText(sonAmountText);
        ui->txtTotalAmount->setCursorPosition(cursorPos-1);
    }
}

/**
 * @brief QtFtp::btnVendorNameTiklandi : vendorName butonuna tiklayinca burasi calisiyor
 * vendorName combobox ından verileri okuyup ayrinti2 penceresini acar
 */
void QtFtp::btnVendorNameTiklandi(bool)
{
    Ayrinti2 *ayrinti2 = new Ayrinti2(vtThread);

    QStringList liste;
    for(int i = 1; i < ui->cbVendorName->count(); i++)
    {
        liste.append(ui->cbVendorName->itemText(i));
    }
    ayrinti2->doldur(liste);
    ayrinti2->setISLEM(_ISLEM_VENDOR_NAME);
    ayrinti2->show();
}

/**
 * @brief QtFtp::btnDocumentTypeTiklandi : documentType butonuna tiklayinca burasi calisiyor
 * documentType combobox ından verileri okuyup ayrinti2 penceresini acar
 */
void QtFtp::btnDocumentTypeTiklandi(bool)
{
    Ayrinti2 *ayrinti2 = new Ayrinti2(vtThread);

    QStringList liste;
    for(int i = 1; i < ui->cbDocumentType->count(); i++)
    {
        liste.append(ui->cbDocumentType->itemText(i));
    }
    ayrinti2->doldur(liste);
    ayrinti2->setISLEM(_ISLEM_DOCUMENT_TYPE);
    ayrinti2->show();
}

/**
 * @brief QtFtp::ayarlariKontrolEt
 * config dosyasından okunan değişkenleri kontrol eder. boş olan varsa ayar config dosyasında yoktur
 * hata verip uygulamadan çıkar
 */
bool QtFtp::ayarlariKontrolEt()
{
    if(Global::config->CNF_VT_USERNAME.isEmpty())
    {
        QMessageBox::warning(0, "error", "no VT_USERNAME option in config file");
        return false;
    }
    else if(Global::config->CNF_VT_PASSWORD.isEmpty())
    {
        QMessageBox::warning(0, "error", "no VT_PASSWORD option in config file");
        return false;
    }
    else if(Global::config->CNF_VT_IP.isEmpty())
    {
        QMessageBox::warning(0, "error", "no VT_IP option in config file");
        return false;
    }
    else if(Global::config->CNF_VT_DATABASE_NAME.isEmpty())
    {
        QMessageBox::warning(0, "error", "no VT_DATABASE_NAME option in config file");
        return false;
    }
    else if(Global::config->CNF_KLASOR_AGACI_ROOT.isEmpty())
    {
        QMessageBox::warning(0, "error", "no SHARED_FOLDER option in config file");
        return false;
    }
    else if(Global::config->CNF_KLASOR_ARCHIVE.isEmpty())
    {
        QMessageBox::warning(0, "error", "no ARCHIVE_FOLDER option in config file");
        return false;
    }
    else if(Global::config->CNF_FILE_NAME_FORMAT.isEmpty())
    {
        QMessageBox::warning(0, "error", "no FILE_NAME_FORMAT option in config file");
        return false;
    }
    else if(Global::config->CNF_SHOW_FILE_TREE.isEmpty())
    {
        QMessageBox::warning(0, "error", "no SHOW_FILE_TREE option in config file");
        return false;
    }

    return true;
}

/**
 * @brief QtFtp::tableWidgetTiklandi
 * tablewidget a çift tiklandigi zaman bu slot calisiyor.
 * tılanan satırda dosya yolunu alıp açıyor
 * @param mi
 * tıklanan satırın data modeli
 */
void QtFtp::tableWidgetTiklandi(QModelIndex mi)
{
    QString dosyaYolu = ui->tableWidget->item(mi.row(),SUTUN_FILE_PATH)->text();
    if(QFileInfo(dosyaYolu).exists())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dosyaYolu));
    }
    else
    {
        QMessageBox::warning(this,"error","file not found\n"+dosyaYolu);
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
 * klasorAgaci treeView ında bir dosyaya tiklandigi zaman
 * klasorAgacinaTiklandi(QModelIndex) slotu calisiyor. tiklanan
 * dosya ismini txtFilePath alanina yaziyor
 */
void QtFtp::klasorAgacinaTiklandi(QModelIndex m)
{
    QString url = dirModel->fileInfo(m).filePath();
    ui->txtFilePath->setText(QDir::toNativeSeparators(url));
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
 * thread basladigi zaman thread i baslatan diger tuslari etkisiz hale getiriyor
 */
void QtFtp::tusEtkisiz(bool b)
{
    ui->btnAra->setDisabled(b);
    ui->btnYenile->setDisabled(b);
    ui->btnKaydet->setDisabled(b);
    ui->btnTemizle->setDisabled(b);
}

/**
 * @brief QtFtp::btnDosyaAcTiklandi
 * dosya ac butonunan tiklandigi zaman bu slot calisiyor.
 * open file dialog aciyor
 */
void QtFtp::btnDosyaAcTiklandi(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", Global::config->CNF_KLASOR_AGACI_ROOT, "*.*");
    ui->txtFilePath->setText(QDir::toNativeSeparators(fileName));
}

/*
 * btnTemizle ye tiklandigi zaman btnTemizleTiklandi(bool) slotu calisiyor.
 * arayuzdeki giriş yapilan alanlari temizliyor.
 */
void QtFtp::btnTemizleTiklandi(bool b)
{
    ui->cbVendorName->setCurrentIndex(0);
    ui->cbDocumentType->setCurrentIndex(0);
    ui->txtFilePath->clear();
    ui->txtTotalAmount->clear();
    ui->txtInvoiceNumber->clear();
}

/*
 * btnYenile ye tiklandigi zaman btnYenileTiklandi(bool) slotu calisiyor.
 * vt thread i baslatiyor
 */
void QtFtp::btnYenileTiklandi(bool)
{
    vtYenile();
}

void QtFtp::vtYenile()
{
    ui->tableWidget->setRowCount(0);
    tusEtkisiz(true);
    vtThread->setISLEM(_ISLEM_TUM_KAYITLAR);
    vtThread->start();
}

/*
 * btnArama ya tiklaninca btnAraTiklandi(bool) slotu calisiyor.
 * SqlSorgu nesnesi olusturup ISLEM_ARAMA_SONUCU icin thread i baslatiyor
 */
void QtFtp::btnAraTiklandi(bool b)
{
    ui->tableWidget->setRowCount(0);

    SqlSorgu sqlsorgu;
    sqlsorgu.vendorName = ui->cbVendorName->currentText();
    sqlsorgu.documentType = ui->cbDocumentType->currentText();
    sqlsorgu.amount = ui->txtTotalAmount->text();
    //QDate date = QDate::fromString(ui->btnTarih->text(),"yyyy-MM-dd");
    //sqlsorgu.invoiceDate = date.toString("MM/dd/yyyy");
    sqlsorgu.invoiceDate = ui->btnTarih->text();
    sqlsorgu.invoiceDate2 = ui->btnTarih2->text();
    sqlsorgu.filePath = ui->txtFilePath->text();
    sqlsorgu.userName = kullaniciAdi;
    //sqlsorgu.saveDate = QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss");

    tusEtkisiz(true);
    vtThread->setISLEM(sqlsorgu, _ISLEM_ARAMA_SONUCU);
    vtThread->start();
}

/**
 * @brief QtFtp::btnKaydetTiklandi
 * btnKaydet e tiklayinca btnKaydetTiklandi(bool) slotu calisiyor.
 * dosyayi arsiv klasorune kopyalar
 * vt ye kayit ekler
 */
void QtFtp::btnKaydetTiklandi(bool)
{
    if( ui->txtFilePath->text().isEmpty())
    {
        QMessageBox::warning(this, "error", "file name is empty");
    }
    else
    {
        //dosya yolundan dosya ismini alıp dosyayı kopyalıyor
        QString dosyaIsmi = ui->txtFilePath->text().mid(ui->txtFilePath->text().lastIndexOf(QDir::separator())+1,ui->txtFilePath->text().size());
        if(QFileInfo(ui->txtFilePath->text()).isFile())
        {
            QStringList listeYeniDosya = dosyaIsmi.split(".");
            QString yeniDosya = listeYeniDosya.at(0);
            yeniDosya.append(QDateTime::currentDateTime().toString(Global::config->CNF_FILE_NAME_FORMAT));
            yeniDosya.append(".");
            yeniDosya.append(listeYeniDosya.at(1));
            QFile::copy(ui->txtFilePath->text(), Global::config->CNF_KLASOR_ARCHIVE+QDir::separator()+yeniDosya);

            //vt sorgusunu hazırlıyor
            SqlSorgu sqlsorgu;
            sqlsorgu.vendorName = ui->cbVendorName->currentText();
            sqlsorgu.documentType = ui->cbDocumentType->currentText();

            if(ui->btnArtiEksi->text() == "+")
            {
                sqlsorgu.amount = ui->txtTotalAmount->text();
            }
            else
            {
                sqlsorgu.amount = ui->btnArtiEksi->text() + ui->txtTotalAmount->text();
            }

            //sqlsorgu.amount = ui->txtTotalAmount->text();
            sqlsorgu.invoiceNumber = ui->txtInvoiceNumber->text();
            QDate date = QDate::fromString(ui->btnTarih->text(),"yyyy-MM-dd");
            sqlsorgu.invoiceDate = date.toString("MM/dd/yyyy");
            sqlsorgu.filePath = Global::config->CNF_KLASOR_ARCHIVE+QDir::separator()+yeniDosya;
            sqlsorgu.saveDate = QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss");
            sqlsorgu.userName = kullaniciAdi;

            tusEtkisiz(true);

            ui->tableWidget->setRowCount(0);//thread in nihayetinde baştan dolacagi icin tabloyu boşaltiyorum
            vtThread->setISLEM(sqlsorgu, _ISLEM_KAYDET);
            vtThread->start();
        }
        else
        {
            QMessageBox::warning(this, "error", "directory selected");
        }
    }
}

/**
 * @brief QtFtp::takvimKapandi : takvim widget i kapandigi zaman takvimKapandi slotu calisiyor
 * secilne tarihi cagiran butona yaziyor
 * @param date : secilen tarih
 * @param btn : takvim widget'ini acan buton
 */
void QtFtp::takvimKapandi(QDate date, QPushButton *btn)
{
    btn->setDisabled(false);
    btn->setText(date.toString(TARIH_FORMAT));
}

/**
 * @brief QtFtp::btnTarihTiklandi : takvim butonuna tiklandigi zaman takvim nesnesi olusturup baslatir
 */
void QtFtp::btnTarihTiklandi(bool)
{
    QPushButton *cagiranBtnTarih = qobject_cast<QPushButton*>(this->sender());
    Takvim *takvim = new Takvim(QDate::fromString(ui->btnTarih->text(), TARIH_FORMAT), cagiranBtnTarih);
    connect(takvim,SIGNAL(takvimKapandi(QDate, QPushButton *)),this,SLOT(takvimKapandi(QDate, QPushButton *)));
    takvim->show();

    cagiranBtnTarih->setDisabled(true);
}

/*
 * vtthread nesnesi olusturur ve baslatir
 */
void QtFtp::vtThreadCalistir()
{
    signalMapper = new QSignalMapper(this);//clicked signalinde in parametre gonderebilmek icin

    vtThread = new VtThread();
    vtThread->setISLEM(_ISLEM_BASLANGIC);

    connect(vtThread, SIGNAL(vendorNameAlindi(QStringList)),this, SLOT(vendorNameAlindi(QStringList)));
    connect(vtThread, SIGNAL(documentTypeAlindi(QStringList)),this, SLOT(documentTypeAlindi(QStringList)));
    connect(vtThread, SIGNAL(islemBitti(int, float)), this, SLOT(islemBitti_vt(int, float)));
    connect(vtThread, SIGNAL(vtKayitAlindi(SqlSorgu)), this, SLOT(vtKayitAlindi(SqlSorgu)));

    tusEtkisiz(true);
    vtThread->start();
}

/**
 * @brief QtFtp::vendorNameAlindi
 * @param sl : vt deki değerlerden oluşan liste
 * VtThread::vendorNameGetir() fonksiyonu işini bitirince VtThread::vendorNameAlindi() sinyalini verir.
 * bu sinyal QtFtp::vendorNameAlindi() slotunu cagirir. Bu slot cbVendorName combobox ını doldurur.
 */
void QtFtp::vendorNameAlindi(QStringList sl)
{
    ui->cbVendorName->clear();
    ui->cbVendorName->addItem("");
    ui->cbVendorName->addItems(sl);
}

/**
 * @brief QtFtp::documentTypeAlindi
 * @param sl : vt deki değerlerden oluşan liste
 * VtThread::documentTypeAlindi() fonksiyonu işini bitirince VtThread::documentTypeAlindi() sinyalini verir.
 * bu sinyal QtFtp::documentTypeAlindi() slotunu cagirir. Bu slot cbDocumentType combobox ını doldurur.
 */
void QtFtp::documentTypeAlindi(QStringList sl)
{
    ui->cbDocumentType->clear();
    ui->cbDocumentType->addItem("");
    ui->cbDocumentType->addItems(sl);
}

/**
 * @brief QtFtp::vtKayitAlindi : vtKayitAlindi() sinyali vtKayitAlindi() slotunu cagiriyor
 * vt den alinan veriyi tablewidget a yaziyor
 * @param srg : vt den gelen veriler
 */
void QtFtp::vtKayitAlindi(SqlSorgu srg)
{
    QPushButton *btnAyrinti = new QPushButton();
    btnAyrinti->setIcon(QIcon(":/resim/disli.png"));
    connect(btnAyrinti, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(btnAyrinti, ui->tableWidget->rowCount());

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, SUTUN_AYRINTI, btnAyrinti);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_ID, new QTableWidgetItem(srg.id));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_DOCUMENT_TYPE, new QTableWidgetItem(srg.documentType));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_VENDOR_NAME, new QTableWidgetItem(srg.vendorName));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_INVOICE_NUMBER, new QTableWidgetItem(srg.invoiceNumber));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_TOTAL_AMOUNT, new QTableWidgetItem(srg.amount));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_FILE_PATH, new QTableWidgetItem(srg.filePath));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_SAVE_DATE ,new QTableWidgetItem(srg.saveDate));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_INVOICE_DATE, new QTableWidgetItem(srg.invoiceDate));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, SUTUN_USER_NAME, new QTableWidgetItem(srg.userName));
}

/**
 * @brief QtFtp::btnAyrintiTiklandi : tablewidget daki butona tıklayınca burası çalışıyor
 * tablewidget ta yazılı dataları alıp Ayrinti sinifinin nesnesine gonderir
 * @param satir : tablewidget satırı
 */
void QtFtp::btnAyrintiTiklandi(int satir)
{
    Ayrinti *ayrinti = new Ayrinti(vtThread);
    ayrinti->setID(ui->tableWidget->item(satir,SUTUN_ID)->text());
    ayrinti->setdocumentType(ui->tableWidget->item(satir,SUTUN_DOCUMENT_TYPE)->text());
    ayrinti->setvendorName(ui->tableWidget->item(satir,SUTUN_VENDOR_NAME)->text());
    ayrinti->setinvoiceNumber(ui->tableWidget->item(satir,SUTUN_INVOICE_NUMBER)->text());
    ayrinti->setamount(ui->tableWidget->item(satir,SUTUN_TOTAL_AMOUNT)->text());
    ayrinti->setfilePath(ui->tableWidget->item(satir,SUTUN_FILE_PATH)->text());
    ayrinti->setinvoiceDate(ui->tableWidget->item(satir,SUTUN_INVOICE_DATE)->text());

    QStringList liste;
    for(int i = 0; i < ui->cbDocumentType->count(); i++)
    {
        liste.append(ui->cbDocumentType->itemText(i));
    }
    ayrinti->setlistDocumentType(liste);
    liste.clear();
    for(int i = 0; i < ui->cbVendorName->count(); i++)
    {
        liste.append(ui->cbVendorName->itemText(i));
    }
    ayrinti->setlistVendorName(liste);

    ayrinti->doldur();
    ayrinti->show();
}

/**
 * @brief QtFtp::toplamlariAl : toplam tutar ve kayıt sayısını ui ye yazdırır
 * @param toplamTutar : gridde lisstelen kayıtların toplam tutari
 */
void QtFtp::toplamlariAl(float toplamTutar)
{
    QString tutar = QLocale(QLocale::English).toString(toplamTutar, 'f', 2);
    ui->txtToplamTutar->setText(tutar);
    ui->txtToplamKayit->setText(QString::number(ui->tableWidget->rowCount()));
}

/**
 * @brief QtFtp::islemBitti_vt : vt threadi işini bitirdiği zaman islemBitti() sinyalini veriyor.
 * islemBitti() sinyali islemBitti_vt() slotunu çağırıyor. islem turune gore ekrana
 * messageBox cikariyor
 * @param islemTuru : biten islemin turu
 */
void QtFtp::islemBitti_vt(int islemTuru, float toplamTutar)
{
    tusEtkisiz(false);

    switch (islemTuru)
    {
    case _ISLEM_KAYDET:
        QMessageBox::information(this,"info","saved");
        alanlariTemizle();
        toplamlariAl(toplamTutar);
        break;
    case _ISLEM_ARAMA:
        toplamlariAl(toplamTutar);
        break;
    case _ISLEM_YENILE:
        toplamlariAl(toplamTutar);
        break;
    case _ISLEM_EKLE_VENDOR_NAME:
        QMessageBox::information(this,"info","vendor name added");
        break;
    case _ISLEM_EKLE_DOCUMENT_TYPE:
        QMessageBox::information(this,"info","document type added");
        break;
    case _ISLEM_GUNCELLE:
        QMessageBox::information(this,"info","recored updated");
        vtYenile();
        break;
    case _ISLEM_BASLANGIC:
        connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(btnAyrintiTiklandi(int)));
        toplamlariAl(toplamTutar);
        break;
    case _ISLEM_VENDOR_NAME:
        QMessageBox::information(this,"info","vendor names saved");
        break;
    case _ISLEM_DOCUMENT_TYPE:
        QMessageBox::information(this,"info","document types saved");
        break;
    case _ISLEM_SIL:
        QMessageBox::information(this,"info","record deleted");
        vtYenile();
        break;
    default:
        break;
    }
}

/**
 * @brief QtFtp::alanlariTemizle : kaydet butonuna tıklandıktan sonra alanlari temizler
 */
void QtFtp::alanlariTemizle()
{
    ui->txtFilePath->clear();
    ui->cbDocumentType->setCurrentIndex(0);
    ui->cbVendorName->setCurrentIndex(0);
    ui->btnTarih->setText(QDate::currentDate().toString(TARIH_FORMAT));
    ui->btnTarih2->setText(QDate::currentDate().toString(TARIH_FORMAT));
    ui->txtInvoiceNumber->clear();
    ui->txtTotalAmount->clear();
}

QtFtp::~QtFtp()
{
    delete vtThread;
    delete ui;
}
