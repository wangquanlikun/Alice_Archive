#include "ui_mainwindow.h"
#include <mainwindow.h>
#include <pals.h>
#include <user.h>

#include <QMessageBox>
#include <QPixmapCache>
extern std::map<QString, QString> ToPinyin;

#define WIN true
#define FAIL false

#include<thread>
#include<chrono>

void MainWindow::Server_Pals_list_click(const QModelIndex index){
    int Server_V_Pals_array = index.row();
    choosed_fight_pal = Server_Pals_list[Server_V_Pals_array];

    QMessageBox msg;
    msg.setText("你选择了" + QString::number(Server_V_Pals_array + 1) + "号精灵：" + Server_Pals_list[Server_V_Pals_array].name);
    msg.exec();
}

void MainWindow::on_choose_Fight_1_clicked(){ //决斗赛
    if(choosed_fight_pal.name == "NULL"){
        QMessageBox::warning(this,"无对战精灵","请点击列表选择虚拟决斗精灵！");
    }
    else{
        ui->Mainpage->setCurrentIndex(2);
        init_fight_page();
        if(fight()){
            QMessageBox::about(this,"邦邦咔邦","对战胜利！");
            userdata.winNum ++;
            //得到新精灵
        }
        else{
            QMessageBox::about(this,"苦呀西","对战失败了呜… 下次再努力吧");
            userdata.failNum ++;
            //失去此精灵
            //判断，若无精灵则给予一只精灵
        }

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this,"邦邦咔邦","战斗结束，要离开这里吗？", QMessageBox::Yes | QMessageBox::No);
        if(box == QMessageBox::Yes){
            ui->Mainpage->setCurrentIndex(4);
            refresh_personalPage();
        }
        else
            return;
    }
}

void MainWindow::on_choose_Fight_2_clicked(){ //升级赛
    if(choosed_fight_pal.name == "NULL"){
        QMessageBox::warning(this,"无对战精灵","请点击列表选择虚拟决斗精灵！");
    }
    else{
        ui->Mainpage->setCurrentIndex(2);
        init_fight_page();
        QMessageBox::about(this,"邦邦咔邦","对战开始");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(fight()){
            QMessageBox::about(this,"邦邦咔邦","对战胜利！");
            userdata.winNum ++;

            bool HighPet = false;
            if(this->userdata.userPals[Now_pet - 1].level == 15)
                HighPet = true;
            this->userdata.userPals[Now_pet - 1].getExp(100);

            if(this->userdata.userPals[Now_pet - 1].level == 15 && (!HighPet))
                this->userdata.HighpetNum ++;
        }
        else{
            QMessageBox::about(this,"苦呀西","对战失败了呜… 下次再努力吧");
            userdata.failNum ++;
        }

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this,"邦邦咔邦","战斗结束，要离开这里吗？", QMessageBox::Yes | QMessageBox::No);
        if(box == QMessageBox::Yes){
            ui->Mainpage->setCurrentIndex(4);
            refresh_personalPage();
        }
        else
            return;
    }
}

void MainWindow::init_fight_page(){
    Pal this_pal = this->userdata.userPals[Now_pet - 1];
    QPixmap pixmap_1;
    QString pix_path_1 = ":/new/prefix1/Resource/blue_archive_big_head_sd/" + ToPinyin[this_pal.name] + ".png";
    QPixmapCache::clear();
    pixmap_1.load(pix_path_1);
    ui->YourPal_IMG->setPixmap(pixmap_1);

    QPixmap pixmap_2;
    QString pix_path_2 = ":/new/prefix1/Resource/blue_archive_big_head_sd/" + ToPinyin[this->choosed_fight_pal.name] + ".png";
    QPixmapCache::clear();
    pixmap_2.load(pix_path_2);
    ui->othersPal_IMG->setPixmap(pixmap_2);

    ui->YourPal_HP_ProgBar->setValue(100);
    ui->othersPal_HP_ProgBar->setValue(100);
}

bool MainWindow::fight(){
    int YourPal_Attribute = this->userdata.userPals[Now_pet - 1].get_attribute_int();
    int othersPal_Attribute = choosed_fight_pal.get_attribute_int();

    Pal* YourPal;
    Pal* othersPal;
    //根据如上属性生成新的精灵类
    switch (YourPal_Attribute){
        case 1:
            YourPal = new Strength_Pal(userdata.userPals[Now_pet - 1]);
            break;
        case 2:
            YourPal = new Tank_Pal(userdata.userPals[Now_pet - 1]);
            break;
        case 3:
            YourPal = new Defense_Pal(userdata.userPals[Now_pet - 1]);
            break;
        case 4:
            YourPal = new Agile_Pal(userdata.userPals[Now_pet - 1]);
            break;
        default:
            YourPal = NULL;
    }

    switch (othersPal_Attribute){
        case 1:
            othersPal = new Strength_Pal(userdata.userPals[Now_pet - 1]);
            break;
        case 2:
            othersPal = new Tank_Pal(userdata.userPals[Now_pet - 1]);
            break;
        case 3:
            othersPal = new Defense_Pal(userdata.userPals[Now_pet - 1]);
            break;
        case 4:
            othersPal = new Agile_Pal(userdata.userPals[Now_pet - 1]);
            break;
        default:
            othersPal = NULL;
    }

/*
    while(true){ // 在双方血量不为零时持续攻击
        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        //双方经过攻击间隔的时间 ++
        YourPal->interval ++;
        othersPal->interval ++;

        //双方技能点 ++
        YourPal->cost ++;
        othersPal->cost ++;

        //内部自行判断是否给出普通攻击与技能。如果给出内部自行处理cost与interval
        //内部判断受击反馈

        Fight_info YourPal_Attack = YourPal->fight();
        Fight_info othersPal_GET_Attack = othersPal->fight(YourPal_Attack);
        //根据攻击与反馈写出到屏幕上 othersPal_GET_Attack.Fight_info_output();
        //血量更新

        Fight_info othersPal_Attack = othersPal->fight();
        Fight_info YourPal_GET_Attack = YourPal->fight(YourPal_Attack);
        //根据攻击与反馈写出到屏幕上 YourPal_GET_Attack.Fight_info_output();
        //血量更新

        //判断血量
        if(YourPal->HP == 0 || othersPal->HP == 0)
            break;
    }


    if(YourPal->HP != 0)
        return WIN;
    else
        return FAIL;
*/
    return true;
}

void MainWindow::refresh_personalPage(){
    ui->winNumLED->display(userdata.winNum);
    ui->failNumLED->display(userdata.failNum);

    if(userdata.petNum >= 30)
        ui->HONOR_1->setText("宠物个数勋章：金勋章");
    else if(userdata.petNum >= 20)
        ui->HONOR_1->setText("宠物个数勋章：银勋章");
    else if(userdata.petNum >= 10)
        ui->HONOR_1->setText("宠物个数勋章：铜勋章");
    else
        ui->HONOR_1->setText("宠物个数勋章：Null");

    if(userdata.HighpetNum >= 8)
        ui->HONOR_2->setText("高级宠物勋章：金勋章");
    else if(userdata.HighpetNum >= 5)
        ui->HONOR_2->setText("高级宠物勋章：银勋章");
    else if(userdata.HighpetNum >= 3)
        ui->HONOR_2->setText("高级宠物勋章：铜勋章");
    else
        ui->HONOR_2->setText("高级宠物勋章：Null");


    ItemModel_PetList->clear();
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

    change_now_pet(this->Now_pet);
}