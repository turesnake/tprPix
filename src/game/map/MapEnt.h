/*
 * ====================== MapEnt.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map 地面单位 
 *    ----------
 *    
 * ----------------------------
 */
#ifndef _TPR_MAP_ENT_H_
#define _TPR_MAP_ENT_H_

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
#include "PixVec.h" 
#include "GameObj.h"


//-- 投影地图单位的 一级信息 [硬盘态] --//
//-  优先查看此信息，如有需要，再通过 id 查找 二级信息。
//-  调整过排序。
struct Fst_diskMapEnt{

    //--- 二级信息区 ---
    u16   sec_data_id {0}; //- 二级信息 id号

    //--- 一级信息区 ---
    u16   fst_data {0}; //- 高4-bits -- [0,15] 地形高度/altitude
                        //- 1-bit  -- 本单位 是否被 某 go 预定 （若有，可访问 二级信息）
                        //- 1-bit  -- 本单位 是否被 某 go 覆盖／踩住 （若有，可访问 二级信息）
                        //- 低1-bit  -- 如果被踩住，本单位是否是 那个 go 的 起始单位
                        //           （一个 go 可能同时踩住数个 mapEnt, 但只有一块，是 起始单位）
                        //           （起始单位 通常是 go 的投影单位集 中，左下角那一块）
                        //           （以此来，精确定位 go 的摆放位置）
    u16   tex_id {0}; //- 地面材质类型texture
                      //  此处的 地面tex 只标记一种 “类型指定”， 
                      //  并不会真的对应到某张 具体的 png tex 上。
                      //  每一次，本 mapent 所在的 section 被加载并渲染时
                      //  这块地面上的 具体像素颜色，都可能发生变化。
                      //  就算是 深渊类型的地面，也会有材质信息。
    u8    mask_id {0}; //- 高1-bit -- 陆地(1) 还是 深渊(0)
                       //  低7-bit -- mask_id.记录 3*3点阵 的 mask 信息
                       //          一个边缘 mapent(陆地 或 深渊)，各有 4*4=16 种类型。
                       //          基础种类4种，乘以 4个方向。
                       //          进一步的，每一种类型，可能随机选为某种 子状态 ...
                       //          生成此 id时， 要配合 （陆地／深渊 标记位）来使用
    u8   padding {0};  
    
};

//-- 投影地图单位的 一级信息 [内存态] --
//-  牺牲一定的 内存空间，换取访问便捷度
class Fst_memMapEnt{
public:
    explicit Fst_memMapEnt( Fst_diskMapEnt *_me ){
        d2m( _me );
    }

    void d2m( Fst_diskMapEnt *_me );
    Fst_diskMapEnt m2d();


    //---------- data ----------//
    bool is_land; //- 陆地／深渊
    u8   mask_id; //- 3*3矩阵 渲染像素 mask
    u8   altitude; //- 海拔.(低4-bit 有效)
    bool is_reserved; //- 是否被某 go 预订
    bool is_covered;  //- 是否被某 go 覆盖／踩住
    bool is_cover_go_head; //- 如果被踩住，本单位是否是 那个 go 的 起始单位
    u16   tex_id;   //- 地面材质类型texture
                    //  此处的 地面tex 只标记一种 “类型指定”， 
                    //  并不会真的对应到某张 具体的 png tex 上。
                    //  每一次，本 mapent 所在的 section 被加载并渲染时
                    //  这块地面上的 具体像素颜色，都可能发生变化。
                    //  就算是 深渊类型的地面，也会有材质信息。
    //--- 二级信息区 ---
    u16   sec_data_id; //- 二级信息 id号

    PixVec2  pos; //- 本 mapent 左下角像素的 坐标值 

private:



};




//-- 投影地图单位的 二级信息 --//
//-  存储在一个 内存池中，数量有限。
struct Sec_MapEnt{
    u64  surface_go_id; //- 地面类 go id (液体，火焰等)
    u64  item_go_id;    //- 道具 go id
    u64  reg_go_id;     //- 常规 go id
};








#endif

