#include "mainwindow.h"
#include "executor.h"
#include "ui_mainwindow.h"
#include "user.h"
#include <QDebug>
#include <QRandomGenerator>

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
    }
}

void MainWindow::create_Server_Pals_list(){
    ItemModel_serverPetList = new QStandardItemModel(this);
    QStringList strList;
    Server_Pals_list.resize(SERVER_V_PALS_NUM);
    strList.append("    精灵列表：");

    for(int i = 0; i < SERVER_V_PALS_NUM; i++){
        int type;
        type = QRandomGenerator::global()->bounded(1, 4);
        type = 10 * type + QRandomGenerator::global()->bounded(1, 4);
        QString name;
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

        Server_Pals_list[i].set_attribute_int(type / 10);
        Server_Pals_list[i].name = name;
        Server_Pals_list[i].Attack_power = AP;
        Server_Pals_list[i].HP = HP;
        Server_Pals_list[i].Defense = DP;
        Server_Pals_list[i].Attack_interval = AI;
        Server_Pals_list[i].exp = 0;

        for(int i = 0; i < LV; i++)
            Server_Pals_list[i].levelUp();

        strList.append(name + "  LV:" + QString::number(LV));
    }

    int nCount = strList.size();
    for(int i = 0; i < nCount; i++){
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        ItemModel_serverPetList->appendRow(item);
    }
    this->ui->Server_Pals_list->setModel(ItemModel_serverPetList);
}

void MainWindow::creat_Regi_User_List(){
    ItemModel_RegiUserList = new QStandardItemModel(this);
    QStringList strList;
    strList.append("  注册用户用户名：");

    int regi_user_num = regi_userdata.size();
    for(int i = 0; i < regi_user_num; i++){
        strList.append(regi_userdata[i].username);
    }

    int nCount = strList.size();
    for(int i = 0; i < nCount; i++){
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        ItemModel_RegiUserList->appendRow(item);
    }
    this->ui->Userlist->setModel(ItemModel_RegiUserList);
}
