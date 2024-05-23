#ifndef USER_H
#define USER_H

#include "pals.h"
#include <vector>
#include <Qstring>
#include <QMainWindow>

class User_Data{
private:
    QString username;
public:
    int winNum;
    int failNum;
    int petNum;
    int HighpetNum;
    std::vector<Pal> userPals;

    void write_name(QString username){
        this->username = username;
    }
    QString get_name() const {
        return username;
    }
    void Write_userData(const QByteArray buffer);
    void erase_userdata();
};

class Registered_userdata{
public:
    QString username;
    int winNum;
    int failNum;
    int petNum;
    std::vector<QString> Pals_name;
    int online; //boolean
};

#endif // USER_H
