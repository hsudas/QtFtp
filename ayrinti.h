#ifndef AYRINTI_H
#define AYRINTI_H

#include <QDialog>
#include <QMessageBox>

#include "config.h"
#include "takvim.h"
#include "vtthread.h"

namespace Ui {
class Ayrinti;
}

class Ayrinti : public QDialog
{
    Q_OBJECT

public:
    explicit Ayrinti(VtThread *vt, QWidget *parent = 0);
    ~Ayrinti();
    void setlistVendorName(QStringList a){listVendorName = a;}
    void setlistDocumentType(QStringList a){listDocumentType = a;}
    void setID(QString a){id = a;}
    void setvendorName(QString a){vendorName = a;}
    void setdocumentType(QString a){documentType = a;}
    void setsaveDate(QString a){saveDate = a;}
    void setinvoiceDate(QString a){invoiceDate = a;}
    void setamount(QString a){amount = a;}
    void setinvoiceNumber(QString a){invoiceNumber = a;}
    void setfilePath(QString a){filePath = a;}
    void doldur();

public slots:
    void btnTarihTiklandi(bool);
    void btnKaydetTiklandi(bool);
    void btnIptalTiklandi(bool);
    void btnSilTiklandi(bool);
    void takvimKapandi(QDate, QPushButton *);

private:
    Ui::Ayrinti *ui;
    QString id;
    QStringList listDocumentType;
    QStringList listVendorName;
    QString vendorName;
    QString documentType;
    QString saveDate;
    QString invoiceDate;
    QString amount;
    QString invoiceNumber;
    QString filePath;
    VtThread *vt;
};

#endif // AYRINTI_H
