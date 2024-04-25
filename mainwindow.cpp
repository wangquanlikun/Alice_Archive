#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket.h"
#include "executor.h"

#include<QMessageBox>
#include<QPixmapCache>
#include<thread>
#include<chrono>

#include<map>
extern std::map<QString, QString> ToPinyin;

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

    ui->Mainpage->setCurrentIndex(0);

    Now_pet = 1;
    Init_pinyin();
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
            userdata.Write_userData(buffer);
            write_regi_userdata(buffer);
            window_personalPage();
        }
        else if(buf == LOGINSUCCESS){
            register_success = true;
            login_success = true;
            ui->Mainpage->setCurrentIndex(4);
            userdata.Write_userData(buffer);
            write_regi_userdata(buffer);
            window_personalPage();
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
    userdata.write_name(ui->username->text().toUtf8());
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
    userdata.write_name(ui->username->text().toUtf8());
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


void MainWindow::on_logout_clicked(){
    socket->write("userLogout");
    //要写回用户信息更改吗
    ui->Mainpage->setCurrentIndex(1);
}

void MainWindow::window_personalPage(){
    ui->Welcome_username->setText(userdata.get_name());
    ui->winNumLED->display(userdata.winNum);
    ui->failNumLED->display(userdata.failNum);

    if(userdata.petNum >= 30)
        ui->HONOR_1->setText("宠物个数勋章：金勋章");
    else if(userdata.petNum >= 20)
        ui->HONOR_1->setText("宠物个数勋章：银勋章");
    else if(userdata.petNum >= 10)
        ui->HONOR_1->setText("宠物个数勋章：铜勋章");
    else
        ui->HONOR_1->setText("宠物个数勋章：Null");

    if(userdata.HighpetNum >= 8)
        ui->HONOR_2->setText("高级宠物勋章：金勋章");
    else if(userdata.HighpetNum >= 5)
        ui->HONOR_2->setText("高级宠物勋章：银勋章");
    else if(userdata.HighpetNum >= 3)
        ui->HONOR_2->setText("高级宠物勋章：铜勋章");
    else
        ui->HONOR_2->setText("高级宠物勋章：Null");

    create_Server_Pals_list();
    creat_Regi_User_List();

    ItemModel_PetList = new QStandardItemModel(this);
    QStringList strList;
    strList.append("\t姓名列表：");
    for (int i = 0; i < this->userdata.petNum; i++){
        strList.append(this->userdata.userPals[i].name);
    }
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++){
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        ItemModel_PetList->appendRow(item);
    }
    ui->Pals_list->setModel(ItemModel_PetList);
    connect(ui->Pals_list,SIGNAL(clicked(QModelIndex)),this,SLOT(Pal_list_click(QModelIndex)));
    change_now_pet(1);
}

void MainWindow::Pal_list_click(QModelIndex index){
    this->Now_pet = index.row();
    if(Now_pet != 0){
        change_now_pet(Now_pet);
        QMessageBox msg;
        msg.setText("你选择了"+QString::number(Now_pet)+"号精灵！");
        msg.exec();
    }
    else {
        Now_pet = 1;
        QMessageBox msg;
        msg.setText("点击列表查看精灵详细信息");
        msg.exec();
    }
}
void MainWindow::change_now_pet(int Now_pet){
    Pal this_pal = this->userdata.userPals[Now_pet - 1];
    ui->this_Pal_name->setText(this_pal.name);
    ui->this_Pal_LV->display(this_pal.level);
    ui->this_Pal_Exp->display(this_pal.exp);
    ui->this_Pal_AP->setValue(this_pal.Attack_power);
    ui->this_Pal_DP->setValue(this_pal.Defense);
    ui->this_Pal_HP->setValue(this_pal.HP);
    ui->this_Pal_AI->setValue(this_pal.Attack_interval);
    QPixmap pixmap;
    QString pix_path = ":/new/prefix1/Resource/blue_archive_big_head_sd/" + ToPinyin[this_pal.name] + ".png";
    switch(this_pal.get_attribute_int()){
    case 1:
        ui->this_Pal_attribute->setText("力量型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    case 2:
        ui->this_Pal_attribute->setText("肉盾型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    case 3:
        ui->this_Pal_attribute->setText("防御型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    case 4:
        ui->this_Pal_attribute->setText("敏捷型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    }
}
