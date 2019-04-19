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
//#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
//#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

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

    void init();

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
    glm::vec2  ecoSysInMapWeight_pposOff {};

    //======== randEngine ========//
    std::default_random_engine  randEngine; //-通用 随机数引擎实例


    //======== flags ========//
    bool   is_all_seed_init {false};


private:

    void init_glm_vec2s();


};





#endif 

