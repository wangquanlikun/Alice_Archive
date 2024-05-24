#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket.h"
#include "executor.h"

#include<QMessageBox>
#include<QPixmapCache>
#include<thread>
#include<chrono>
#include<QLayout>
#include<QPainter>

QPixmap MainWindow::setPixmapOpacity(const QPixmap &src, qreal opacity) {
    QPixmap dest(src.size());
    dest.fill(Qt::transparent);
    QPainter painter(&dest);
    painter.setOpacity(opacity);
    painter.drawPixmap(0, 0, src);
    painter.end();
    return dest;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Alice Archive"));
    setWindowIcon(QIcon(":/new/prefix1/Resource/AliceArchive_ICO.ico"));

    socket = new QTcpSocket(this);
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);

    IP = "127.0.0.1";
    port = 10043;
    on_online_state = false;
    register_success = false;
    login_success = false;

    ui->Mainpage->setCurrentIndex(0);

    connect(ui->Pals_list,SIGNAL(clicked(QModelIndex)),this,SLOT(Pal_list_click(QModelIndex)));
    connect(ui->Server_Pals_list,SIGNAL(clicked(QModelIndex)),this,SLOT(Server_Pals_list_click(QModelIndex)));
    connect(ui->Userlist,SIGNAL(clicked(QModelIndex)),this,SLOT(Userlist_click(QModelIndex)));

    Now_pet = 1;
    choosed_fight_pal.name = "NULL";
    Init_pinyin();

    #if BGM_ON
        startSound = new QSoundEffect(this);
        startSound->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Constant Moderato.wav"));
        startSound->setLoopCount(QSoundEffect::Infinite); //设置循环次数
        startSound->play();
        startSound->setVolume(0.2);

        fightSound = new QSoundEffect(this);
        fightSound->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Mechanical JUNGLE 2.wav"));
        fightSound->setLoopCount(QSoundEffect::Infinite);
        fightSound->setVolume(0.2);
    #endif

    #if CHARACTER_VOICE
        Alice_Fail = new QSoundEffect(this);
        Alice_Fail->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Alice_Fail.wav"));
        Alice_Fail->setLoopCount(1);
        Alice_FightStart = new QSoundEffect(this);
        Alice_FightStart->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Alice_FightStart.wav"));
        Alice_FightStart->setLoopCount(1);
        Alice_Levelup = new QSoundEffect(this);
        Alice_Levelup->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Alice_Levelup.wav"));
        Alice_Levelup->setLoopCount(1);
        Alice_Welcome = new QSoundEffect(this);
        Alice_Welcome->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Alice_Welcome.wav"));
        Alice_Welcome->setLoopCount(1);
        Alice_Win = new QSoundEffect(this);
        Alice_Win->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Alice_Win.wav"));
        Alice_Win->setLoopCount(1);
        Alice_Leave = new QSoundEffect(this);
        Alice_Leave->setSource(QUrl::fromLocalFile(":/new/prefix1/Resource/BGM/Alice_Leave.wav"));
        Alice_Leave->setLoopCount(1);
    #endif

    //添加点击小精灵头像处的虚拟Click_Lable元素
    QWidget *page = ui->Mainpage->widget(4);
    Click_Lable * click_pixmap_show_power_desc = new Click_Lable(page);
    click_pixmap_show_power_desc->setGeometry(60 + 440, 370 + 30, 240, 240);
    connect(click_pixmap_show_power_desc, &Click_Lable::doubleClicked, this, &MainWindow::onLabelDoubleClicked);
    click_pixmap_show_power_desc->setStyleSheet("Click_Lable { border: 2px solid rgba(0, 0, 0, 100); border-radius: 10px;}");

    //背景图片：透明度
    QPixmap pixmap(":/new/prefix1/Resource/personal_background.png");
    qreal opacity = 0.2;
    QPixmap transparentPixmap = setPixmapOpacity(pixmap, opacity);
    ui->person_background->setPixmap(transparentPixmap);
}

MainWindow::~MainWindow(){
    delete ui;
    delete socket;
}

void MainWindow::socket_disconnected(){
    socket->close();
}

