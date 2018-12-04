/*
 * =================== PerlinNoise3D.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.04
 *                                        修改 -- 2018.12.04
 * ----------------------------------------------------------
 *    随机数 模块
 * ----------------------------
 */
#include "PerlinNoise3D.h"
#include <cmath>

#include <algorithm>
#include <numeric>

//-------------------- C --------------------//
#include <cassert>


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


/* ===========================================================
 *                    init [1]
 * -----------------------------------------------------------
 * -- 手动设置 p集。
 * -- 本质是一串 [0,255] 的 乱序排列的 数。这个数串在p中存在 两遍
 */
void PerlinNoise3D::init() {

    //-- seed & eng --
	seed = get_new_seed();
    eng.seed(seed);

    //-- 向 p 中填入 [0,255] 这串数 -- 
    p.resize(256);
	std::iota(p.begin(), p.end(), 0);

    std::shuffle(p.begin(), p.end(), eng); //-- 将 p 中元素 乱序 --
	p.insert(p.end(), p.begin(), p.end()); //-- 元素 增殖 一倍 --

    is_init = true;
}

/* ===========================================================
 *                    init [2]
 * -----------------------------------------------------------
 * -- 通过调用者传入的 种子，来排序 [0,255] 这串数。
 * -- 最终目的，和上一个函数是一样的，也是制作成一个 p集
 */
void PerlinNoise3D::init( unsigned int _seed ) {

    //-- seed & eng --
	seed = _seed;
    eng.seed(seed);

	//-- 向 p 中填入 [0,255] 这串数 -- 
    p.resize(256);
	std::iota(p.begin(), p.end(), 0);

	std::shuffle(p.begin(), p.end(), eng); //-- 将 p 中元素 乱序 --
	p.insert(p.end(), p.begin(), p.end()); //-- 元素 增殖 一倍 --

    is_init = true;
}


/* ===========================================================
 *                      noise
 * -----------------------------------------------------------
 * -- 主函数，获取 noise 数据。 是3D版的。
 * -- 当用于 2D用途时，可将z值设为 任何值。比如 0
 * -- 参数 xyz 的有效 范围在 [0.n, 255.n] 允许存在小数部分
 */
float PerlinNoise3D::noise(float _x, float _y, float _z) {

    assert( is_init == true );//- 确保 调用者执行 init 函数

	// Find the unit cube that contains the point
    // 通过 x & 255 这个运算，取 x 的 低8-bit 。有点类似 取余 的功能
	int X = (int) floor(_x) & 255; //- floor() 获得目标数的 最大整数（但是，返回值类型是double）
	int Y = (int) floor(_y) & 255;
	int Z = (int) floor(_z) & 255;

	// Find relative x, y,z of point in cube
    //- xyz 变成了 原参数的 小数部分
	float x = _x - floor(_x);
	float y = _y - floor(_y);
	float z = _z - floor(_z);

	// Compute fade curves for each of x, y, z
	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A  = p[X]     + Y;
	int AA = p[A]     + Z;
	int AB = p[A + 1] + Z;
	int B  = p[X + 1] + Y;
	int BA = p[B]     + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	float res = lerp(w, 
                      lerp( v, 
                            lerp(u, 
                                 grad(p[AA], x, y, z), 
                                 grad(p[BA], x-1, y, z)), 
                            lerp(u, 
                                 grad(p[AB], x, y-1, z), 
                                 grad(p[BB], x-1, y-1, z))),	
                      lerp( v, 
                            lerp(u, 
                                 grad(p[AA+1], x, y, z-1), 
                                 grad(p[BA+1], x-1, y, z-1)), 
                            lerp(u, 
                                 grad(p[AB+1], x, y-1, z-1),	
                                 grad(p[BB+1], x-1, y-1, z-1))));
	    
    return (res + 1.0)/2.0;
}


/* ===========================================================
 *                    test_hight
 * -----------------------------------------------------------
 * -- 淡出曲线  6t^5 - 15t^4 + 10t^3
 */
float PerlinNoise3D::fade(float t) { 
	return t * t * t * (t * (t * 6 - 15) + 10);
}


/* ===========================================================
 *                      lerp
 * -----------------------------------------------------------
 * -- 有点 混合：glm::mix 的意思
 * -- (1-t)a + tb
 * -- 参数 t 决定了 b占有的 百分比（剩余部分是 a）
 */
float PerlinNoise3D::lerp(float t, float a, float b) { 
	return a + t * (b - a); 
    //- (1-t)a + tb 
}


/* ===========================================================
 *                      grad
 * -----------------------------------------------------------
 */
float PerlinNoise3D::grad(int hash, float x, float y, float z) {

    //- 保留 参数hash 低 4-bit 数据
    //- h 范围被限制在 [0, 15]
	int h = hash & 15;

	// Convert lower 4 bits of hash into 12 gradient directions
	//float u = h < 8 ? x : y;
    //float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

    float u;
    if( h < 8 ){
        u = x;
    }else{
        u = y;
    }

    float v;
    if( h < 4 ){
        v = y;
    }else{
        if( (h==12) || (h==14) ){
            v = x;
        }else{
            v = z;
        }
    }

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
