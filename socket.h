#ifndef SOCKET_H
#define SOCKET_H

#include <QtWidgets/QWidget>
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#define Login_Try 0
#define Register_Try 1

class Packet{
    private:
        enum UserTry{
                _register,
                _login
            };

        UserTry usertry;
        QByteArray UserID;
        QByteArray Password;
    public:
        bool isvaild_UserID() const {
            const int length = UserID.length();
            for(int i = 0; i < length; i++){
                if((UserID[i] >= 'a' && UserID[i] <= 'z') || (UserID[i] >= 'A' && UserID[i] <= 'Z') || (UserID[i] >= '0' && UserID[i] <= '9'))
                    continue;
                else
                    return false;
            }
            return true;
        }

        bool isvaild_Password() const {
            const int length = Password.length();
            for(int i = 0; i < length; i++){
                if((Password[i] >= 'a' && Password[i] <= 'z') || (Password[i] >= 'A' && Password[i] <= 'Z') || (Password[i] >= '0' && Password[i] <= '9'))
                    continue;
                else
                    return false;
            }
            return true;
        }

        bool notempty() const {
            return (UserID.length() > 0 ) && (Password.length() > 0);
        }

        bool usernameTooShort() const {
            return (UserID.length() <= 2);
        }

        void clear(){
            UserID.clear();
            Password.clear();
        }

        void getUserID(const QByteArray s){
            UserID.append(s);
        }
        QByteArray getUserID() const {
            return UserID;
        }

        void getPassword(const QByteArray s){
            Password.append(s);
        }
        QByteArray getPassword() const {
            return Password;
        }

        void getUserTry(const int x){
            if(x == 0)
                usertry = _login;
            else
                usertry = _register;
        }

        int getUserTry() const {
            if(usertry == _login)
                return 0;
            else
                return 1;
        }

        QByteArray trans_to_QByteArray() const {
            QByteArray temp;
            if(usertry == _login)
                temp.append("l");
            else
                temp.append("r");

            temp.append(UserID);
            temp.append("&");
            temp.append(Password);
            return temp;
        }
};

#endif // SOCKET_H
