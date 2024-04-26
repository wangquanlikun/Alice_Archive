#ifndef PALS_H
#define PALS_H

#include <Qstring>
#include <vector>

#define SERVER_V_PALS_NUM 20

class Attribute{
public:
    enum Main{
        Strength, // 力量型：高攻击；
        Tank, // 肉盾型：高生命值；
        Defense, // 防御型：高防御；
        Agile // 敏捷型：低攻击间隔；
    };

    enum Sub{
        null
    };

    Main main_attribute;
    Sub sub_attribute;
};


class Pal{
private:
    Attribute attribute;

    int expNeed_for_levelup(){
        return level * 100 + 5 * level * level;
    }

    int expNeed_for_levelup(int pre_level){
        return pre_level * 100 + 5 * pre_level * pre_level;
    }

public:
    QString name;
    int level;
    int exp;
    int Attack_power;
    int Defense;
    int HP;
    int Attack_interval;
    virtual void Attack(){
        return;
    }

    bool levelUp(){
        if(level < 15){
            level++;
            switch(this->attribute.main_attribute){
            case Attribute::Strength:
                Attack_power = 150 + 2 * level * level + 25 * level;
                HP = 100 + 20 * level;
                Defense = 100 + 20 * level;
                Attack_interval = 50 - level;
                break;
            case Attribute::Tank:
                Attack_power = 100 + 20 * level;
                HP = 150 + 2 * level * level + 25 * level;
                Defense = 100 + 20 * level;
                Attack_interval = 50 - level;
                break;
            case Attribute::Defense:
                Attack_power = 100 + 20 * level;
                HP = 100 + 20 * level;
                Defense = 150 + 2 * level * level + 25 * level;
                Attack_interval = 50 - level;
                break;
            case Attribute::Agile:
                Attack_power = 100 + 20 * level;
                HP = 100 + 20 * level;
                Defense = 100 + 20 * level;
                Attack_interval = 35 - 2 * level;
                break;
            }
            return true;
        }
        else
            return false;
    }
    void getExp(const int exp){
        this->exp += exp;
        if(this->exp >= expNeed_for_levelup()){
            if(levelUp())
                this->exp -= expNeed_for_levelup(this->level - 1);
        }
    }

    void Initi_set(int type, QString name, int AP, int DP, int HP, int AI, int LV, int EXP);

    void set_attribute_int(const int main_type){
        switch(main_type){
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

    int get_attribute_int(){
        switch(this->attribute.main_attribute){
        case Attribute::Strength:
            return 1;
        case Attribute::Tank:
            return 2;
        case Attribute::Defense:
            return 3;
        case Attribute::Agile:
            return 4;
        }
    }
};


#endif // PALS_H
