#ifndef PALS_H
#define PALS_H

#include <Qstring>
#include <QRandomGenerator>

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

class Fight_info{
public:
    int Attack_power;
    QString special_power_name;
    QString special_power;

    int real_HP_loss;
    QString special_status;

    Fight_info(){
        Attack_power = 0;
        special_power_name = "";
        special_power = "";
        real_HP_loss = 0;
        special_status = "";
    }

    QString Fight_info_output(){
        QString output;

        return output;
    }
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

    int interval;
    int cost;

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

    virtual Fight_info fight(){
        Fight_info attack_info;
        return attack_info;
    } //内部自行判断是否给出普通攻击与技能。如果给出内部自行处理cost与interval

    virtual Fight_info fight(Fight_info attack_infomation){
        Fight_info get_attack_info;
        return get_attack_info;
    } //内部判断受击结果，自行处理HP等。给出反馈

};

class Strength_Pal: public Pal{
    private:
        int special_power_choice;
        int special_power[3 + 1];
        int special_power_cost[3 + 1];
        QString special_power_name[3 + 1];

    public:
        Strength_Pal(Pal Base_Pal){
            interval = 0;
            cost = 0;

            name = Base_Pal.name;
            level = Base_Pal.level;
            exp = Base_Pal.exp;
            Attack_power = Base_Pal.Attack_power;
            Defense = Base_Pal.Defense;
            HP = Base_Pal.HP;
            Attack_interval = Base_Pal.Attack_interval;
            set_attribute_int(Base_Pal.get_attribute_int());
            special_power_choice = -1;

            special_power[1] = 2 * Attack_power;
            special_power[2] = 4 * Attack_power;
            special_power[3] = 2 * Attack_power;

            special_power_cost[1] = 20;
            special_power_cost[2] = 35;
            special_power_cost[3] = 30;

            special_power_name[1] = "瓦尼瓦尼";
            special_power_name[2] = "光啊！";
            special_power_name[3] = "格黑娜的风纪委员";
        }

        Fight_info fight(){
            Fight_info attack_info;
            if(interval >= Attack_interval){
                attack_info.Attack_power += Attack_power;
                interval -= Attack_interval;
            }

            if(special_power_choice == -1){
                special_power_choice = QRandomGenerator::global()->bounded(1, 3);
            }
            else{
                switch(special_power_choice){
                case 1:
                    if(cost >= special_power_cost[1]){
                        cost -= special_power_cost[1];
                        attack_info.Attack_power += special_power[1];
                        attack_info.special_power_name = special_power_name[1];

                        special_power_choice = -1;
                    }
                    break;
                case 2:
                    if(cost >= special_power_cost[2]){
                        cost -= special_power_cost[2];
                        attack_info.Attack_power += special_power[2];
                        attack_info.special_power_name = special_power_name[2];

                        special_power_choice = -1;
                    }
                    break;
                case 3:
                    if(cost >= special_power_cost[3]){
                        cost -= special_power_cost[3];
                        attack_info.Attack_power += special_power[3];
                        attack_info.special_power_name = special_power_name[3];

                        special_power_choice = -1;
                    }
                    break;
                }
            }
            return attack_info;
        }

        Fight_info fight(Fight_info attack_infomation){
            Fight_info get_attack_info;
            return get_attack_info;
        }
};

class Tank_Pal: public Pal{
public:
    Tank_Pal(Pal Base_Pal){
        interval = 0;
        cost = 0;

        name = Base_Pal.name;
        level = Base_Pal.level;
        exp = Base_Pal.exp;
        Attack_power = Base_Pal.Attack_power;
        Defense = Base_Pal.Defense;
        HP = Base_Pal.HP;
        Attack_interval = Base_Pal.Attack_interval;
        set_attribute_int(Base_Pal.get_attribute_int());
    }

    Fight_info fight(){
        Fight_info attack_info;
        return attack_info;
    }

    Fight_info fight(Fight_info attack_infomation){
        Fight_info get_attack_info;
        return get_attack_info;
    }
};

class Defense_Pal: public Pal{
public:
    Defense_Pal(Pal Base_Pal){
        interval = 0;
        cost = 0;

        name = Base_Pal.name;
        level = Base_Pal.level;
        exp = Base_Pal.exp;
        Attack_power = Base_Pal.Attack_power;
        Defense = Base_Pal.Defense;
        HP = Base_Pal.HP;
        Attack_interval = Base_Pal.Attack_interval;
        set_attribute_int(Base_Pal.get_attribute_int());
    }

    Fight_info fight(){
        Fight_info attack_info;
        return attack_info;
    }

    Fight_info fight(Fight_info attack_infomation){
        Fight_info get_attack_info;
        return get_attack_info;
    }
};

class Agile_Pal: public Pal{
public:
    Agile_Pal(Pal Base_Pal){
        interval = 0;
        cost = 0;

        name = Base_Pal.name;
        level = Base_Pal.level;
        exp = Base_Pal.exp;
        Attack_power = Base_Pal.Attack_power;
        Defense = Base_Pal.Defense;
        HP = Base_Pal.HP;
        Attack_interval = Base_Pal.Attack_interval;
        set_attribute_int(Base_Pal.get_attribute_int());
    }

    Fight_info fight(){
        Fight_info attack_info;
        return attack_info;
    }

    Fight_info fight(Fight_info attack_infomation){
        Fight_info get_attack_info;
        return get_attack_info;
    }
};

#endif // PALS_H
