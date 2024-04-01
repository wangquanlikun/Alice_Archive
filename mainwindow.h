#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStackedWidget Mainpage;

private slots:
    void on_enterGame_clicked();

    void on_page2_to_page1_clicked();

    void on_register_2_clicked();

    void on_login_2_clicked();

    void on_exitGame_clicked();

    void on_About_returntomain_clicked();

    void on_page2_to_page4_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *socket;
    QString IP;
    int port;
};
#endif // MAINWINDOW_H
