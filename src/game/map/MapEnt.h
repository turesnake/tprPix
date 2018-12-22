/*
 * ====================== MapEnt.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map 地面单位 
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
#include "ID_Manager.h" 


//-- 投影地图单位的 一级信息 [硬盘态] --//
//-  优先查看此信息，如有需要，再通过 id 查找 二级信息。
//-  -- 调整过字段排序 --
struct Fst_diskMapEnt{

    u16   tex_id {NULLID}; //- 地面材质类型texture
                      //  此处的 地面tex 只标记一种 “类型指定”， 
                      //  并不会真的对应到某张 具体的 png tex 上。
                      //  每一次，本 mapent 所在的 section 被加载并渲染时
                      //  这块地面上的 具体像素颜色，都可能发生变化。
                      //  就算是 深渊类型的地面，也会有材质信息。

    //--- 二级信息区 ---
    u16   sec_data_id {NULLID}; //- 二级信息 id号

    u8   sec_data_info {0}; //-- 二级信息 额外数据
                    //- 1-bit -- is_major_go_default.
                    //           若为“常规”，surface_go 存储的是 species。
                    //           若不为常规，surface_go 存储的是 具体 go id号。                    
                    //[-余-7-bits-]...

    //--- 一级信息区 ---
    u8   fst_data {0}; //- 高4-bits -- [0,15] 地形高度/altitude
                        //- 1-bit  -- is_reserved. 本mapent 是否被 某 major_go 预定 （若有，可访问 二级信息）
                        //- 1-bit  -- is_covered. 本mapent 是否被 某 major_go 覆盖／踩住 （若有，可访问 二级信息）
                        //            go_head. 如果被踩住，本单位是否是 那个 go 的 起始单位
                        //            由于在 硬盘态，并不保留独立的 道具go，
                        //            所以，只要显示 is_covered（被一个 major go覆盖）
                        //            就意味着 这一格是 go head.
                        //           （一个 go 可能同时踩住数个 mapEnt, 但只有一块，是 起始单位）
                        //           （起始单位 通常是 go 的投影单位集 中，左下角那一块）
                        //           （以此来，精确定位 go 的摆放位置）
                        //            一个 major_go 占据数个单位的 mapent，且只有一个 go head
                        //
                        //[-余-2-bits-]...

    u8    mask_id {0}; //- 高1-bit -- 陆地(1) 还是 深渊(0)
                       //  低7-bit -- mask_id.记录 3*3点阵 的 mask 信息
                       //          一个边缘 mapent(陆地 或 深渊)，各有 4*4=16 种类型。
                       //          基础种类4种，乘以 4个方向。
                       //          进一步的，每一种类型，可能随机选为某种 子状态 ...
                       //          生成此 id时， 要配合 （陆地／深渊 标记位）来使用  

    //--- padding -----
    u8   padding  {0};
};


//-- 投影地图单位的 二级信息 --//
//-  存储在一个 内存池中，数量有限。
//--- 为了最大限度地 减少 go硬盘存储，二级信息中的 字段。兼容2种信息:
//  -1- 在硬盘态, 有些 go 为压缩态，只存储其 go_species 信息，
//      当我们从 硬盘读取到 内存后，根据这个 go_species 信息，就地生成一个 go实例，
//      放在对应 mapent 上。并把此时的 二级信息，改为 这个临时新建的 go_id
//
//  --- 如果一个 go 始终没被改动过，它的 is_default 字段始终 true。
//  --- 一个 被改动过的 go实例，在间隔一定游戏时间后，也会恢复 default 态。
//      比如一颗树，被玩家砍过一刀。但过去了很久，这颗树的 go实例将会恢复 default。
//  --- 部分 go实例，只要不死，会在每次 伴随 section 存入硬盘时，被强行 default 化。
//      又比如某颗树，只要不被砍死，在存入硬盘时，都会被 default 化。
//
//  -2- 当我们 要把一个 mapSection 存储到 硬盘中时。会遍历 此 section 中的所有 go实例
//      如果一个 go实例 为 default。我们会丢弃这个 go实例，仅把它的 go_species 信息，
//      写入 二级信息中。 以此节约 一份 go实例的 硬盘存储。
//------------------
// -- 一个 surface go 的每一个 mapent，都会被记录到 二级信息中
//     尽管它看起来有点浪费 二级信息存储空间，但它节省了 go硬盘存储空间
// -- 一个 item go 永不独立存入硬盘。（但可放置 宝箱内）
// -- 一个 major go 拥有数个 mapent，其中只有一个 head ent
//     只有这个 head ent 会被记录到 二级信息中，剩余的 常规 ent，会被丢弃。
struct Sec_diskMapEnt{

    u64  major_go   {0}; //- 主体 go id／species (活体，建筑，树...)  
    
    //- 道具 go. 并不存入硬盘。
    //- 当 section 被存入硬盘时，其上的所有 裸露道具go（宝箱内不算）都会被销毁。
    //- 下一次读取此 section 时。也不会重建。
    //- 一切 道具go， 只占据 1个 mapent，且是 head ent 
    //----------------
    //  道具go 的创建方式：
    //  --1-- 当一张 section 第一次被创建时，会在地图上随机生成 宝箱，宝箱内有 若干道具。 
    //        存在一种 道具密集点，那里分部的 道具会非常多...
    //        这种创建 只发生一次。
    //        宝箱作为一种 major go, 也存在生命值（通常非常大）。
    //        一旦被击毁，会把自己携带的 所有 道具go 都散布在地图上。
    //  --2-- 当 击败一名 major go 时，有一定记录 爆出 道具。

    u32  surface_go_species {0}; //- 地面 go species (液体，火焰等)
                              //- 一切 surface类 go实例，在硬盘存储时，都会被压缩为 species。
                              //- 每次都在 section 加载阶段，根据 species 临时创建 go实例。

    //---- padding -----//
    u32  padding    {0};
};





//-- 投影地图单位的信息 [内存态] --
//-  牺牲一定的 内存空间，换取访问便捷度
class MemMapEnt{
public:
    explicit MemMapEnt( Fst_diskMapEnt *_fdme ){
        fst_d2m( _fdme );
    }

    
    //=============== data: 一级信息 ===============//
    bool is_land     {true}; //- 陆地／深渊
    u8   mask_id     {0}; //- 3*3矩阵 渲染像素 mask
    u8   altitude    {0}; //- 海拔.(低4-bit 有效)
    bool is_reserved {true}; //- 是否被某 go 预订
    bool is_covered  {true};  //- 是否被某 go 覆盖／踩住
    bool is_cover_go_head {true}; //- 如果被踩住，本单位是否是 那个 go 的 起始单位
    u16  tex_id      {NULLID};   //- 地面材质类型texture
                    //  此处的 地面tex 只标记一种 “类型指定”， 
                    //  并不会真的对应到某张 具体的 png tex 上。
                    //  每一次，本 mapent 所在的 section 被加载并渲染时
                    //  这块地面上的 具体像素颜色，都可能发生变化。
                    //  就算是 深渊类型的地面，也会有材质信息。
    //--- 二级信息区 ---
    u16  sec_data_id  {NULLID}; //- 二级信息 id号

    bool is_major_go_default   {true};

    //-- 一级信息： mem <--> disk --
    void fst_d2m( Fst_diskMapEnt *_dme );
    Fst_diskMapEnt fst_m2d();

    
    //=============== data: 二级信息 ===============//
    u64  major_go_id   {NULLID}; //- 主体go id. (实例)
    u64  item_go_id    {NULLID}; //- 道具go id. (实例，并不存入硬盘)
    u64  surface_go_id {NULLID}; //- 表面go id. (实例，压缩为 species 存入硬盘)

    //-- 二级信息： mem <--> disk --
    void sec_d2m( Sec_diskMapEnt *_dme ); //-- unfinish...
    Sec_diskMapEnt sec_m2d(); //-- unfinish...


    //=============== data: oth ===============//
    PixVec2  pos {}; //- 本 mapent 左下角像素的 坐标值 
private:
};











#endif

