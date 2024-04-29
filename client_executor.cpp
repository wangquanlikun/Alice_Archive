#include "mainwindow.h"
#include "executor.h"
#include "ui_mainwindow.h"
#include "user.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QMessageBox>

QString Instruction::get_head(){
    QString buf;
    int index = instruction.indexOf('&');
    if (index != -1) {
        buf = QString::fromUtf8(instruction.left(index));
    }
    return buf;
}

int Instruction::executor(){
    QString head = this->get_head();
    if(head == "UsernameIn"){
        return USERNAMEIN;
    }
    else if(head == "PasswordWrong"){
        return PASSWORDWRONG;
    }
    else if(head == "RegisterSuccess"){
        return REGISTERSUCCESS;
    }
    else if(head == "LoginSuccess"){
        return LOGINSUCCESS;
    } //对于登录注册
    else{

    }
    return 0;
}

void User_Data::Write_userData(const QByteArray buffer){
    int pos = buffer.indexOf('&');
    QByteArray cache = buffer.mid(pos + 1);
    QTextStream stream(&cache);
    stream >> this->winNum >> this->failNum >> this->petNum >> this->HighpetNum;
    userPals.resize(petNum);
    for (int i = 0; i < petNum; i++){
        int type, AP, DP, HP, AI, LV, EXP;
        QString name;
        stream >> type >> AP >> DP >> HP >> AI >> name >> LV >> EXP;
        userPals[i].Initi_set(type, name, AP, DP, HP, AI, LV, EXP);
    }
}

void MainWindow::write_regi_userdata(const QByteArray buffer){
    int pos = buffer.indexOf('#');
    QByteArray cache = buffer.mid(pos + 1);
    QTextStream stream(&cache);

    int regi_user_num;
    stream >> regi_user_num;
    char c = ' ';
    stream >> c; //读取注册用户数量后面的一个空格
    this->regi_userdata.resize(regi_user_num);
    for(int i = 0; i < regi_user_num; i++){
        stream >> this->regi_userdata[i].username;

        int petnum;
        stream >> petnum;
        this->regi_userdata[i].Pals_name.resize(petnum);
        this->regi_userdata[i].petNum = petnum;

        for(int j = 0; j < petnum; j++){
            int LV;
            stream >> this->regi_userdata[i].Pals_name[j] >> LV;
            this->regi_userdata[i].Pals_name[j] += ( "  LV." + QString::number(LV));
        }

        stream >> this->regi_userdata[i].winNum >> this->regi_userdata[i].failNum;
    }
}

