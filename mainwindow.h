#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
