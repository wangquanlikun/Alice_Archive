# include "_server_TcpServer.h"

Database::Database(){
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        data = QSqlDatabase::database("qt_sql_default_connection");
    }
    else {
        data = QSqlDatabase::addDatabase("QSQLITE");
        data.setDatabaseName("AliceArchive.db");
        data.setUserName("Arisu");
        data.setPassword("114514");
    }

    if (!data.open()) {
        qDebug() << "Failed to connect the database.";
    }
    else {
        DBquery = new QSqlQuery(data);
        QString createSql = "CREATE TABLE player (name TEXT primary key, password TEXT, winNum INT, failNum INT, petNum INT, highPetNum INT, pet TEXT);";
        DBquery->prepare(createSql);
        DBquery->exec();
    }
}

Database::~Database(){
    data.close();
}

bool Database::nameIndatabase(const QByteArray username){
    QString select = "select * from player where name = ";
    select += QString(username);
    if(!DBquery->exec(select)){
        qDebug() << DBquery->lastError();
        return false;
    }
    else{
        if(DBquery->next()){  //用户名已存在
            qDebug() << "Username is in the table. Username: " + username;
            return true;
        }
        else{  //用户名不存在
            qDebug() << "Username is not in table. Username: " + username;
            return false;
        }
    }
}

void Executor_Data::server_executor(){
    if(socket_data[0] == 'r')
        server_register();
    else if(socket_data[0] == 'l')
        server_login();
}

void Executor_Data::server_login(){
    QByteArray login_username;
    QByteArray login_password;
    int i;
    for(i = 1; i < 16; i++){
        if(socket_data[i] == '&')
            break;
        else
            login_username += socket_data[i];
    }
    for( ; i < this->socket_data.length(); i++)
        login_password += socket_data[i];

    QString select_all = "select * from player where name = " + QString(login_username) + " and password = " +  QString(login_password);
    userDatabase.DBquery->prepare(select_all);
    userDatabase.DBquery->exec(select_all);

    if(userDatabase.DBquery->next()){ //用户名已存在且密码正确

        //还要返回用户信息
        socket->write("LoginSuccess&");
    }
    else {
        socket->write("PasswordWrong&");
    }
}

void Executor_Data::server_register(){
    QByteArray register_username;
    QByteArray register_password;
    int i;
    for(i = 1; i < 16; i++){
        if(socket_data[i] == '&')
            break;
        else
            register_username += socket_data[i];
    }
    for( ; i < this->socket_data.length(); i++)
        register_password += socket_data[i];

    if(userDatabase.nameIndatabase(register_username)){
        qDebug() << "UsernameIN";
        socket->write("UsernameIn&");
    }
    else {
        QString insert_sql = "insert into player values (?, ?, ?, ?, ?, ?, ?)";
        userDatabase.DBquery->prepare(insert_sql);

        userDatabase.DBquery->addBindValue(QString(register_username));  //name
        userDatabase.DBquery->addBindValue(QString(register_password));  //password
        userDatabase.DBquery->addBindValue(0);    //win num
        userDatabase.DBquery->addBindValue(0);    //fail num
        userDatabase.DBquery->addBindValue(3);    //pet num

        //初始化与随机值用户信息
        userDatabase.DBquery->addBindValue(0);    //high pet num
        userDatabase.DBquery->addBindValue(0);    //pet information

        userDatabase.DBquery->exec();

        //还要返回用户信息
        socket->write("RegisterSuccess&");
    }
}
