/*
 * ====================== GameSeed.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   game seed for random
 * ----------------------------
 */
#ifndef TPR_GameSeed_H
#define TPR_GameSeed_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"


//- singleton --
// 一个存档只有一个 gameSeed 实例 --
// 具体的 随机引擎和分布器，则由各模块自行包含 （tmp）
class GameSeed{
public:
    GameSeed(){
        this->randEngine.seed(0); //- 这一步仅为了应付 构造器。
                                // 真正的初始化，还在 init 函数中
    }

    void init( u32_t baseSeed_ );


    inline std::default_random_engine &getnc_realRandEngine(){
        return this->realRandEngine;
    }
    inline const glm::dvec2 &get_altiSeed_pposOffSeaLvl() const {
        return this->altiSeed_pposOffSeaLvl;
    }
    inline const glm::dvec2 &get_altiSeed_pposOffBig() const {
        return this->altiSeed_pposOffBig;
    }
    inline const glm::dvec2 &get_altiSeed_pposOffMid() const {
        return this->altiSeed_pposOffMid;
    }
    inline const glm::dvec2 &get_altiSeed_pposOffSml() const {
        return this->altiSeed_pposOffSml;
    }

    inline const glm::dvec2 &get_densitySeed_pposOff() const {
        return this->densitySeed_pposOff;
    }
    inline const glm::dvec2 &get_field_pposOff() const {
        return this->field_pposOff;
    }
    inline const glm::dvec2 &get_ecoObjWeight_pposOff() const {
        return this->ecoObjWeight_pposOff;
    }

    //======== static ========//
    static u32_t apply_new_baseSeed(){
        return get_new_seed();
    }

private:
    //======== vals ========//
    u32_t  baseSeed {}; //-- 最基础的那颗种子，其它种子由它生成。

    //- [-1000, 1000] 之间的 随机数
    glm::dvec2  altiSeed_pposOffSeaLvl {};
    glm::dvec2  altiSeed_pposOffBig {};
    glm::dvec2  altiSeed_pposOffMid {};
    glm::dvec2  altiSeed_pposOffSml {};

    //- [-1000, 1000] 之间的 随机数
    glm::dvec2  densitySeed_pposOff {};
    glm::dvec2  field_pposOff {};
    glm::dvec2  ecoObjWeight_pposOff {};

    //======== randEngine ========//
    std::default_random_engine  randEngine; //-通用 伪随机数引擎实例

    std::default_random_engine  realRandEngine; //-通用 真随机数引擎实例
                        //- 主用于 “无关静态世界” 的随机事件。比如 螃蟹刷怪笼 的分布。
                        //  每次生成都可以不一样
                        //  这个引擎会被外部 以任何次序和方式调用，所以它的值是彻底混乱的

    //======== flags ========//
    bool   is_all_seed_init {false};

    void init_glm_vec2s();
};





#endif 

