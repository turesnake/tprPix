/*
 * ========================= PerlinNoise1D.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Perlin Noise 2D 简易版
 *    Perlin Noise Next 
 * ----------------------------
 */
#ifndef TPR_PERLIN_NOISE_1D_H
#define TPR_PERLIN_NOISE_1D_H
#include "random.h"

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- CPP --------------------//
#include <cstdint> // uint8_t

//------------------- Engine --------------------//
#include "tprAssert.h"



//-- 一个 二维版的 perlin noise 曲线 --
// 用法:
//   PerlinNoise1D  perlin { freq_, ampl_ };
//   perlin.init( seed_ );
//   int y = perlin.get_y( x_ ); //- 通过x获得对应的y值。
class PerlinNoise1D{
public:
    PerlinNoise1D(  float freq_ = 1.0f, 
                    float ampl_ = 1.0f ):
        freq(freq_),
        ampl(ampl_),
        isInit( false )
        {}

    inline void init()noexcept{ //- use auto seed
        seed = get_new_seed();
        isInit = true;
    }
    inline void init( uint32_t seed_ )noexcept{ //- use param seed
        seed = seed_;
        isInit = true;
    }

    //-- 主功能，传入 perlin曲线的 x值，获得对应的 y值 --
    inline float get_y( float x_ )noexcept{

        tprAssert( isInit == true );//- 确保 调用者执行 init 函数

        x_ *= freq;
        float i = glm::floor( x_ ); //- 取 x_ 的整数部分
        float f = glm::fract( x_ ); //- 取 x_ 的小数部分

        float u = f * f * ( 3.0 - 2.0 * f ); //- 三次多项式 取代 glm::smoothstep()
        float y = glm::mix( pseudo_rand( i ), 
                            pseudo_rand( i + 1 ), 
                            u );
        y *= ampl;
        //------
        return y;
    }

    //-- 手动 更新 seed --
    inline void set_seed( uint32_t seed_ )noexcept{
        seed = seed_;
    }

    //-- 自动分配 新seed 值 --
    inline void set_seed_auto()noexcept{
        seed = get_new_seed();
    }

    //-- 手动更新 频率 和 振幅 --
    inline void set_freq( float freq_ )noexcept{
        freq = freq_;
    }

    //-- 手动更新 频率 和 振幅 --
    inline void set_ampl( float ampl_ )noexcept{
        ampl = ampl_;
    }

private:
    //-- 一个恒定不变的 伪随机数 序列 --
    //-  通过不同的 整形x
    //-  访问这个 序列上的 不同 y值 [ 0.0f, 1.0f ]
    inline float pseudo_rand( int intX_ )noexcept{
        engine.seed( seed );
        engine.discard( intX_ ); //- 前进n个状态
        return di(engine);
    }

    //======== vals ========//
    uint32_t seed {}; //- 一个 perlin 实例， 需要一个稳定不变的种子。
    std::default_random_engine   engine; //- 随机数引擎，默认初始状态
    std::uniform_real_distribution<float>  di { 0.0f, 1.0f }; //- 分布器

    float freq  {}; //- 频率
    float ampl  {}; //- 振幅

    bool isInit {}; //- 检查 是否执行 init
};


//-- 针对 perlin noise 曲线 的一种 "特殊但常用" 的方法 --
//  本class 将 perlin 的访问自动化
//  本class 自己维护一个 时快时慢的 x，
//  并自动将 下一个位置的 x 对应的 y 返回给调用者。
class PerlinNoise1DNext{
public:
    PerlinNoise1DNext(  float freq_, 
                        float ampl_ )
        {
            pn_main.set_freq( freq_ );
            pn_main.set_ampl( ampl_ );
        }
    
    inline void init()noexcept{
        pn_step.init();
        pn_main.init();
    }

    //-- 主功能 --
    //  按照一个 时快时慢的速度访问 perlin 曲线上的值
    //  进而获得一个 更加随机的 曲线 y值
    inline float next()noexcept{
        xstep += 1.0f;
        x += pn_step.get_y( xstep );
        return pn_main.get_y( x );
    }

private:
    PerlinNoise1D pn_step { 0.03f, 1.0f };
    PerlinNoise1D pn_main {}; //- 主perlin

    float x {0.0f};   //- 遍历 pn_main 的光标。
    float xstep {0.0f}; //- 遍历 pn_step 的光标
};





#endif
