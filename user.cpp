#include "user.h"
#include "pals.h"

void Pal::Initi_set(int type, QString name, int AP, int DP, int HP, int AI, int LV, int EXP){
    this->name = name;
    this->Attack_power = AP;
    this->Defense = DP;
    this->HP = HP;
    this->Attack_interval = AI;
    this->level = LV;
    this->exp = EXP;
    this->type = type;

    switch(type/10){
    case 1:
        this->attribute.main_attribute = Attribute::Strength;
        break;
    case 2:
        this->attribute.main_attribute = Attribute::Tank;
        break;
    case 3:
        this->attribute.main_attribute = Attribute::Defense;
        break;
    case 4:
        this->attribute.main_attribute = Attribute::Agile;
        break;
    }
}

void User_Data::erase_userdata(){
    this->userPals.clear();
    this->userPals.shrink_to_fit(); //清除元素的同时释放内存
    this->username.clear();
    this->petNum = -1;
    this->HighpetNum = -1;
    this->winNum = -1;
    this->failNum = -1;
}
