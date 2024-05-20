#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QtWidgets/QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QLabel>
#include <QMouseEvent>

#define BGM_ON 1
#define CHARACTER_VOICE 1
#include <QSoundEffect>

#include "user.h"

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
    void on_logout_clicked();
    void Pal_list_click(QModelIndex index);
    void Userlist_click(QModelIndex index);
    void on_choose_Fight_1_clicked();
    void on_choose_Fight_2_clicked();
    void Server_Pals_list_click(const QModelIndex index);

    void readData();
    void socket_disconnected();

    void Init_pinyin();

    void on_refresh_Virtual_Pals_clicked();
    void on_exit_fight_clicked();
    void on_leave_fight_run_clicked();
    void onLabelDoubleClicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *socket;
    QString IP;
    int port;

    bool register_success;
    bool login_success;

    User_Data userdata;
    std::vector<Pal> Server_Pals_list;
    void create_Server_Pals_list();
    int Now_pet;
    void window_personalPage();
    QStandardItemModel *ItemModel_PetList;
    QStandardItemModel *ItemModel_serverPetList;
    void change_now_pet(int Now_pet);

    std::vector<Registered_userdata> regi_userdata;
    void write_regi_userdata(const QByteArray buffer);
    QStandardItemModel *ItemModel_RegiUserList;
    void creat_Regi_User_List();

    Pal choosed_fight_pal;
    void init_fight_page();
    bool fight();
    bool leave_fight;
    void refresh_personalPage();
    int choose_throw_pal();

    QSoundEffect * startSound;
    QSoundEffect * fightSound;
    QSoundEffect * Alice_Fail;
    QSoundEffect * Alice_FightStart;
    QSoundEffect * Alice_Levelup;
    QSoundEffect * Alice_Welcome;
    QSoundEffect * Alice_Win;
    QSoundEffect * Alice_Leave;
};

class Click_Lable : public QLabel {
Q_OBJECT
    public:
        explicit Click_Lable(QWidget *parent = nullptr) : QLabel(parent) {}
    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override {
            if (event->button() == Qt::LeftButton) {
                emit doubleClicked();
            }
            QLabel::mouseDoubleClickEvent(event);
        }
    signals:
        void doubleClicked();
};
#endif // MAINWINDOW_H