void MainWindow::readData(){
    QByteArray buffer;
    buffer = socket->readAll();
    qDebug() << "[R] " + buffer;
    if(!buffer.isEmpty() && !on_online_state){
        Instruction instruction;
        instruction.get_inst(buffer);
        int buf = instruction.executor(); //对发回的buffer消息进行后续处理的逻辑
        if(buf == USERNAMEIN){
            register_success = false;
            QMessageBox::warning(this,"注册失败","用户名已存在");
        }
        else if(buf == PASSWORDWRONG){
            login_success = false;
            QMessageBox::warning(this,"登陆失败","用户名或密码错误");
        }
        else if(buf == USERONLINE){
            login_success = false;
            QMessageBox::warning(this,"登陆失败","无法重复登录账户");
        }
        else if(buf == REGISTERSUCCESS){
            register_success = true;
            login_success = true;
            on_online_state = true;
            #if CHARACTER_VOICE
                Alice_Welcome->play();
            #endif
            QMessageBox::about(this,"200","注册成功，现在登陆");
            ui->Mainpage->setCurrentIndex(4);
            userdata.Write_userData(buffer);
            write_regi_userdata(buffer);
            window_personalPage();
        }
        else if(buf == LOGINSUCCESS){
            register_success = true;
            login_success = true;
            on_online_state = true;
            #if CHARACTER_VOICE
                Alice_Welcome->play();
            #endif
            ui->Mainpage->setCurrentIndex(4);
            userdata.Write_userData(buffer);
            write_regi_userdata(buffer);
            window_personalPage();
        }
    }
}

void MainWindow::on_enterGame_clicked(){
    socket->abort(); //取消已有的连接
    socket->connectToHost(IP, port);
    if(!socket->waitForConnected(3000)){
        QMessageBox::warning(this,"连接超时","请检查服务端是否正确配置  "+IP+":"+QString::number(port));
    }
    else{
        socket->write("200 Connection Established.");
        ui->Mainpage->setCurrentIndex(1);
    }
}


void MainWindow::on_page2_to_page1_clicked(){
    ui->Mainpage->setCurrentIndex(0);
}


void MainWindow::on_register_2_clicked(){
    Packet packet;
    packet.clear();
    packet.getUserID(ui->username->text().toUtf8());
    userdata.write_name(ui->username->text().toUtf8());
    packet.getPassword(ui->password->text().toUtf8());
    packet.getUserTry(Register_Try);

    if(!packet.isvaild_Password() || !packet.isvaild_UserID()){
        QMessageBox::warning(this,"输入非法","用户名和密码仅能由字母和数字组成！  ");
    }
    else if(!packet.notempty()){
        QMessageBox::warning(this,"输入非法","用户名或密码不能为空！  ");
    }
    else if(packet.usernameTooShort()){
        QMessageBox::warning(this,"用户名过短","用户名请大于2个字符！  ");
    }
    else{
        socket->write(packet.trans_to_QByteArray());
    }
}


void MainWindow::on_login_2_clicked(){
    Packet packet;
    packet.clear();
    packet.getUserID(ui->username->text().toUtf8());
    userdata.write_name(ui->username->text().toUtf8());
    packet.getPassword(ui->password->text().toUtf8());
    packet.getUserTry(Login_Try);

    if(!packet.isvaild_Password() || !packet.isvaild_UserID()){
        QMessageBox::warning(this,"输入非法","用户名和密码仅能由字母和数字组成！  ");
    }
    else if(!packet.notempty()){
        QMessageBox::warning(this,"输入非法","用户名或密码不能为空！  ");
    }
    else if(packet.usernameTooShort()){
        QMessageBox::warning(this,"用户名过短","用户名请大于2个字符！  ");
    }
    else{
        socket->write(packet.trans_to_QByteArray());
    }
}


void MainWindow::on_exitGame_clicked(){
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this,"邦邦咔邦","确定要离开吗？爱丽丝会想你的", QMessageBox::Yes | QMessageBox::No);
    if(box == QMessageBox::Yes){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        exit(0);
    }
    else
        return;
}


void MainWindow::on_About_returntomain_clicked(){
    ui->Mainpage->setCurrentIndex(0);
}


void MainWindow::on_page2_to_page4_clicked(){
    ui->Mainpage->setCurrentIndex(3);
}


