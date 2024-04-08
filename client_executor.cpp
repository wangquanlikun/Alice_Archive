#include "executor.h"

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
