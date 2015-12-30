#include "qtftp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFtp w;
    w.show();

    return a.exec();
}
