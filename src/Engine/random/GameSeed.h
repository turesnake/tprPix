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

//------------------- CPP --------------------//
#include <cstdint> // uint8_t


//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"


//- singleton --
// 一个存档只有一个 gameSeed 实例 --
// 具体的 随机引擎和分布器，则由各模块自行包含 （tmp）
class GameSeed{
public:
    GameSeed()=default;

    void init( uint32_t baseSeed_ );

    inline std::default_random_engine &getnc_realRandEngine()noexcept{ return this->realRandEngine; }
    inline const glm::dvec2 &get_altiSeed_pposOffSeaLvl() const noexcept{ return this->altiSeed_pposOffSeaLvl; }
    inline const glm::dvec2 &get_altiSeed_pposOffBig() const noexcept{ return this->altiSeed_pposOffBig; }
    inline const glm::dvec2 &get_altiSeed_pposOffMid() const noexcept{ return this->altiSeed_pposOffMid; }
    inline const glm::dvec2 &get_altiSeed_pposOffSml() const noexcept{ return this->altiSeed_pposOffSml; }

    inline const glm::dvec2 &get_densitySeed_pposOff() const noexcept{ return this->densitySeed_pposOff; }
    inline const glm::dvec2 &get_field_dposOff() const noexcept{ return this->field_dposOff; }
    inline const glm::dvec2 &get_chunk_dposOff() const noexcept{ return this->chunk_dposOff; }
    inline const glm::dvec2 &get_ecoObjWeight_dposOff() const noexcept{ return this->ecoObjWeight_dposOff; }


    inline std::default_random_engine &getnc_shuffleEngine()noexcept{ 
        this->shuffleEngine.seed( GameSeed::fixedShuffleSeed ); // every time
        return this->shuffleEngine; 
    }


    //======== static ========//
    static uint32_t apply_new_baseSeed()noexcept{ return get_new_seed(); }

private:
    //======== vals ========//
    uint32_t  baseSeed {}; //-- 最基础的那颗种子，其它种子由它生成。

    //- [-1000, 1000] 之间的 随机数
    glm::dvec2  altiSeed_pposOffSeaLvl {};
    glm::dvec2  altiSeed_pposOffBig {};
    glm::dvec2  altiSeed_pposOffMid {};
    glm::dvec2  altiSeed_pposOffSml {};

    //- [-1000, 1000] 之间的 随机数 --
    glm::dvec2  densitySeed_pposOff {};

    //- [-1000.0, 1000.0] 之间的 随机数 --
    glm::dvec2  field_dposOff {};
    glm::dvec2  chunk_dposOff {};
    glm::dvec2  ecoObjWeight_dposOff {};

    

    //======== randEngine ========//
    std::default_random_engine  randEngine; //-通用 伪随机数引擎实例

    std::default_random_engine  realRandEngine; //-通用 真随机数引擎实例
                        //- 主用于 “无关静态世界” 的随机事件。比如 螃蟹刷怪笼 的分布。
                        //  每次生成都可以不一样
                        //  这个引擎会被外部 以任何次序和方式调用，所以它的值是彻底混乱的

    std::default_random_engine  shuffleEngine; //- 全局统一的 shuffle 用引擎


    //======== flags ========//
    bool   is_all_seed_init {false};

    void init_glm_vec2s();


    //======== static ========//
    static constexpr uint_fast32_t  fixedShuffleSeed { 131 }; 
                    // shuffle 用引擎 使用全局统一 seed，每次用前都要初始化
                    // 素数，全局固定
};


#endif 

