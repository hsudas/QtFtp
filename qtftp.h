#ifndef QTFTP_H
#define QTFTP_H

#include <QMainWindow>

namespace Ui {
class QtFtp;
}

class QtFtp : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtFtp(QWidget *parent = 0);
    ~QtFtp();

private:
    Ui::QtFtp *ui;
};

#endif // QTFTP_H
