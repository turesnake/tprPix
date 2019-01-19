/*
 * ========================= FramePos.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *    action 中，单张 图元帧 拥有的 全部 pos数据集
 *  ----
 *  -1- rootColliEntsOff
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
#include "ColliEntHead.h"



//-- 仅用来描述 action，所以必须是 静态数据 --//
class FramePos{
public:
    FramePos(){};

    //---- set ----//
    // set 阶段 无需考虑性能
    inline void clear_all(){
        rootAnchorOff.clear_all();
        rootColliEntHeadIdx = 0;
        is_rootAnchorOff_set       = false;
        is_rootColliEntHeadIdx_set = false;
        colliEntHeads.clear();
    }

    inline void set_rootAnchorOff( const PixVec2 &_v  ){
        rootAnchorOff  = _v;
        is_rootAnchorOff_set = true;
    }
    
    //-- regular ceh --
    inline void pushBack_new_colliEntHead( const ColliEntHead &_ceh ){
        colliEntHeads.push_back( _ceh ); //- copy
    }

    //-- root ceh --
    inline void pushBack_the_rootColliEntHead( const ColliEntHead &_ceh ){
        rootColliEntHeadIdx = colliEntHeads.size();
        colliEntHeads.push_back( _ceh ); //- copy
        is_rootColliEntHeadIdx_set = true;
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
    //-- 单个 图元帧 只有一个 --
    PixVec2     rootAnchorOff     {0,0};
                                //-- 从图元帧 左下角，到 rootAnchor 的 pos偏移值
                                //   [-可以是任意 整形数，不用对齐于 mapEnt-]
                                //   每一帧的都不一样
                                //   rootAnchor 是一张 图元帧 上真正的 “基准坐标”
                                //   这意味着，为了节约空间，每张图元 不需要对齐，
                                //   只需要各自记好自己的 rootAnchor 值

    //-- 单个 图元帧 只有一个 -- 
    int         rootColliEntHeadIdx  {0};
                                //-- root ceh 在 colliEntHeads容器中的 idx

    //-- 单个 图元帧 拥有一组  --
    std::vector<ColliEntHead>  colliEntHeads {};
                                //-- 统一存储 所有 colliEntHead 信息
                                //  包括 rootColliEntHeadOff

    //-- 在未来，下方部分 flag 可能会被改成 计数器。
    bool is_rootAnchorOff_set       {false};
    bool is_rootColliEntHeadIdx_set {false};
    

};


#endif 

