/*
 * ========================= FramePos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    animFrameSet 中，单张 图元帧 拥有的 全部 pos数据集
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
#include "MapEntCompass.h"
#include "AnchorPos.h"


//-- 仅用来描述 animFrameSet，所以必须是 静态数据 --//
class FramePos{
public:
    FramePos(){};

    //---- set ----//
    // set 阶段 无需考虑性能
    inline void clear_all(){
        this->rootAnchorPos.clear_all();
        this->rootColliEntHeadIdx = 0;
        this->is_rootAnchorPos_set       = false;
        this->is_rootColliEntHeadIdx_set = false;
        this->colliEntHeads.clear();
    }

    //-- 统一设置 --
    inline void set_rootAnchorPos( const IntVec2 &_rootAnchor, const IntVec2 &_rootCEH ){
        this->rootAnchorPos.pposOff = _rootAnchor;
        this->rootAnchorPos.compass = calc_ppos_compass( _rootAnchor - _rootCEH );
        this->is_rootAnchorPos_set  = true;
    }

    //-- regular ceh --
    inline void pushBack_new_colliEntHead( const ColliEntHead &_ceh ){
        this->colliEntHeads.push_back( _ceh ); //- copy
    }

    //-- root ceh --
    inline void pushBack_the_rootColliEntHead( const ColliEntHead &_ceh ){
        this->rootColliEntHeadIdx = this->colliEntHeads.size();
        this->colliEntHeads.push_back( _ceh ); //- copy
        this->is_rootColliEntHeadIdx_set = true;
    }

    //-- ceh.pposOff_fromRootAnchor 在 pjt_rgba解析器中，只计算到一半
    //   通过本函数，正式计算出这个值
    void calc_ceh_pposOff_fromRootAnchor();

    //-- 检测 rootAnchor偏移 是否与 idx指向的 colliEntSet预制件
    //   中的 center 偏移 吻合 
    void check();

    //---- get ----//
    inline const AnchorPos &get_rootAnchorPos() const {
        return this->rootAnchorPos;
    }

    //-- IMPORTANT !!! --//
    inline const std::vector<ColliEntHead> &get_colliEntHeads() const {
        return this->colliEntHeads;
    }
    

private:
    AnchorPos   rootAnchorPos     {};
    int         rootColliEntHeadIdx  {0};
                                //-- root ceh 在 colliEntHeads容器中的 idx

    //-- 每个 图元帧 拥有一组 ceh  --
    std::vector<ColliEntHead>  colliEntHeads {};
                                //-- 统一存储 所有 colliEntHead 信息
                                //  包括 rootColliEntHeadOff
    
    //-- 在未来，下方部分 flag 可能会被改成 计数器。
    bool is_rootAnchorPos_set       {false};
    bool is_rootColliEntHeadIdx_set {false};
    
};


#endif 

