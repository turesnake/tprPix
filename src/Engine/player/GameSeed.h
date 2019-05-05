/*
 * ====================== GameSeed.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   game seed for random
 * ----------------------------
 */
#ifndef _TPR_GameSeed_H_
#define _TPR_GameSeed_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "random.h"
#include "PerlinNoise3D.h"
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

    

    void init( u32_t _baseSeed );


    inline const glm::vec2 &get_altiSeed_pposOffSeaLvl() const {
        return this->altiSeed_pposOffSeaLvl;
    }
    inline const glm::vec2 &get_altiSeed_pposOffBig() const {
        return this->altiSeed_pposOffBig;
    }
    inline const glm::vec2 &get_altiSeed_pposOffMid() const {
        return this->altiSeed_pposOffMid;
    }
    inline const glm::vec2 &get_altiSeed_pposOffSml() const {
        return this->altiSeed_pposOffSml;
    }

    inline const glm::vec2 &get_densitySeed_pposOff() const {
        return this->densitySeed_pposOff;
    }
    inline const glm::vec2 &get_field_pposOff() const {
        return this->field_pposOff;
    }
    inline const glm::vec2 &get_ecoObjWeight_pposOff() const {
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
    glm::vec2  altiSeed_pposOffSeaLvl {};
    glm::vec2  altiSeed_pposOffBig {};
    glm::vec2  altiSeed_pposOffMid {};
    glm::vec2  altiSeed_pposOffSml {};

    //- [-1000, 1000] 之间的 随机数
    glm::vec2  densitySeed_pposOff {};
    glm::vec2  field_pposOff {};
    glm::vec2  ecoObjWeight_pposOff {};

    //======== randEngine ========//
    std::default_random_engine  randEngine; //-通用 随机数引擎实例

    //======== flags ========//
    bool   is_all_seed_init {false};

    void init_glm_vec2s();
};





#endif 

