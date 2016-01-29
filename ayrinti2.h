#ifndef AYRINTI2_H
#define AYRINTI2_H

#include <QDialog>
#include "vtthread.h"

namespace Ui
{
class Ayrinti2;
}

class Ayrinti2 : public QDialog
{
    Q_OBJECT

public:
    explicit Ayrinti2(VtThread *vt, QWidget *parent = 0);
    ~Ayrinti2();
    void doldur(QStringList);
    void closeEvent(QCloseEvent *);
    void setISLEM(int i){ISLEM = i;}

public slots:
    void btnEkleTiklandi(bool);
    void btnCikarTiklandi(bool);
    void btnKaydetTiklandi(bool);
    void btnIptalTiklandi(bool);

private:
    Ui::Ayrinti2 *ui;
    VtThread *vt;
    int ISLEM;
};

#endif // AYRINTI2_H
