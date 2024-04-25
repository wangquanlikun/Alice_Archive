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

            /*精灵升级的时候，宠物对应的属性值会有少量增加（主属性增加量相对较多）*/

            return true;
        }
        else
            return false;
    }
    void getExp(const int exp){
        this->exp += exp;
        if(this->exp >= expNeed_for_levelup()){
            if(levelUp())
                this->exp -= expNeed_for_levelup();
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
