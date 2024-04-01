#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket.h"

#include<QMessageBox>
#include<thread>
#include<chrono>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Alice Archive"));
    setWindowIcon(QIcon(":/new/prefix1/Resource/AliceArchive_ICO.ico"));

    socket = new QTcpSocket();
    // QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    // QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);

    // ui->userPetPic_1->setScaledContents(true);

    IP = "127.0.0.1";
    port = 6666;
    // nowPet = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
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
        QMessageBox::warning(this,"输入非法","用户名和密码仅能由字母和数字组成！  ");
    }
    else if(!packet.notempty()){
        QMessageBox::warning(this,"输入非法","用户名或密码不能为空！  ");
    }
    else{
        /*
         * Socket通信
         * 进入页面3
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
        QMessageBox::warning(this,"输入非法","用户名和密码仅能由字母和数字组成！  ");
    }
    else if(!packet.notempty()){
        QMessageBox::warning(this,"输入非法","用户名或密码不能为空！  ");
    }
    else{
        /*
         * Socket通信
         * 进入页面3
         */
    }
}


void MainWindow::on_exitGame_clicked()
{
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this,"邦邦咔邦","确定要离开吗？爱丽丝会想你的", QMessageBox::Yes | QMessageBox::No);
    if(box == QMessageBox::Yes){
        std::this_thread::sleep_for(std::chrono::microseconds(200));
        exit(0);
    }
    else
        return;
}


void MainWindow::on_About_returntomain_clicked()
{
    ui->Mainpage->setCurrentIndex(0);
}


void MainWindow::on_page2_to_page4_clicked()
{
    ui->Mainpage->setCurrentIndex(3);
}

