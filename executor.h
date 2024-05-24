#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QByteArray>
#include <Qstring>

#define USERNAMEIN 1
#define PASSWORDWRONG 2
#define REGISTERSUCCESS 3
#define LOGINSUCCESS 4
#define USERONLINE 5

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

        QString get_head() const;

        int executor() const;
};

#endif // EXECUTOR_H
