#include "ui_mainwindow.h"
#include <mainwindow.h>
#include <pals.h>
#include <user.h>

#include <QMessageBox>
#include <QPixmapCache>
extern std::map<QString, QString> ToPinyin;

#define WIN true
#define FAIL false

#include<QThread>
#include<QCoreApplication>

void MainWindow::Server_Pals_list_click(const QModelIndex index){
    int Server_V_Pals_array = index.row();
    choosed_fight_pal = Server_Pals_list[Server_V_Pals_array];

    QMessageBox msg;
    QString box_mes;
    box_mes += ("你选择了" + QString::number(Server_V_Pals_array + 1) + "号精灵：" + Server_Pals_list[Server_V_Pals_array].name + "\n");
    box_mes += ("LV: " + QString::number(Server_Pals_list[Server_V_Pals_array].level) + "  ");
    box_mes += ("HP: " + QString::number(Server_Pals_list[Server_V_Pals_array].HP) + "  ");
    box_mes += ("DP: " + QString::number(Server_Pals_list[Server_V_Pals_array].Defense) + "  ");
    box_mes += ("AP: " + QString::number(Server_Pals_list[Server_V_Pals_array].Attack_power) + "  ");
    box_mes += ("AI: " + QString::number(Server_Pals_list[Server_V_Pals_array].Attack_interval));
    msg.setText(box_mes);
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
            othersPal = new Strength_Pal(choosed_fight_pal);
            break;
        case 2:
            othersPal = new Tank_Pal(choosed_fight_pal);
            break;
        case 3:
            othersPal = new Defense_Pal(choosed_fight_pal);
            break;
        case 4:
            othersPal = new Agile_Pal(choosed_fight_pal);
            break;
        default:
            othersPal = NULL;
    }

    int round = 0;
    ui->fight_Round->display(round);
    ui->fight_Desc->clear();

    QMessageBox::about(this,"邦邦咔邦","对战开始");

    #define ENABLE_MESSAGEBOX 0

    while(true){
        QThread::msleep(20);
        QCoreApplication::processEvents();

        YourPal->interval ++;
        othersPal->interval ++;
        round ++;
        ui->fight_Round->display(round);
        YourPal->cost ++;
        othersPal->cost ++;
        ui->YourPal_cost->display(YourPal->cost);
        ui->othersPal_cost->display(othersPal->cost);

        Fight_info YourPal_Attack = YourPal->fight();
        Fight_info othersPal_GET_Attack = othersPal->fight(YourPal_Attack);
        if(othersPal_GET_Attack.Fight_info_output() != "-1"){
            ui->fight_Desc->append("Round: " + QString::number(round) + "\t" + othersPal_GET_Attack.Fight_info_output());
            #if ENABLE_MESSAGEBOX
                QMessageBox::about(this,"",othersPal_GET_Attack.Fight_info_output());
            #else
                QThread::msleep(200);
                QCoreApplication::processEvents();
            #endif
        }
        ui->othersPal_HP_ProgBar->setValue(fmax((int) ( ((double)othersPal->HP / (double)choosed_fight_pal.HP)*100 ) , 0));

        QThread::msleep(20);
        QCoreApplication::processEvents();

        Fight_info othersPal_Attack = othersPal->fight();
        Fight_info YourPal_GET_Attack = YourPal->fight(othersPal_Attack);
        if(YourPal_GET_Attack.Fight_info_output() != "-1"){
            ui->fight_Desc->append("Round: " + QString::number(round) + "\t" + YourPal_GET_Attack.Fight_info_output());
            #if ENABLE_MESSAGEBOX
                QMessageBox::about(this,"",YourPal_GET_Attack.Fight_info_output());
            #else
                QThread::msleep(200);
                QCoreApplication::processEvents();
            #endif
        }
        ui->YourPal_HP_ProgBar->setValue(fmax((int) ( ((double)YourPal->HP / (double)userdata.userPals[Now_pet - 1].HP)*100 ) , 0));

        if(YourPal->HP <= 0 || othersPal->HP <= 0)
            break;
    }

    QThread::msleep(200);
    QCoreApplication::processEvents();

    if(YourPal->HP < 0)
        ui->YourPal_HP_ProgBar->setValue(0);
    else
        ui->YourPal_HP_ProgBar->setValue((int) ( ((double)YourPal->HP / (double)userdata.userPals[Now_pet - 1].HP) * 100 ) );
    if(othersPal->HP < 0)
        ui->othersPal_HP_ProgBar->setValue(0);
    else
        ui->othersPal_HP_ProgBar->setValue((int) ( ((double)othersPal->HP / (double)choosed_fight_pal.HP) * 100 ) );

    ui->YourPal_cost->display(YourPal->cost);
    ui->othersPal_cost->display(othersPal->cost);
    #if ENABLE_MESSAGEBOX
    QMessageBox::about(this,"","战斗结束");
    #endif

    if(YourPal->HP > 0)
        return WIN;
    else
        return FAIL;
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
