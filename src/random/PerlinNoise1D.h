/*
 * ========================= PerlinNoise1D.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.03
 *                                        修改 -- 2018.12.03
 * ----------------------------------------------------------
 *    Perlin Noise 2D 简易版
 *    Perlin Noise Next 
 * ----------------------------
 */
#ifndef _TPR_PERLIN_NOISE_H_
#define _TPR_PERLIN_NOISE_H_
#include "random.h"


//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- C --------------------//
#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h"



//-- 一个 二维版的 perlin noise 曲线 --
class PerlinNoise1D{
public:
    explicit PerlinNoise1D( float _freq = 1.0f, 
                          float _ampl = 1.0f ):
        freq(_freq),
        ampl(_ampl),
        is_init( false )
        {}

    //-- 将 init 分离 --
    inline void init(){
        seed = get_new_seed();
        is_init = true;
    }

    //-- 主功能，传入 perlin曲线的 x值，获得对应的 y值 --
    inline float get_y( float _x ){

        assert( is_init == true );//- 确保 调用者执行 init 函数

        _x *= freq;
        float i = glm::floor( _x ); //- 取 _x 的整数部分
        float f = glm::fract( _x ); //- 取 _x 的小数部分

        float u = f * f * ( 3.0 - 2.0 * f ); //- 三次多项式 取代 glm::smoothstep()
        float y = glm::mix( pseudo_rand( i ), 
                            pseudo_rand( i + 1 ), 
                            u );
        y *= ampl;
        //------
        return y;
    }

    //-- 手动 更新 seed --
    inline void set_seed( u32 _seed ){
        seed = _seed;
    }

    //-- 自动分配 新seed 值 --
    inline void set_seed_auto(){
        seed = get_new_seed();
    }

    //-- 手动更新 频率 和 振幅 --
    inline void set_freq( float _freq ){
        freq = _freq;
    }

    //-- 手动更新 频率 和 振幅 --
    inline void set_ampl( float _ampl ){
        ampl = _ampl;
    }

private:
    u32 seed {}; //- 一个 perlin 实例， 需要一个稳定不变的种子。
    std::default_random_engine   eng; //- 随机数引擎，默认初始状态
    std::uniform_real_distribution<float>  di { 0.0f, 1.0f }; //- 分布器

    float freq; //- 频率
    float ampl; //- 振幅

    bool is_init; //- 检查 是否执行 init，


    //-- 一个恒定不变的 伪随机数 序列 --
    //-  通过不同的 整形x
    //-  访问这个 序列上的 不同 y值 [ 0.0f, 1.0f ]
    inline float pseudo_rand( int _intx ){

        eng.seed( seed );
        eng.discard( _intx ); 
        return di(eng);
    }
};


//-- 针对 perlin noise 曲线 的一种 "特殊但常用" 的方法 --
//  本class 将 perlin 的访问自动化
//  本class 自己维护一个 时快时慢的 x，
//  并自动将 下一个位置的 x 对应的 y 返回给调用者。
class PerlinNoise1DNext{
public:
    explicit PerlinNoise1DNext( float _freq, 
                                float _ampl )
        {
            pn_main.set_freq( _freq );
            pn_main.set_ampl( _ampl );
        }
    
    //-- 将 init 分离 --
    inline void init(){
        pn_step.init();
        pn_main.init();
    }

    //-- 主功能 --
    //  按照一个 时快时慢的速度访问 perlin 曲线上的值
    //  进而获得一个 更加随机的 曲线 y值
    inline float next(){
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
