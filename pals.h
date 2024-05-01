#ifndef PALS_H
#define PALS_H

#include <Qstring>
#include <QRandomGenerator>
#include <QtMath>

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
    QString special_power_desc;
    QString Attack_giver_name;

    int real_HP_loss;
    QString special_status;
    QString Attack_reciver_name;

    Fight_info(){
        Attack_power = 0;
        special_power_name = "";
        special_power_desc = "";
        Attack_giver_name = "";
        real_HP_loss = 0;
        special_status = "";
        Attack_reciver_name = "";
    }

    QString Fight_info_output(){
        QString output;
        if(Attack_power == 0){
            if(special_power_name == "")
                output = "-1";
            else{
                output += (Attack_giver_name + " 发动了 【" + special_power_name + "】");
            }

            if(special_power_desc == "0&22")
                output += (" 无效化了对手下一次技能");
            else if(special_power_desc == "0&24")
                output += (" 抵消了下一次受到的攻击");
            else if(special_power_desc == "0&42")
                output += (" 立即回复了 20% 的生命值");
            else if(special_power_desc == "0&43")
                output += (" 立即增加了 40 普通攻击点与 20 技能点");
        }
        else{
            output += (Attack_giver_name + " 对 " + Attack_reciver_name);
            if(special_power_name != "")
                output += (" 发动了 【" + special_power_name + "】");
            output += " 造成了 " + QString::number(real_HP_loss) + " 点伤害";

            if(special_power_desc != ""){
                if(special_power_desc[0] == '1')
                    output += " 同时使 " + Attack_reciver_name;
                else if(special_power_desc[0] == '0')
                    output += " 同时使 " + Attack_giver_name;

                if(special_power_desc == "0&21")
                    output += (" 下一次受击伤害减少 50%");
                else if(special_power_desc == "0&22")
                    output += (" 无效化了对手下一次技能");
                else if(special_power_desc == "0&23")
                    output += (" 下一次受击时防御力增加 50%");
                else if(special_power_desc == "0&24")
                    output += (" 抵消了下一次受到的攻击");
                else if(special_power_desc == "0&42")
                    output += (" 立即回复了 20% 的生命值");
            }
        }
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
        return 0;
    }

    virtual Fight_info fight(){
        Fight_info attack_info;
        return attack_info;
    } //内部自行判断是否给出普通攻击与技能。如果给出内部自行处理cost与interval

    virtual Fight_info fight(Fight_info attack_infomation){
        Fight_info get_attack_info = attack_infomation;
        get_attack_info.real_HP_loss = (attack_infomation.Attack_power - this->Defense);
        return get_attack_info;
    } //内部判断受击结果，自行处理HP等。给出反馈

};

class Strength_Pal: public Pal{
    private:
        int special_power_choice;
        int special_power[4 + 1];
        int special_power_cost[4 + 1];
        QString special_power_name[4 + 1];
        QString special_power_desc[4 + 1];

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
            special_power[4] = 2 * Attack_power;
            special_power_cost[1] = 20;
            special_power_cost[2] = 35;
            special_power_cost[3] = 30;
            special_power_cost[4] = 25;
            special_power_name[1] = "瓦尼瓦尼";
            special_power_name[2] = "光啊！";
            special_power_name[3] = "格黑娜的风纪委员";
            special_power_name[4] = "少女们的垂怜经";
            special_power_desc[1] = "";
            special_power_desc[2] = "";
            special_power_desc[3] = "";
            special_power_desc[4] = "";
        }

        Fight_info fight(){
            Fight_info attack_info;
            attack_info.Attack_giver_name = this->name;
            if(interval >= Attack_interval){
                attack_info.Attack_power += Attack_power;
                interval -= Attack_interval;
            }

            if(special_power_choice == -1){
                special_power_choice = QRandomGenerator::global()->bounded(1, 5);
            }
            else{
                switch(special_power_choice){
                case 1:
                    if(cost >= special_power_cost[1]){
                        cost -= special_power_cost[1];
                        attack_info.Attack_power += special_power[1];
                        attack_info.special_power_name = special_power_name[1];
                        attack_info.special_power_desc = special_power_desc[1];
                        special_power_choice = -1;
                    }
                    break;
                case 2:
                    if(cost >= special_power_cost[2]){
                        cost -= special_power_cost[2];
                        attack_info.Attack_power += special_power[2];
                        attack_info.special_power_name = special_power_name[2];
                        attack_info.special_power_desc = special_power_desc[2];
                        special_power_choice = -1;
                    }
                    break;
                case 3:
                    if(cost >= special_power_cost[3]){
                        cost -= special_power_cost[3];
                        attack_info.Attack_power += special_power[3];
                        attack_info.special_power_name = special_power_name[3];
                        attack_info.special_power_desc = special_power_desc[3];
                        special_power_choice = -1;
                    }
                    break;
                case 4:
                    if(cost >= special_power_cost[4]){
                        cost -= special_power_cost[4];
                        attack_info.Attack_power += special_power[4];
                        attack_info.special_power_name = special_power_name[4];
                        attack_info.special_power_desc = special_power_desc[4];
                        special_power_choice = -1;
                    }
                    break;
                }
            }
            return attack_info;
        }

        Fight_info fight(Fight_info attack_infomation){
            Fight_info get_attack_info = attack_infomation;
            get_attack_info.Attack_reciver_name = this->name;
            get_attack_info.real_HP_loss = (int)((double)get_attack_info.Attack_power * (double)(1 - Defense/1000) * 0.05);
            this->HP -= get_attack_info.real_HP_loss;
            return get_attack_info;
        }
};

