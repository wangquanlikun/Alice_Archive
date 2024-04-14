#include "user.h"
#include "pals.h"

void Pal::Initi_set(int type, QString name, int AP, int DP, int HP, int AI, int LV, int EXP){
    this->name = name;
    this->level = 1;
    this->exp = 0;
    this->Attack_power = AP;
    this->Defense = DP;
    this->HP = HP;
    this->Attack_interval = AI;
    this->level = LV;
    this->exp = EXP;

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
