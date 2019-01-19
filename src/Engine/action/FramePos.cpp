/*
 * ========================= FramePos.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.15
 *                                        修改 -- 2019.01.15
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "FramePos.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>

//------------------- Engine --------------------//
#include "config.h" 
#include "srcs_engine.h"


using std::string;

#include "debug.h" 


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 */
void FramePos::check(){
    
    //-- 确保关键数据已经载入完毕 --//
    
    assert( is_rootAnchorOff_set && 
            is_rootColliEntHeadIdx_set &&
            (!colliEntHeads.empty()) );
    

    //-- 检测 action.rootAnchor 与 root ceh 是否对齐 --//
    ColliEntHead &rootCehRef = colliEntHeads.at(rootColliEntHeadIdx);
    //--
    PixVec2 center_ppos     = esrc::colliEntSets.at( rootCehRef.colliEntSetIdx ).get_center();
    PixVec2 rootAnchor_ppos = rootAnchorOff - rootCehRef.colliEntHeadPPosOff;
    assert( center_ppos == rootAnchor_ppos ); //- 确保对齐 ！！！ --


    //-- 检测 regular ceh 是否与 root ceh 对齐 --//
    PixVec2 pposOff;
    for( size_t i=0; i<colliEntHeads.size(); i++ ){

        if( i == rootColliEntHeadIdx ){
            continue;
        }

        pposOff = colliEntHeads.at(i).colliEntHeadPPosOff - rootCehRef.colliEntHeadPPosOff;
        assert( (pposOff.x%PIXES_PER_MAPENT==0) && (pposOff.y%PIXES_PER_MAPENT==0) );
    }
}

