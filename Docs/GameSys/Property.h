/*
 * ====================== Property.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.17
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    [- 任何程序代码都不应 include 本文件 -]
 *  --------
 *  具象go类／游戏生物 属性表
 * 
 * ----------------------------
 */
#ifndef _TPR_PROPERTY_H_
#define _TPR_PROPERTY_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 


//- 各种属性的 抗性等级 -- 
enum class ResisLvl : u8{
    Weak,    //- 伤害加倍  (弱)
    Norm,    //- 正常伤害 （default）
    Half,    //- 伤害减半
    Immu,    //- 1/4伤害 （近乎免疫）
}; 




struct ResisOfElement{
    ResisLvl  lighting {ResisLvl::Norm}; //- 电
    ResisLvl  poison   {ResisLvl::Norm}; //- 毒
    ResisLvl  water    {ResisLvl::Norm}; //- 水
    ResisLvl  fire     {ResisLvl::Norm}; //- 火
};


//-- 具象go类／游戏生物 属性表 --
//  应该设计得充分 简单／大胆 
class Property{
public:
    int  maxHP; //- 生命值 
    int  currentHP;
                //  最纯粹的生命值。

    int  dmg;   //- 单次伤害。
                // 经过 技能scale缩放后，作用于 currentHP.

    int  speed; //-
                //- 对于主要移动方式 crawl 来说，共有8档。
                //- speed 只能在这些档位之间切换。

    ResisOfElement  resisOfElement {}; //- 各属性抗性等级


    u32             resisOfStates  {}; //- bitmap,异常状态免疫表
                // 是否对某个异常状态 免疫（ 1:免疫。 0:不免疫 ）
                // 这部分可以专门制作一个 class
                // ----------------
                // -- hitRecovery / 硬直 （所有生物都为0）
                // -- repel ／ 击退
                // -- poison ／ 中毒
                // -- dizzy  ／ 眩晕
                // -- paralysis / 麻痹
                // -- sleep ／ 睡眠
                // -- blind ／ 失明
                // -- burn  ／ 着火
                // -- rabies / 狂犬症
                // -- plague ／ 鼠疫
                // -- dehydration ／ 脱水
};




#endif 

