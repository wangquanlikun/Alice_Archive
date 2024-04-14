#ifndef _SERVER_TCPSERVER_H
#define _SERVER_TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Database{
    public:
        Database();
        ~Database();
        QSqlDatabase data;
        QSqlQuery *DBquery;

        bool nameIndatabase(const QByteArray username);
};

class Executor_Data{
    private:
        QTcpSocket* socket;
        QByteArray socket_data;
        void server_register();
        void server_login();
        void server_logout();
        QString New_login_Random_Pals();
    public:
        Executor_Data(QTcpSocket* socket, QByteArray data){
            this->socket = socket;
            socket_data = data;
        }
        void server_executor();

        Database userDatabase;
};

#endif // _SERVER_TCPSERVER_H
