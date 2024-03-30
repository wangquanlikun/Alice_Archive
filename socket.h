#ifndef SOCKET_H
#define SOCKET_H

#include <QByteArray>

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
        bool isvaild_UserID(){
            int length = UserID.length();
            for(int i = 0; i < length; i++){
                if((UserID[i] >= 'a' && UserID[i] <= 'z') || (UserID[i] >= 'A' && UserID[i] <= 'Z') || (UserID[i] >= '0' && UserID[i] <= '9'))
                    continue;
                else
                    return false;
            }
            return true;
        }

        bool isvaild_Password(){
            int length = Password.length();
            for(int i = 0; i < length; i++){
                if((Password[i] >= 'a' && Password[i] <= 'z') || (Password[i] >= 'A' && Password[i] <= 'Z') || (Password[i] >= '0' && Password[i] <= '9'))
                    continue;
                else
                    return false;
            }
            return true;
        }

        bool notempty(){
            return (UserID.length() > 0 ) && (Password.length() > 0);
        }

        void clear(){
            UserID.clear();
            Password.clear();
        }

        void getUserID(QByteArray s){
            UserID.append(s);
        }
        QByteArray getUserID(){
            return UserID;
        }

        void getPassword(QByteArray s){
            Password.append(s);
        }
        QByteArray getPassword(){
            return Password;
        }

        void getUserTry(int x){
            if(x == 0)
                usertry = _login;
            else
                usertry = _register;
        }

        int getUserTry(){
            if(usertry == _login)
                return 0;
            else
                return 1;
        }
};

#endif // SOCKET_H