void MainWindow::create_Server_Pals_list(){
    ItemModel_serverPetList = new QStandardItemModel(this);
    QStringList labels = QStringLiteral("精灵,属性,等级").simplified().split(",");
    ItemModel_serverPetList->setHorizontalHeaderLabels(labels);

    QStandardItem* item = 0;

    Server_Pals_list.resize(SERVER_V_PALS_NUM);
    for(int i = 0; i < SERVER_V_PALS_NUM; i++){
        int type;
        type = QRandomGenerator::global()->bounded(1, 4);
        type = 10 * type + QRandomGenerator::global()->bounded(1, 4);
        QString name;
        QString str_type;
        int LV;
        int LV_Temp = QRandomGenerator::global()->bounded(15);
        LV = LV_Temp %4 != 0 ? QRandomGenerator::global()->bounded(1, 15) : 1;
        Server_Pals_list[i].level = 1;
        int AP = 100, HP = 100, DP = 100, AI = 50;
        switch(type){
        case 11:  //力量型
            AP = 150;
            name = "圣园弥香";
            break;
        case 12:
            AP = 150;
            name = "空崎阳奈";
            break;
        case 13:
            AP = 150;
            name = "天童爱丽丝";
            break;
        case 14:
            AP = 150;
            name = "白洲梓";
            break;
        case 21:  //肉盾型
            HP = 150;
            name = "优香";
            break;
        case 22:
            HP = 150;
            name = "小鸟游星野";
            break;
        case 23:
            HP = 150;
            name = "阿罗娜";
            break;
        case 24:
            HP = 150;
            name = "普拉娜";
            break;
        case 31:  //防御型
            DP = 150;
            name = "枣伊吕波";
            break;
        case 32:
            DP = 150;
            name = "阿慈谷日富美";
            break;
        case 33:
            DP = 150;
            name = "砂狼白子";
            break;
        case 34:
            DP = 150;
            name = "才羽桃井";
            break;
        case 41:  //敏捷型
            AI = 35;
            name = "才羽绿";
            break;
        case 42:
            AI = 35;
            name = "伊落玛丽";
            break;
        case 43:
            AI = 35;
            name = "下江小春";
            break;
        case 44:
            AI = 35;
            name = "花岗柚子";
            break;
        }

        switch(type /10){
        case 1:
            str_type = "力量型";
            break;
        case 2:
            str_type = "肉盾型";
            break;
        case 3:
            str_type = "防御型";
            break;
        case 4:
            str_type = "敏捷型";
            break;
        }

        Server_Pals_list[i].set_attribute_int(type / 10);
        Server_Pals_list[i].name = name;
        Server_Pals_list[i].Attack_power = AP;
        Server_Pals_list[i].HP = HP;
        Server_Pals_list[i].Defense = DP;
        Server_Pals_list[i].Attack_interval = AI;
        Server_Pals_list[i].exp = 0;

        for(int i = 0; i < LV; i++)
            Server_Pals_list[i].levelUp();

        item = new QStandardItem(name);
        ItemModel_serverPetList->setItem(i, 0, item);
        item = new QStandardItem(str_type);
        ItemModel_serverPetList->setItem(i, 1, item);
        item = new QStandardItem(QString::number(LV));
        ItemModel_serverPetList->setItem(i, 2, item);
    }
    this->ui->Server_Pals_list->setModel(ItemModel_serverPetList);
    ui->Server_Pals_list->setColumnWidth(0, 120);
    ui->Server_Pals_list->setColumnWidth(2, 55);
}

void MainWindow::creat_Regi_User_List(){
    ItemModel_RegiUserList = new QStandardItemModel(this);
    QStringList labels = QStringLiteral("用户名,状态").simplified().split(",");
    ItemModel_RegiUserList->setHorizontalHeaderLabels(labels);

    QStandardItem* item = 0;

    int regi_user_num = regi_userdata.size();
    for(int i = 0; i < regi_user_num; i++){
        item = new QStandardItem(regi_userdata[i].username);
        ItemModel_RegiUserList->setItem(i, 0, item);
        QString Online_status;
        if(regi_userdata[i].online == 1)
            Online_status = "在线";
        else
            Online_status = "离线";
        item = new QStandardItem(Online_status);
        ItemModel_RegiUserList->setItem(i, 1, item);
    }

    this->ui->Userlist->setModel(ItemModel_RegiUserList);
    ui->Userlist->setColumnWidth(0, 150);
    ui->Userlist->setColumnWidth(1, 80);
}

void MainWindow::Userlist_click(QModelIndex index){
    QMessageBox msg;
    QString message_test;
    message_test += "你选择的用户名称为：" + this->regi_userdata[index.row()].username + "\n";
    int winNum = this->regi_userdata[index.row()].winNum;
    int failNum = this->regi_userdata[index.row()].failNum;
    float winRate = -1;
    if(winNum + failNum != 0)
        winRate = (float)winNum / (float)(winNum + failNum);
    QString S_winRate;
    if(winRate == -1)
        S_winRate = "NA";
    else
        S_winRate = QString::number((int)(winRate*100));
    message_test += "用户胜利 " + QString::number(winNum) + " 场，失败 " + QString::number(failNum) + " 场，胜率为 " + S_winRate + "% \n";
    message_test += "用户拥有 " + QString::number(this->regi_userdata[index.row()].petNum) + " 个精灵" + "\n";
    message_test += "用户精灵为：\n";
    for(int i = 0; i < this->regi_userdata[index.row()].petNum; i++){
        message_test += "\t";
        message_test += this->regi_userdata[index.row()].Pals_name[i];
        message_test += "\n";
    }
    msg.setText(message_test);
    msg.exec();
}
