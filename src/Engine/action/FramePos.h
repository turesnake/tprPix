/*
 * ========================= FramePos.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *    action 中，单张 图元帧 拥有的 全部 pos数据集
 *  ----
 *  -1- rootColliEntsOff
 *  -2- rootAnchorOff
 *  -3- colliEnts
 *  -----
 *  上诉数据之间的 转变变得复杂。
 *  将他们捆绑，集中存储在一个 数据容器中，统一管理
 * ----------------------------
 */
#ifndef _TPR_FRAME_POS_H_
#define _TPR_FRAME_POS_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "MapCoord.h"
#include "AltiRange.h"



class FramePos{
public:
    FramePos(){};

    //---- set ----//
    // set 阶段 无需考虑性能
    inline void clear_all(){
        rootColliEntOff = PixVec2{ 0, 0};
        rootAnchorOff   = PixVec2{ 0, 0};
        altiRange.low = 0;
        altiRange.high = 0;
        colliEntSetIdx = 0;
        is_rootColliEntOff_set = false;
        is_rootAnchorOff_set = false;
        is_altiRange_set = false;
        is_colliEntSetIdx_set = false;
    }

    inline void set_rootColliEntOff( const PixVec2 &_v ){
        rootColliEntOff = _v;
        is_rootColliEntOff_set = true;
    }

    inline void set_rootAnchorOff( const PixVec2 &_v  ){
        rootAnchorOff  = _v;
        is_rootAnchorOff_set = true;
    }

    inline void set_altiRange( const AltiRange &_ar ){
        altiRange = _ar;
        is_altiRange_set = true;
    }

    inline void set_colliEntSetIdx( int _idx ){
        colliEntSetIdx = _idx;
        is_colliEntSetIdx_set = true;
    }

    //-- 检测 rootAnchor偏移 是否与 idx指向的 colliEntSet预制件
    //   中的 center 偏移 吻合 
    void check();

    //---- get ----//
    inline const PixVec2 &get_rootAnchorOff() const {
        return rootAnchorOff;
    }

    //-- debug --


private:

    PixVec2     rootColliEntOff  {0,0}; 
                                //-- 从图元帧 左下角，到 rootColliEnt 的 pos偏移值
                                //   [-可以是任意 整形数，不用对齐于 mapEnt-]
                                //   rootColliEnt 在概念上是个点，
                                //   它是 所有 colliEnts 的左下角
    
    PixVec2     rootAnchorOff     {0,0};
                                //-- 从图元帧 左下角，到 rootAnchor 的 pos偏移值
                                //   [-可以是任意 整形数，不用对齐于 mapEnt-]
                                //   每一帧的都不一样
                                //   rootAnchor 是一张 图元帧 上真正的 “基准坐标”
                                //   这意味着，为了节约空间，每张图元 不需要对齐，
                                //   只需要各自记好自己的 rootAnchor 值

    //std::vector<AltiRange> altiRanges {};
    AltiRange     altiRange    {0,0};
                                //- 和 colliEnts 对应，记载每一个 collient 携带的 高度区间信息
                                //  暂时只支持 1份。
                                //  等 单个图元帧 可以容纳数个 ces预制件时，此处将被拓展

    int        colliEntSetIdx  {0}; 
                                //- ces预制件 idx号

    //-- 在未来，下方部分 flag 可能会被改成 计数器。
    bool is_rootColliEntOff_set {false};
    bool is_rootAnchorOff_set   {false};
    bool is_altiRange_set       {false};
    bool is_colliEntSetIdx_set  {false};
};


#endif 

