#include "ui_mainwindow.h"
#include <mainwindow.h>
#include <pals.h>
#include <user.h>

#include <QMessageBox>
#include <QInputDialog>
#include <QPixmapCache>
extern std::map<QString, QString> ToPinyin;

#define WIN true
#define FAIL false

#include<QThread>
#include<QCoreApplication>

#include <QPropertyAnimation>

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
            userdata.userPals.push_back(choosed_fight_pal);
            userdata.petNum ++;
            QString New_Pal_info;
            New_Pal_info += ("获得新精灵：" + choosed_fight_pal.name + "  LV. " + QString::number(choosed_fight_pal.level));
            QMessageBox::about(this,"获得新精灵！",New_Pal_info);
        }
        else{
            QMessageBox::about(this,"苦呀西","对战失败了呜… 下次再努力吧");
            userdata.failNum ++;
            QString Thrown_Pal_info;
            int thrown_Pal_index = choose_throw_pal();
            Thrown_Pal_info += ("失去了精灵：" + userdata.userPals[thrown_Pal_index - 1].name);
            QMessageBox::warning(this,"失去了精灵！",Thrown_Pal_info);
            userdata.userPals.erase( userdata.userPals.begin() + (thrown_Pal_index - 1) );
            userdata.petNum --;
            if(Now_pet > userdata.petNum)
                Now_pet --;
            if(userdata.petNum == 0){ //判断，若无精灵则给予一只精灵
                Pal new_pal;

                int type;
                type = QRandomGenerator::global()->bounded(1, 5);
                type = 10 * type + QRandomGenerator::global()->bounded(1, 5);
                QString name;
                int LV = 1;
                int AP = 100, HP = 100, DP = 100, AI = 50;
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
                new_pal.Initi_set(type, name, AP, DP, HP, AI, LV, 0);

                userdata.userPals.push_back(new_pal);
                userdata.petNum ++;
                Now_pet = 1;

                QString get_new_pal = "杂鱼♡~杂鱼~ 没有精灵可太可怜了！来，带上这个吧\n\n  获得精灵：" + new_pal.name + " LV. 1";
                QMessageBox::about(this,"哼！才不是给你的精灵呢", get_new_pal);
            }
        }

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this,"邦邦咔邦","战斗结束，要离开这里吗？", QMessageBox::Yes | QMessageBox::No);
        if(box == QMessageBox::Yes){
            #if BGM_ON
                this->startSound->play();
                this->fightSound->stop();
            #endif

            ui->Mainpage->setCurrentIndex(4);
            refresh_personalPage();
        }
        else{
            ui->exit_fight->show();
            return;
        }
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
            #if BGM_ON
                this->startSound->play();
                this->fightSound->stop();
            #endif

            ui->Mainpage->setCurrentIndex(4);
            refresh_personalPage();
        }
        else{
            ui->exit_fight->show();
            return;
        }
    }
}

