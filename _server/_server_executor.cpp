# include "_server_TcpServer.h"
# include <vector>
# include <time.h>

std::vector<QString> Online_userName;

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
    select += ( "'" + QString::fromUtf8(username) + "'" );
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
    else{
        if(socket_data == "userLogout")
            server_logout();
    }
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
    i++;
    for( ; i < this->socket_data.length(); i++)
        login_password += socket_data[i];

    qDebug() << "Login Try username: " + login_username + " password: " + login_password;

    QString select_all = "select * from player where name = '" + QString(login_username) + "' and password = '" +  QString(login_password) + "'";
    userDatabase.DBquery->prepare(select_all);
    userDatabase.DBquery->exec(select_all);

    if(userDatabase.DBquery->next()){ //用户名已存在且密码正确
        Online_userName.push_back(login_username);
        QString loginReturn = "LoginSuccess&";
        //还要返回用户信息
        QString winNum = userDatabase.DBquery->value(2).toString();
        QString failNum = userDatabase.DBquery->value(3).toString();
        QString petNum = userDatabase.DBquery->value(4).toString();
        QString highPetNum = userDatabase.DBquery->value(5).toString();
        QString pet = userDatabase.DBquery->value(6).toString();
        loginReturn += (winNum + " " + failNum + " " + petNum + " " + highPetNum + " " + pet);

        //返回所有用户与其精灵信息
        QString all_user_info;
        userDatabase.DBquery->prepare("SELECT name, pet FROM player");
        userDatabase.DBquery->exec();
        int user_num = 0;
        while(userDatabase.DBquery->next()){
            QString name = userDatabase.DBquery->value(0).toString();
            QString pet = userDatabase.DBquery->value(1).toString();
            all_user_info += (" " + name);
            user_num ++;
        }
        loginReturn += ("#" + QString::number(user_num) + all_user_info);

        socket->write(loginReturn.toUtf8());
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
    i++;
    for( ; i < this->socket_data.length(); i++)
        register_password += socket_data[i];

    if(userDatabase.nameIndatabase(register_username)){
        qDebug() << "UsernameIN";
        socket->write("UsernameIn&");
    }
    else {
        QString insert_sql = "insert into player values (?, ?, ?, ?, ?, ?, ?)";
        userDatabase.DBquery->prepare(insert_sql);

        qDebug() << "Register username: " + register_username + " password: " + register_password;
        Online_userName.push_back(register_username);

        userDatabase.DBquery->addBindValue(QString::fromUtf8(register_username));  //name
        userDatabase.DBquery->addBindValue(QString::fromUtf8(register_password));  //password
        userDatabase.DBquery->addBindValue(0);    //win num
        userDatabase.DBquery->addBindValue(0);    //fail num
        userDatabase.DBquery->addBindValue(30);    //pet num
        userDatabase.DBquery->addBindValue(0);    //high pet num

        //初始化与随机值用户信息
        QString registerReturn = "RegisterSuccess&0 0 30 0 ";
        QString random_pet_info = New_login_Random_Pals(30);
        userDatabase.DBquery->addBindValue(random_pet_info);    //pet information

        userDatabase.DBquery->exec();

        //还要返回用户信息
        registerReturn += random_pet_info;

        //返回所有用户与其精灵信息
        QString all_user_info;
        userDatabase.DBquery->prepare("SELECT name, pet FROM player");
        userDatabase.DBquery->exec();
        int user_num = 0;
        while(userDatabase.DBquery->next()){
            QString name = userDatabase.DBquery->value(0).toString();
            QString pet = userDatabase.DBquery->value(1).toString();
            all_user_info += (" " + name);
            user_num ++;
        }
        registerReturn += ("#" + QString::number(user_num) + all_user_info);

        socket->write(registerReturn.toUtf8());
    }
}

void Executor_Data::server_logout(){
    ;
}

QString Executor_Data::New_login_Random_Pals(int times){
    QString Random_Pals = "";
    srand((unsigned)time(NULL));
    for (int i = 0; i < times; i++){
        int type = rand() % 4 + 1;
        int subtype = rand() % 4 + 1;
        type = 10 * type + subtype;
        Random_Pals += QString::number(type);
        QString name;
        int AP = 100, DP = 100, HP = 100, AI = 50;
        switch(type){
            case 11:  //力量型
                AP = 150;
                name = "圣园弥香";
                break;
            case 12:
                AP = 150;
                name = "空崎阳奈";
                break;
            case 13:
                AP = 150;
                name = "天童爱丽丝";
                break;
            case 14:
                AP = 150;
                name = "白洲梓";
                break;
            case 21:  //肉盾型
                HP = 150;
                name = "优香";
                break;
            case 22:
                HP = 150;
                name = "小鸟游星野";
                break;
            case 23:
                HP = 150;
                name = "阿罗娜";
                break;
            case 24:
                HP = 150;
                name = "普拉娜";
                break;
            case 31:  //防御型
                DP = 150;
                name = "枣伊吕波";
                break;
            case 32:
                DP = 150;
                name = "阿慈谷日富美";
                break;
            case 33:
                DP = 150;
                name = "砂狼白子";
                break;
            case 34:
                DP = 150;
                name = "才羽桃井";
                break;
            case 41:  //敏捷型
                AI = 35;
                name = "才羽绿";
                break;
            case 42:
                AI = 35;
                name = "伊落玛丽";
                break;
            case 43:
                AI = 35;
                name = "下江小春";
                break;
            case 44:
                AI = 35;
                name = "花岗柚子";
                break;
        }
        Random_Pals += (" " + QString::number(AP) + " " + QString::number(DP) + " " + QString::number(HP) + " " + QString::number(AI) + " " + name + " "+"1 0 ");
    }

    return Random_Pals;
}