void MainWindow::on_logout_clicked(){
    QString Send_logout_data;
    Send_logout_data = "O&" + userdata.get_name();
    Send_logout_data += " " + QString::number(userdata.winNum) + " " + QString::number(userdata.failNum) + " " + QString::number(userdata.petNum) + " " + QString::number(userdata.HighpetNum);
    Send_logout_data += "#";
    for(int i = 0; i < userdata.petNum; i++){
        Send_logout_data += QString::number(userdata.userPals[i].type) + " ";
        Send_logout_data += QString::number(userdata.userPals[i].Attack_power) + " ";
        Send_logout_data += QString::number(userdata.userPals[i].Defense) + " ";
        Send_logout_data += QString::number(userdata.userPals[i].HP) + " ";
        Send_logout_data += QString::number(userdata.userPals[i].Attack_interval) + " ";
        Send_logout_data += userdata.userPals[i].name + " ";
        Send_logout_data += QString::number(userdata.userPals[i].level) + " ";
        Send_logout_data += QString::number(userdata.userPals[i].exp) + " ";
    }
    socket->write(Send_logout_data.toUtf8());
    on_online_state = false;
    this->userdata.erase_userdata();
    #if CHARACTER_VOICE
        Alice_Leave->play();
    #endif
    ui->Mainpage->setCurrentIndex(1);
}

void MainWindow::window_personalPage(){
    ui->Welcome_username->setText(userdata.get_name());
    ui->winNumLED->display(userdata.winNum);
    ui->failNumLED->display(userdata.failNum);

    QPixmap pixmap;
    if(userdata.petNum >= 30){
        ui->HONOR_1->setText("宠物个数勋章：金勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_S.png");
        ui->HONOR_IMG_1->setPixmap(pixmap);
    }
    else if(userdata.petNum >= 20){
        ui->HONOR_1->setText("宠物个数勋章：银勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_A.png");
        ui->HONOR_IMG_1->setPixmap(pixmap);
    }
    else if(userdata.petNum >= 10){
        ui->HONOR_1->setText("宠物个数勋章：铜勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_B.png");
        ui->HONOR_IMG_1->setPixmap(pixmap);
    }
    else{
        ui->HONOR_1->setText("宠物个数勋章：无勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_D.png");
        ui->HONOR_IMG_1->setPixmap(pixmap);
    }

    if(userdata.HighpetNum >= 8){
        ui->HONOR_2->setText("高级宠物勋章：金勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_S.png");
        ui->HONOR_IMG_2->setPixmap(pixmap);
    }
    else if(userdata.HighpetNum >= 5){
        ui->HONOR_2->setText("高级宠物勋章：银勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_A.png");
        ui->HONOR_IMG_2->setPixmap(pixmap);
    }
    else if(userdata.HighpetNum >= 3){
        ui->HONOR_2->setText("高级宠物勋章：铜勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_B.png");
        ui->HONOR_IMG_2->setPixmap(pixmap);
    }
    else{
        ui->HONOR_2->setText("高级宠物勋章：无勋章");
        QPixmapCache::clear();
        pixmap.load(":/new/prefix1/Resource/BA_TerRat_D.png");
        ui->HONOR_IMG_2->setPixmap(pixmap);
    }

    create_Server_Pals_list();
    creat_Regi_User_List();

    ItemModel_PetList = new QStandardItemModel(this);
    QStringList labels = QStringLiteral("精灵,等级").simplified().split(",");
    ItemModel_PetList->setHorizontalHeaderLabels(labels);
    QStandardItem* item = 0;

    for (int i = 0; i < this->userdata.petNum; i++){
        item = new QStandardItem(userdata.userPals[i].name);
        ItemModel_PetList->setItem(i, 0, item);
        item = new QStandardItem(QString::number(userdata.userPals[i].level));
        ItemModel_PetList->setItem(i, 1, item);
    }
    ui->Pals_list->setModel(ItemModel_PetList);
    ui->Pals_list->setColumnWidth(0, 120);
    ui->Pals_list->setColumnWidth(1, 60);
    ui->Pals_list->setSelectionBehavior(QTableView::SelectRows);

    change_now_pet(1);
}

