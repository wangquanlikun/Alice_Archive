#include "executor.h"
#include "user.h"
#include <QDebug>

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

void User_Data::Write_userData(QByteArray buffer){
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
