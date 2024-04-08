#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket.h"
#include "executor.h"

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

    socket = new QTcpSocket(this);
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);

    IP = "127.0.0.1";
    port = 10043;

    register_success = false;
    login_success = false;
}

MainWindow::~MainWindow(){
    delete ui;
    delete socket;
}

void MainWindow::socket_disconnected(){
    socket->close();
}

void MainWindow::readData(){
    QByteArray buffer;
    buffer = socket->readAll();
    qDebug() << "[R] " + buffer;
    if(!buffer.isEmpty()){
        Instruction instruction;
        instruction.get_inst(buffer);
        int buf = instruction.executor(); //对发回的buffer消息进行后续处理的逻辑
        if(buf == USERNAMEIN){
            register_success = false;
            QMessageBox::warning(this,"注册失败","用户名已存在");
        }
        else if(buf == PASSWORDWRONG){
            login_success = false;
            QMessageBox::warning(this,"登陆失败","用户名或密码错误");
        }
        else if(buf == REGISTERSUCCESS){
            register_success = true;
            login_success = true;
            QMessageBox::about(this,"200","注册成功，现在登陆");
            ui->Mainpage->setCurrentIndex(4);
        }
        else if(buf == LOGINSUCCESS){
            register_success = true;
            login_success = true;
            ui->Mainpage->setCurrentIndex(4);
        }
    }
}

void MainWindow::on_enterGame_clicked(){
    socket->abort(); //取消已有的连接
    socket->connectToHost(IP, port);
    if(!socket->waitForConnected(3000)){
        QMessageBox::warning(this,"连接超时","请检查服务端是否正确配置  "+IP+":"+QString::number(port));
    }
    else{
        socket->write("200 Connection Established.");
        ui->Mainpage->setCurrentIndex(1);
    }
}


void MainWindow::on_page2_to_page1_clicked(){
    ui->Mainpage->setCurrentIndex(0);
}


void MainWindow::on_register_2_clicked(){
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
        socket->write(packet.trans_to_QByteArray());
    }
}


void MainWindow::on_login_2_clicked(){
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
        socket->write(packet.trans_to_QByteArray());
    }
}


void MainWindow::on_exitGame_clicked(){
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this,"邦邦咔邦","确定要离开吗？爱丽丝会想你的", QMessageBox::Yes | QMessageBox::No);
    if(box == QMessageBox::Yes){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        exit(0);
    }
    else
        return;
}


void MainWindow::on_About_returntomain_clicked(){
    ui->Mainpage->setCurrentIndex(0);
}


void MainWindow::on_page2_to_page4_clicked(){
    ui->Mainpage->setCurrentIndex(3);
}

