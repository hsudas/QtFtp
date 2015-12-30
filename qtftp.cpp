#include "qtftp.h"
#include "ui_qtftp.h"

QtFtp::QtFtp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFtp)
{
    ui->setupUi(this);
}

QtFtp::~QtFtp()
{
    delete ui;
}
