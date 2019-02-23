/*
 * ====================== SkillTree.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.17
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    [- 任何程序代码都不应 include 本文件 -]
 *  --------
 *  游戏技能树 设计 ／ skill tree design
 * 
 * ----------------------------
 */
#ifndef _TPR_GAME_SYS_H_
#define _TPR_GAME_SYS_H_

//-------------------- CPP --------------------//
#include <vector>



// 可以直接对 敌方造成 hp伤害的 一切技能
class BattleAttack{
public:
    int attack; //- 普通攻击
};


// 确切地说是 战斗辅助。 本质是辅助工作，但仅仅与 战斗有关
class BattleSupport{
public:
    int aa;
};


class LogicSupport{
public:
    int bb;
};




class SkillTree{
public:

    std::vector<BattleAttack>   battleAttacks {}; //- 战斗攻击类
                // 可以对 敌方造成 hp伤害的 一切技能
                //
                //

    std::vector<BattleSupport>   battleSupports {}; //- 战斗辅助类
                // 本质是辅助工作，但仅仅与 战斗有关
                //
                //

    std::vector<LogicSupport>    logicSupports {}; //- 逻辑辅助类 (tmp)
                // 本意：一组类似编程规则的，作用与 指令序列的 技能
                // 
                //
                //

};



#endif 

