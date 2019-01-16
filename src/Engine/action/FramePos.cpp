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

//-------------------- CPP --------------------//
#include <string>

//------------------- Engine --------------------//
#include "srcs_engine.h"


using std::string;

#include "debug.h" 


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 */
void FramePos::check(){
    assert( is_rootColliEntOff_set && 
            is_rootAnchorOff_set &&
            is_altiRange_set &&
            is_colliEntSetIdx_set );

    string name = esrc::colliEntSet_idx_name.at( colliEntSetIdx );
    PixVec2 center_ppos = esrc::colliEntSets.at(name).get_center();

    PixVec2 rootAnchor_ppos = rootAnchorOff - rootColliEntOff;

    assert( center_ppos == rootAnchor_ppos ); //- 确保对齐 ！！！ --
}

