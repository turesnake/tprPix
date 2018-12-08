/*
 * ====================== ProjectMap.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    投影地图。 游戏的碰撞检测 核心组件
 *    ----------
 *    目前版本，默认制作一个 512 * 512 地面单位的 投影地图
 * ----------------------------
 */
#ifndef _TPR_PROJECT_MAP_H_
#define _TPR_PROJECT_MAP_H_

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

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//------------------- SELF --------------------//
#include "GameObj.h"



//-- 投影地图单位的 一级信息 --//
//-  优先查看此信息，如有需要，再通过 id 查找 二级信息。
//-- 可能包含的信息:
//  4-bits  -- [0,15] 地形高度 
//
//  1-bit  -- 是 陆地 还是 深渊
//  4~7-bit  -- 一个 id号，记录 3*3点阵 的 mask 信息
//             一个边缘 mapent(陆地 或 深渊)，各有 4*4=16 种类型。
//             基础种类4种，乘以 4个方向。
//             进一步的，每一种类型，可能随机选为某种 子状态 ...
//             所以此 项 bits 暂未确定
//             生成此 id时， 要配合 （陆地／深渊 标记位）来使用
//
//  1-bit  -- 本单位 是否被 某 go 预定      （若有，可访问 二级信息）
//  1-bit  -- 本单位 是否被 某 go 覆盖／踩住 （若有，可访问 二级信息）
//  1-bit  -- 如果被踩住，本单位是否是 那个 go 的 起始单位
//             （一个 go 可能同时踩住数个 mapEnt, 但只有一块，是 起始单位）
//             （起始单位 通常是 go 的投影单位集 中，左下角那一块）
//             （以此来，精确定位 go 的摆放位置）
//
//
struct Fst_MapEnt{
    //--- 一级信息区 ---
    u16   fst_data; //- 一级信息 本体
    u16   tex_id;   //- 地面材质类型texture
                    //  此处的 地面tex 只标记一种 “类型指定”， 
                    //  并不会真的对应到某张 具体的 png tex 上。
                    //  每一次，本 mapent 所在的 section 被加载并渲染时
                    //  这块地面上的 具体像素颜色，都可能发生变化。
                    //  就算是 深渊类型的地面，也会有材质信息。

    //--- 二级信息区 ---
    u16   sec_data_id; //- 二级信息 id号
};
//---------




//-- 投影地图单位的 二级信息 --//
//-  存储在一个 内存池中，数量有限。
struct Sec_MapEnt{
    u64  surface_go_id; //- 地面类 go id
    u64  item_go_id;    //- 道具 go id
    u64  reg_go_id;     //- 常规 go id

};



class ProjectMap{


public:


    void init();


private:
    std::vector<Fst_MapEnt>  fst_mapEnts; //- 地图单位 一级信息集

    //- 一个 内存池 实例，里面有申请到的 内存 --//
    //- 二级信息
    //...




};




//-- 256*256 个 mapEnt, 组成一张 section --
//  section 作为一个整体被存储到硬盘，就像 mc 中的 chunk 
class Section{





}







#endif

