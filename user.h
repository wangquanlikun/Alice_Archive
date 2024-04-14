#ifndef USER_H
#define USER_H

#include "pals.h"
#include <vector>
#include <QString>
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
    QString get_name(){
        return username;
    }
    void Write_userData(QByteArray buffer);
    void erase_userdata();
};

#endif // USER_H