void MainWindow::Pal_list_click(QModelIndex index){
    this->Now_pet = index.row() + 1;
    change_now_pet(Now_pet);
    QMessageBox msg;
    msg.setText("你选择了"+QString::number(Now_pet)+"号精灵！");
    msg.exec();
}
void MainWindow::change_now_pet(int Now_pet){
    Pal this_pal = this->userdata.userPals[Now_pet - 1];
    ui->this_Pal_name->setText(this_pal.name);
    ui->this_Pal_LV->display(this_pal.level);
    ui->this_Pal_Exp->display(this_pal.exp);
    ui->this_Pal_AP->setValue(this_pal.Attack_power);
    ui->this_Pal_DP->setValue(this_pal.Defense);
    ui->this_Pal_HP->setValue(this_pal.HP);
    ui->this_Pal_AI->setValue(this_pal.Attack_interval);
    QPixmap pixmap;
    QString pix_path = ":/new/prefix1/Resource/blue_archive_big_head_sd/" + ToPinyin[this_pal.name] + ".png";
    switch(this_pal.get_attribute_int()){
    case 1:
        ui->this_Pal_attribute->setText("力量型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    case 2:
        ui->this_Pal_attribute->setText("肉盾型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    case 3:
        ui->this_Pal_attribute->setText("防御型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    case 4:
        ui->this_Pal_attribute->setText("敏捷型");
        QPixmapCache::clear();
        pixmap.load(pix_path);
        ui->this_Pal_img->setPixmap(pixmap);
        break;
    }
}

void MainWindow::onLabelDoubleClicked(){
    QMessageBox msg;
    QString msg_text;
    switch(userdata.userPals[Now_pet - 1].get_attribute_int()){
    case 1:
        msg_text += (QString::fromStdString("力量型精灵") + "\n\n");
        msg_text += QString::fromStdString("1. 瓦尼瓦尼：精确狙击，造成 2 * 攻击力 的伤害；有 25% 的概率暴击，共计造成 3 * 攻击力 的伤害\n\n") +
                    "2. 光啊：光之剑充能，造成 3 * 攻击力 的伤害\n\n" +
                    "3. 格黑娜的风纪委员：造成 2 * 攻击力 的伤害，同时使下一次攻击造成的攻击值增加 50% ，持续一回合\n\n" +
                    "4. 少女们的垂怜经：造成随机为 1 * 攻击力 与 3 * 攻击力 之间的伤害";
        break;
    case 2:
        msg_text += (QString::fromStdString("肉盾型精灵") + "\n\n");
        msg_text += QString::fromStdString("1. 千年的100kg小姐：怪力攻击，造成 2 * 攻击力 的伤害；同时下一次受击伤害减少 50% \n\n") +
                    "2. 蓝色恶魔：无效化对手下一次技能 -- 对手下一次抽卡必不出彩\n\n" +
                    "3. 一动不动还是好热啊~ ：造成 2 * 攻击力 的伤害；同时增加 50% 的防御力，持续一回合\n\n" +
                    "4. 老师大人的责任：「色彩」老师将会出现，抵消对方下一次攻击";
        break;
    case 3:
        msg_text += (QString::fromStdString("防御型精灵") + "\n\n");
        msg_text += QString::fromStdString("1. 虎丸！出击：召唤虎丸坦克，继承 25% 的生命值与 100% 的防御力，在被击破前吸收所有伤害。同时其每 15 攻击间隔 造成 200% 攻击力 的伤害\n\n") +
                    "2. 佩洛洛Sama：召唤佩洛洛人偶，抵消对方下一次攻击\n\n" +
                    "3. 苦呀西~ ：增加 100% 的防御力，持续 4 回合\n\n" +
                    "4. 银行不妙曲：立刻造成随机值为 攻击力 与 2 * 攻击力 之间的伤害，同时防御增加 50% ，持续一回合";
        break;
    case 4:
        msg_text += (QString::fromStdString("敏捷型精灵") + "\n\n");
        msg_text += QString::fromStdString("1. 游戏开发部的秘籍：可以在本次攻击间隔内，立即发动一次普通攻击\n\n") +
                    "2. H的事情不行！：当血量低于 50% 时，可以立即回复 20% 的生命值；否则对对方造成一次 攻击力 的伤害\n\n" +
                    "3. 对玛导致昏：立即增加 20 的 攻击间隔 与 10 的 技能点 \n\n" +
                    "4. UZQueen：造成 2 * 攻击力 的伤害";
        break;
    }
    msg.setWindowTitle("精灵技能描述");
    msg.setText(msg_text);
    msg.exec();
}
