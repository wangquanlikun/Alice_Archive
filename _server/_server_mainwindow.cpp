#include "_server_mainwindow.h"
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
        socket = server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &_Server_MainWindow::ReadData);
    }
}

void _Server_MainWindow::ReadData(){
    QByteArray buf = socket->readAll();//readAll最多接收65532的数据
    ui->text_Debug_Info->append("[R] " + QString(buf));

    /*
     *
     * 后续逻辑处理
     *
     * */
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
