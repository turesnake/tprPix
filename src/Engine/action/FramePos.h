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

//#include "debug.h" 


class FramePos{
public:
    FramePos(){};

    //---- set ----//
    // set 阶段 无需考虑性能
    inline void clear_all(){
        rootColliEntOff = PixVec2{ 0, 0};
        rootAnchorOff   = PixVec2{ 0, 0};
        colliEntOffs.clear();
        is_rootColliEntOff_set = false;
        is_rootAnchorOff_set = false;
    }

    inline void set_rootColliEntOff( const PixVec2 &_v ){
        rootColliEntOff = _v;
        is_rootColliEntOff_set = true;
    }

    inline void set_rootAnchorOff( const PixVec2 &_v  ){
        rootAnchorOff  = _v;
        is_rootAnchorOff_set = true;
    }

    //-- 输入的是 相对于 图元帧 左下角的 绝对 ppos
    //  called by action.init() 
    inline void colliEntOffs_push_back( const PixVec2 &_abs_ppos ){
        assert( is_rootColliEntOff_set == true );
        MapCoord pos;
        pos.set_by_ppos(_abs_ppos.x-rootColliEntOff.x,
                        _abs_ppos.y-rootColliEntOff.y );
        colliEntOffs.push_back( pos ); //- copy 
    }

    //  called by action.init() 
    inline void altiRanges_push_back( const AltiRange &_ar ){
        altiRanges.push_back( _ar  ); //- copy -
    }


    //---- get ----//
    inline const PixVec2 &get_rootAnchorOff() const {
        return rootAnchorOff;
    }

    //-- debug --
    /*
    inline void print_colliEnts() const {
        cout << "-------" << endl;
        for( const auto &i : colliEnts ){
            PixVec2 ppos = i.get_ppos();
            cout << "" << ppos.x
                << ", " << ppos.y
                << endl;
        }
    }
    */

private:

    PixVec2     rootColliEntOff  {0,0}; 
                                //-- 从图元帧 左下角，到 rootColliEnt 的 pos偏移值
                                //   [-可以是任意 整形数，不用对齐于 mapEnt-]
                                //   rootColliEnt 在概念上是个点，
                                //   它是 所有 colliEnts 的左下角
    
    PixVec2     rootAnchorOff     {0,0};
                                //-- 从图元帧 左下角，到 rootAnchor 的 pos偏移值
                                //   [-可以是任意 整形数，不用对齐于 mapEnt-]
                                                                
    std::vector<MapCoord>  colliEntOffs {};
                                //- 当前图元帧，所有 colliEnts 相对于 rootColliEnt 的偏移 
                                //  由于这些 偏移一定是 基于 mapEnt 对齐的
                                //  所以单位类型设置为 MapCoord

    std::vector<AltiRange> altiRanges {};
                                //- 和 colliEnts 对应，记载每一个 collient 携带的
                                //  高度区间 信息

    bool is_rootColliEntOff_set {false};
    bool is_rootAnchorOff_set   {false};
};







#endif 