class Tank_Pal: public Pal{
    private:
        int special_power_choice;
        int special_power[4 + 1];
        int special_power_cost[4 + 1];
        QString special_power_name[4 + 1];
        QString special_power_desc[4 + 1];

        bool next_fight_useless;
        bool next_fight_Defused;
        bool next_HP_loss_half;
        bool next_hurt_DP_more;

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

            special_power_choice = -1;

            special_power[1] = 2 * Attack_power;
            special_power[2] = 0;
            special_power[3] = 2 * Attack_power;
            special_power[4] = 0;
            special_power_cost[1] = 35;
            special_power_cost[2] = 30;
            special_power_cost[3] = 35;
            special_power_cost[4] = 30;
            special_power_name[1] = "千年的100kg小姐";
            special_power_name[2] = "蓝色恶魔";
            special_power_name[3] = "一动不动还是好热啊~ ";
            special_power_name[4] = "大人的责任";
            special_power_desc[1] = "0&21";
            special_power_desc[2] = "0&22";
            special_power_desc[3] = "0&23";
            special_power_desc[4] = "0&24";

            next_fight_useless = false;
            next_fight_Defused = false;
            next_HP_loss_half = false;
            next_hurt_DP_more = false;
        }

        Fight_info fight(){
            Fight_info attack_info;
            attack_info.Attack_giver_name = this->name;
            if(interval >= Attack_interval){
                attack_info.Attack_power += Attack_power;
                interval -= Attack_interval;
            }

            if(special_power_choice == -1){
                special_power_choice = QRandomGenerator::global()->bounded(1, 5);
            }
            else{
                switch(special_power_choice){
                case 1:
                    if(cost >= special_power_cost[1]){
                        cost -= special_power_cost[1];
                        attack_info.Attack_power += special_power[1];
                        attack_info.special_power_name = special_power_name[1];
                        attack_info.special_power_desc = special_power_desc[1];
                        special_power_choice = -1;

                        next_HP_loss_half = true;
                    }
                    break;
                case 2:
                    if(cost >= special_power_cost[2]){
                        cost -= special_power_cost[2];
                        attack_info.Attack_power += special_power[2];
                        attack_info.special_power_name = special_power_name[2];
                        attack_info.special_power_desc = special_power_desc[2];
                        special_power_choice = -1;

                        next_fight_useless = true;
                    }
                    break;
                case 3:
                    if(cost >= special_power_cost[3]){
                        cost -= special_power_cost[3];
                        attack_info.Attack_power += special_power[3];
                        attack_info.special_power_name = special_power_name[3];
                        attack_info.special_power_desc = special_power_desc[3];
                        special_power_choice = -1;

                        next_hurt_DP_more = true;
                    }
                    break;
                case 4:
                    if(cost >= special_power_cost[4]){
                        cost -= special_power_cost[4];
                        attack_info.Attack_power += special_power[4];
                        attack_info.special_power_name = special_power_name[4];
                        attack_info.special_power_desc = special_power_desc[4];
                        special_power_choice = -1;

                        next_fight_Defused = true;
                    }
                    break;
                }
            }
            return attack_info;
        }

        Fight_info fight(Fight_info attack_infomation){
            Fight_info get_attack_info = attack_infomation;
            get_attack_info.Attack_reciver_name = this->name;
            if(get_attack_info.Attack_power != 0 && get_attack_info.special_power_name != "" && next_fight_useless){
                get_attack_info.real_HP_loss = 0;
                next_fight_useless = false;
            }
            else if(get_attack_info.Attack_power != 0 && next_fight_Defused){
                get_attack_info.real_HP_loss = 0;
                next_fight_Defused = false;
            }
            else if(get_attack_info.Attack_power != 0 && next_HP_loss_half){
                get_attack_info.real_HP_loss = (int)((double)get_attack_info.Attack_power * (double)(1 - Defense/1000) * 0.05 * 0.5);
            }
            else if(get_attack_info.Attack_power != 0 && next_hurt_DP_more){
                get_attack_info.real_HP_loss = (int)((double)get_attack_info.Attack_power * (double)(fmax((1 - (Defense * 2)/1000), 0)) * 0.05);
            }
            else{
                get_attack_info.real_HP_loss = (int)((double)get_attack_info.Attack_power * (double)(1 - Defense/1000) * 0.05);
            }
            this->HP -= get_attack_info.real_HP_loss;
            return get_attack_info;
        }
};

