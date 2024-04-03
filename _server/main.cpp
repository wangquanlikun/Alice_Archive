#include "_server_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    _Server_MainWindow w;
    w.show();
    return a.exec();
}
