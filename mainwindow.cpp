#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket.h"

#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Alice Archive"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enterGame_clicked()
{
    ui->Mainpage->setCurrentIndex(1);
}


void MainWindow::on_page2_to_page1_clicked()
{
    ui->Mainpage->setCurrentIndex(0);
}


void MainWindow::on_register_2_clicked()
{
    Packet packet;
    packet.clear();
    packet.getUserID(ui->username->text().toUtf8());
    packet.getPassword(ui->password->text().toUtf8());
    packet.getUserTry(Register_Try);

    if(!packet.isvaild_Password() || !packet.isvaild_UserID()){
        QMessageBox::about(this,"输入非法","用户名和密码仅能由字母和数字组成！  ");
    }
    else if(!packet.notempty()){
        QMessageBox::about(this,"输入非法","用户名或密码不能为空！  ");
    }
    else{
        /*
         * Socket通信
         *
         */
    }
}


void MainWindow::on_login_2_clicked()
{
    Packet packet;
    packet.clear();
    packet.getUserID(ui->username->text().toUtf8());
    packet.getPassword(ui->password->text().toUtf8());
    packet.getUserTry(Login_Try);

    if(!packet.isvaild_Password() || !packet.isvaild_UserID()){
        QMessageBox::about(this,"输入非法","用户名和密码仅能由字母和数字组成！  ");
    }
    else if(!packet.notempty()){
        QMessageBox::about(this,"输入非法","用户名或密码不能为空！  ");
    }
    else{
        /*
         * Socket通信
         *
         */
    }
}