class Defense_Pal: public Pal{
    private:
        int special_power_choice;
        int special_power[4 + 1];
        int special_power_cost[4 + 1];
        QString special_power_name[4 + 1];
        QString special_power_desc[4 + 1];

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

            special_power_choice = -1;

            special_power[1] = 2 * Attack_power;
            special_power[2] = 0;
            special_power[3] = 2 * Attack_power;
            special_power[4] = 0;
            special_power_cost[1] = 35;
            special_power_cost[2] = 30;
            special_power_cost[3] = 35;
            special_power_cost[4] = 30;
            special_power_name[1] = "千年的100kg小姐";
            special_power_name[2] = "蓝色恶魔";
            special_power_name[3] = "一动不动还是好热啊~ ";
            special_power_name[4] = "大人的责任";
            special_power_desc[1] = "0&21";
            special_power_desc[2] = "0&22";
            special_power_desc[3] = "0&23";
            special_power_desc[4] = "0&24";
        }

        Fight_info fight(){
            Fight_info attack_info;
            return attack_info;
        }

        Fight_info fight(Fight_info attack_infomation){
            Fight_info get_attack_info = attack_infomation;
            get_attack_info.Attack_reciver_name = this->name;
            get_attack_info.real_HP_loss = (int)((double)get_attack_info.Attack_power * (double)(1 - Defense/1000) * 0.05);
            this->HP -= get_attack_info.real_HP_loss;
            return get_attack_info;
        }
};

class Agile_Pal: public Pal{
    private:
        int special_power_choice;
        int special_power[4 + 1];
        int special_power_cost[4 + 1];
        QString special_power_name[4 + 1];
        QString special_power_desc[4 + 1];

        int full_HP;

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
            full_HP = HP;
            Attack_interval = Base_Pal.Attack_interval;
            set_attribute_int(Base_Pal.get_attribute_int());

            special_power_choice = -1;

            special_power[1] = Attack_power;
            special_power[2] = Attack_power;
            special_power[3] = 0;
            special_power[4] = 2 * Attack_power;
            special_power_cost[1] = 5;
            special_power_cost[2] = 20;
            special_power_cost[3] = 15;
            special_power_cost[4] = 15;
            special_power_name[1] = "游戏开发部的秘籍";
            special_power_name[2] = "H的事情不行！";
            special_power_name[3] = "对玛导致昏";
            special_power_name[4] = "UZQueen";
            special_power_desc[1] = "";
            special_power_desc[2] = "0&42";
            special_power_desc[3] = "0&43";
            special_power_desc[4] = "";
        }

        Fight_info fight(){
            Fight_info attack_info;
            attack_info.Attack_giver_name = this->name;
            if(interval >= Attack_interval){
                attack_info.Attack_power += Attack_power;
                interval -= Attack_interval;
            }

            if(special_power_choice == -1){
                special_power_choice = QRandomGenerator::global()->bounded(1, 5);
            }
            else{
                switch(special_power_choice){
                case 1:
                    if(cost >= special_power_cost[1]){
                        cost -= special_power_cost[1];
                        attack_info.Attack_power += special_power[1];
                        attack_info.special_power_name = special_power_name[1];
                        attack_info.special_power_desc = special_power_desc[1];
                        special_power_choice = -1;
                    }
                    break;
                case 2:
                    if(cost >= special_power_cost[2]){
                        cost -= special_power_cost[2];
                        if((double)HP < (double)full_HP * 0.5){
                            attack_info.special_power_name = special_power_name[2];
                            attack_info.special_power_desc = special_power_desc[2];
                            HP += (int)((double)full_HP * 0.2);
                        }
                        else{
                            attack_info.Attack_power += special_power[2];
                            attack_info.special_power_name = special_power_name[2];
                        }
                        special_power_choice = -1;
                    }
                    break;
                case 3:
                    if(cost >= special_power_cost[3]){
                        cost -= special_power_cost[3];
                        attack_info.Attack_power += special_power[3];
                        attack_info.special_power_name = special_power_name[3];
                        attack_info.special_power_desc = special_power_desc[3];
                        special_power_choice = -1;

                        this->interval += 40;
                        this->cost += 20;
                    }
                    break;
                case 4:
                    if(cost >= special_power_cost[4]){
                        cost -= special_power_cost[4];
                        attack_info.Attack_power += special_power[4];
                        attack_info.special_power_name = special_power_name[4];
                        attack_info.special_power_desc = special_power_desc[4];
                        special_power_choice = -1;
                    }
                    break;
                }
            }
            return attack_info;
        }

        Fight_info fight(Fight_info attack_infomation){
            Fight_info get_attack_info = attack_infomation;
            get_attack_info.Attack_reciver_name = this->name;
            get_attack_info.real_HP_loss = (int)((double)get_attack_info.Attack_power * (double)(1 - Defense/1000) * 0.05);
            this->HP -= get_attack_info.real_HP_loss;
            return get_attack_info;
        }
};

#endif // PALS_H
