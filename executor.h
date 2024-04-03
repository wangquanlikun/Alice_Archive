#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QByteArray>
#include <QString>

#define USERNAMEIN 1
#define PASSWORDWRONG 2
#define LOGINSUCCESS 3

class Instruction{
    private:
        QByteArray instruction;
    public:
        Instruction(){
            instruction.clear();
        }

        void get_inst(QByteArray s){
            instruction.append(s);
        }

        QString get_head();

        int executor();
};

#endif // EXECUTOR_H