void MainWindow::init_fight_page(){
    #if BGM_ON
        this->startSound->stop();
        this->fightSound->play();
    #endif

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

    ui->Yourpal_S_AP->hide();
    ui->Yourpal_S_AP_back->hide();
    ui->Yourpal_S_HP->hide();
    ui->Yourpal_S_HP_back->hide();
    ui->Yourpal_S_cost->hide();
    ui->Yourpal_S_cost_back->hide();
    ui->Yourpal_S_down->hide();
    ui->Yourpal_S_down_back->hide();
    ui->Yourpal_S_tank->hide();
    ui->Yourpal_S_tank_back->hide();
    ui->Yourpal_S_up->hide();
    ui->Yourpal_S_up_back->hide();

    ui->otherspal_S_AP->hide();
    ui->otherspal_S_AP_back->hide();
    ui->otherspal_S_HP->hide();
    ui->otherspal_S_HP_back->hide();
    ui->otherspal_S_cost->hide();
    ui->otherspal_S_cost_back->hide();
    ui->otherspal_S_down->hide();
    ui->otherspal_S_down_back->hide();
    ui->otherspal_S_tank->hide();
    ui->otherspal_S_tank_back->hide();
    ui->otherspal_S_up->hide();
    ui->otherspal_S_up_back->hide();

    ui->exit_fight->hide();

    ui->Yourpal_death->hide();
    ui->otherspal_death->hide();
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

    QPropertyAnimation YourPal_anim((ui->YourPal_IMG), "geometry");
    YourPal_anim.setDuration(100);
    YourPal_anim.setLoopCount(3);
    YourPal_anim.setEasingCurve(QEasingCurve::OutBounce);
    QPropertyAnimation othersPal_anim((ui->othersPal_IMG), "geometry");
    othersPal_anim.setDuration(100);
    othersPal_anim.setLoopCount(3);
    othersPal_anim.setEasingCurve(QEasingCurve::OutBounce);

    //设回动画效果初始位置
    ui->YourPal_IMG->move(160, 90);
    ui->othersPal_IMG->move(880, 90);
    const auto YourPal_geometry = ui->YourPal_IMG->geometry();
    const auto othersPal_geometry = ui->othersPal_IMG->geometry();
    QObject::connect(&YourPal_anim, &QPropertyAnimation::finished, [&]() {
        ui->YourPal_IMG->setGeometry(YourPal_geometry);
    });
    QObject::connect(&othersPal_anim, &QPropertyAnimation::finished, [&]() {
        ui->othersPal_IMG->setGeometry(othersPal_geometry);
    });

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
            if(YourPal->get_attribute_int() == 1){
                ;
            }
            else if(YourPal->get_attribute_int() == 2){
                //无效技能 || 抵消攻击 || 伤害减少 || 防御增加
                if(YourPal_Attack.special_power_desc == "0&22" || YourPal_Attack.special_power_desc == "0&24" || YourPal_Attack.special_power_desc == "0&23" || YourPal_Attack.special_power_desc == "0&21"){
                    ui->Yourpal_S_up->show();
                    ui->Yourpal_S_up_back->show();
                }
                else{
                    ui->Yourpal_S_up->hide();
                    ui->Yourpal_S_up_back->hide();
                }
            }
            else if(YourPal->get_attribute_int() == 3){
                //虎丸坦克
                if((YourPal_Attack.special_power_desc.length() == 5 && YourPal_Attack.special_power_desc[4] == 'x') || (YourPal_Attack.special_power_desc != "" && YourPal_Attack.special_power_desc[0] == 'x')){
                    ui->Yourpal_S_tank->show();
                    ui->Yourpal_S_tank_back->show();
                }
                else{
                    ui->Yourpal_S_tank->hide();
                    ui->Yourpal_S_tank_back->hide();
                }
                //抵消攻击 || 防御增加
                if(YourPal_Attack.special_power_desc == "0&32" || YourPal_Attack.special_power_desc == "0&32x" || YourPal_Attack.special_power_desc == "0&33" || YourPal_Attack.special_power_desc == "0&33x" || YourPal_Attack.special_power_desc == "0&34" || YourPal_Attack.special_power_desc == "0&34x"){
                    ui->Yourpal_S_up->show();
                    ui->Yourpal_S_up_back->show();
                }
                else{
                    ui->Yourpal_S_up->hide();
                    ui->Yourpal_S_up_back->hide();
                }
            }
            else if(YourPal->get_attribute_int() == 4){
                //生命值
                if(YourPal_Attack.special_power_desc == "0&42"){
                    ui->Yourpal_S_HP->show();
                    ui->Yourpal_S_HP_back->show();
                }
                else{
                    ui->Yourpal_S_HP->hide();
                    ui->Yourpal_S_HP_back->hide();
                }
                //cost
                if(YourPal_Attack.special_power_desc == "0&43"){
                    ui->Yourpal_S_cost->show();
                    ui->Yourpal_S_cost_back->show();
                }
                else{
                    ui->Yourpal_S_cost->hide();
                    ui->Yourpal_S_cost_back->hide();
                }
            }

            othersPal_anim.setStartValue(ui->othersPal_IMG->geometry());
            QRect endValue = ui->othersPal_IMG->geometry();
            endValue.translate(5, 5);
            othersPal_anim.setEndValue(endValue);
            othersPal_anim.start();

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
            if(othersPal->get_attribute_int() == 1){
                ;
            }
            else if(othersPal->get_attribute_int() == 2){
                //无效技能 || 抵消攻击 || 伤害减少 || 防御增加
                if(othersPal_Attack.special_power_desc == "0&22" || othersPal_Attack.special_power_desc == "0&24" || othersPal_Attack.special_power_desc == "0&23" || othersPal_Attack.special_power_desc == "0&21"){
                    ui->otherspal_S_up->show();
                    ui->otherspal_S_up_back->show();
                }
                else{
                    ui->otherspal_S_up->hide();
                    ui->otherspal_S_up_back->hide();
                }
            }
            else if(othersPal->get_attribute_int() == 3){
                //虎丸坦克
                if((othersPal_Attack.special_power_desc.length() == 5 && othersPal_Attack.special_power_desc[4] == 'x') || (othersPal_Attack.special_power_desc != "" && othersPal_Attack.special_power_desc[0] == 'x')){
                    ui->otherspal_S_tank->show();
                    ui->otherspal_S_tank_back->show();
                }
                else{
                    ui->otherspal_S_tank->hide();
                    ui->otherspal_S_tank_back->hide();
                }
                //抵消攻击 || 防御增加
                if(othersPal_Attack.special_power_desc == "0&32" || othersPal_Attack.special_power_desc == "0&32x" || othersPal_Attack.special_power_desc == "0&33" || othersPal_Attack.special_power_desc == "0&33x" || othersPal_Attack.special_power_desc == "0&34" || othersPal_Attack.special_power_desc == "0&34x"){
                    ui->otherspal_S_up->show();
                    ui->otherspal_S_up_back->show();
                }
                else{
                    ui->otherspal_S_up->hide();
                    ui->otherspal_S_up_back->hide();
                }
            }
            else if(othersPal->get_attribute_int() == 4){
                //生命值
                if(othersPal_Attack.special_power_desc == "0&42"){
                    ui->otherspal_S_HP->show();
                    ui->otherspal_S_HP_back->show();
                }
                else{
                    ui->otherspal_S_HP->hide();
                    ui->otherspal_S_HP_back->hide();
                }
                //cost
                if(othersPal_Attack.special_power_desc == "0&43"){
                    ui->otherspal_S_cost->show();
                    ui->otherspal_S_cost_back->show();
                }
                else{
                    ui->otherspal_S_cost->hide();
                    ui->otherspal_S_cost_back->hide();
                }
            }

            YourPal_anim.setStartValue(ui->YourPal_IMG->geometry());
            QRect endValue = ui->YourPal_IMG->geometry();
            endValue.translate(5, 5);
            YourPal_anim.setEndValue(endValue);
            YourPal_anim.start();

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

    QObject::disconnect(&YourPal_anim, &QPropertyAnimation::finished, nullptr, nullptr);
    QObject::disconnect(&othersPal_anim, &QPropertyAnimation::finished, nullptr, nullptr);

    if(YourPal->HP <= 0)
        ui->Yourpal_death->show();
    if(othersPal->HP <= 0)
        ui->otherspal_death->show();

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

int MainWindow::choose_throw_pal(){
    int choosed_pal_index = Now_pet;
    std::vector<int> pre_choose_pal_index;
    pre_choose_pal_index.resize(fmin(3, userdata.petNum));
    for(int i = 0; i < fmin(3, userdata.petNum); i++){
        re_rand:
        int temp_random = QRandomGenerator::global()->bounded(1, userdata.petNum + 1);
        for(int j = 0; j < i; j++){
            if(temp_random == pre_choose_pal_index[j])
                goto re_rand;
        }
        pre_choose_pal_index[i] = temp_random;
    }

    std::map<QString, int> name_to_index;
    QString TEXT_choose_to_throw_pal_1;
    QString TEXT_choose_to_throw_pal_2 = "";
    TEXT_choose_to_throw_pal_1 = "请选择一个精灵送出";
    QStringList items;
    for(int i = 0; i < (int)pre_choose_pal_index.size(); i++){
        TEXT_choose_to_throw_pal_2 += QString::number(i + 1) + ". 精灵名称：" + userdata.userPals[pre_choose_pal_index[i] - 1].name;
        TEXT_choose_to_throw_pal_2 += "   LV. " + QString::number(userdata.userPals[pre_choose_pal_index[i] - 1].level);
        TEXT_choose_to_throw_pal_2 += "\n";
        items << userdata.userPals[pre_choose_pal_index[i] - 1].name;
        name_to_index[userdata.userPals[pre_choose_pal_index[i] - 1].name] = i;
    }
    bool ok;
    QString item = QInputDialog::getItem(nullptr, TEXT_choose_to_throw_pal_1, TEXT_choose_to_throw_pal_2, items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        qDebug() << "送出的精灵：" + item;
        choosed_pal_index = pre_choose_pal_index[name_to_index[item]];
    }
    return choosed_pal_index;
}

void MainWindow::on_exit_fight_clicked(){
    #if BGM_ON
        this->startSound->play();
        this->fightSound->stop();
    #endif

    ui->Mainpage->setCurrentIndex(4);
    refresh_personalPage();
}
