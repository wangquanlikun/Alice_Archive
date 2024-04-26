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
    if(index.row() == 0){
        QMessageBox msg;
        msg.setText("请点击列表选择虚拟决斗精灵！");
        msg.exec();
    }
    else{
        int Server_V_Pals_array = index.row() - 1;
        choosed_fight_pal = Server_Pals_list[Server_V_Pals_array];
    }
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
            //得到新精灵
        }
        else{
            QMessageBox::about(this,"苦呀西","对战失败了呜… 下次再努力吧");
            //失去此精灵
            //判断，若无精灵则给予一只精灵
        }

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this,"邦邦咔邦","战斗结束，要离开这里吗？", QMessageBox::Yes | QMessageBox::No);
        if(box == QMessageBox::Yes){
            ui->Mainpage->setCurrentIndex(4);
            //刷新页面上用户状态
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
            this->userdata.userPals[Now_pet - 1].getExp(100);
        }
        else{
            QMessageBox::about(this,"苦呀西","对战失败了呜… 下次再努力吧");
        }

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this,"邦邦咔邦","战斗结束，要离开这里吗？", QMessageBox::Yes | QMessageBox::No);
        if(box == QMessageBox::Yes){
            ui->Mainpage->setCurrentIndex(4);
            this->change_now_pet(this->Now_pet); //刷新当前宠物数据
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

    //根据如上属性生成新的精灵类
/*
    while(true){ // 在双方血量不为零时持续攻击
        //双方经过攻击间隔的时间 ++
        //YourPal.interval ++;
        //othersPal.interval ++;

        //双方技能点 ++
        //YourPal.cost ++;
        //othersPal.cost ++;

        //内部自行判断是否给出普通攻击与技能。如果给出内部自行处理cost与interval
        //内部判断受击反馈

        //QString YourPal_Attack = YourPal.fight();
        //QString othersPal_GET_Attack othersPal.fight(YourPal_Attack);
        //根据攻击与反馈写出到屏幕上
        //血量更新

        //QString othersPal_Attack = othersPal.fight();
        //QString YourPal_GET_Attack YourPal.fight(othersPal_Attack);
        //根据攻击与反馈写出到屏幕上
        //血量更新

        //判断血量
    }
*/
/*
    if(YourPal.HP != 0)
        return WIN;
    else
        return FAIL;
*/

    return true;
}
