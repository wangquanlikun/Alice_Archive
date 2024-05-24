#ifndef _SERVER_MAINWINDOW_H
#define _SERVER_MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class _Server_MainWindow;
}
QT_END_NAMESPACE

class _Server_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    _Server_MainWindow(QWidget *parent = nullptr);
    ~_Server_MainWindow();

private slots:
    void on_Exit_Button_clicked();

private:
    Ui::_Server_MainWindow *ui;

    QTcpServer* server;
    std::vector<QTcpSocket *> socket_list;
    QString IP;
    int port;

    void ClientConnect();
    void ReadData();
};
#endif // _SERVER_MAINWINDOW_H
