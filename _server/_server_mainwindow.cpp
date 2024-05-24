#include "_server_mainwindow.h"
#include "_server_TcpServer.h"
#include "ui__server_mainwindow.h"

#include<QMessageBox>

_Server_MainWindow::_Server_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::_Server_MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Alice Archive Server"));

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &_Server_MainWindow::ClientConnect);

    port = 10043;
    server->listen(QHostAddress::Any, port);

    ui->text_Info->setText("localhost:" + QString::number(port));
}

_Server_MainWindow::~_Server_MainWindow()
{
    delete ui;
    server->close();
    server->deleteLater();
}

void _Server_MainWindow::ClientConnect(){
    while (server->hasPendingConnections()){
        QTcpSocket * socket;
        socket = server->nextPendingConnection();
        this->socket_list.push_back(socket);
        connect(socket, &QTcpSocket::readyRead, this, &_Server_MainWindow::ReadData);
    }
}

void _Server_MainWindow::ReadData(){
    QTcpSocket * socket = qobject_cast<QTcpSocket *>(sender());
    QByteArray buf = socket->readAll(); //readAll接收发来的数据
    ui->text_Debug_Info->append("[R] " + QString(buf));
    //后续逻辑处理
    Executor_Data executor(socket, buf);
    executor.server_executor();
}

void _Server_MainWindow::on_Exit_Button_clicked(){
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this,"邦邦咔邦","确定要结束服务吗？爱丽丝会想你的", QMessageBox::Yes | QMessageBox::No);
    if(box == QMessageBox::Yes){
        exit(0);
    }
    else
        return;
}
