/*
 * =================== PerlinNoise3D.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.04
 *                                        修改 -- 2018.12.04
 * ----------------------------------------------------------
 *    随机数 模块
 * ----------------------------
 */
#ifndef _TPR_PERLIN_NOISE_3D_H
#define _TPR_PERLIN_NOISE_3D_H


//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- SELF --------------------//
#include "Engine/random/random.h" 


class PerlinNoise3D {
public:
	
	explicit PerlinNoise3D() = default;

    //-- 推迟 init --
    void init(); //-- 使用自动生成的 seed 初始化 perlin --
    void init( u32 seed ); //-- 使用 调用者提供的 seed 初始化 perlin --

	float noise(float x, float y, float z);

private:
    u32  seed {}; //- 一个 perlin 实例， 需要一个稳定不变的种子。
    std::default_random_engine  eng; //- 随机数引擎，默认初始状态

    std::vector<int> p;
    bool is_init {false}; //- 检查 是否执行 init

    //-----------------
	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
};

#endif
