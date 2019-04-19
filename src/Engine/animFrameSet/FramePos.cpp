/*
 * ========================= FramePos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "FramePos.h"

//-------------------- C --------------------//
#include <cassert>

//------------------- Engine --------------------//
#include "config.h" 
#include "esrc_colliEntSet.h"


//#include "debug.h" 



/* ===========================================================
 *             calc_ceh_pposOff_fromRootAnchor
 * -----------------------------------------------------------
 */
void FramePos::calc_ceh_pposOff_fromRootAnchor(){
    assert( this->is_rootAnchorPos_set == true );
    for( auto &i : this->colliEntHeads ){
        i.pposOff_fromRootAnchor -= this->rootAnchorPos.pposOff;
    }
}


/* ===========================================================
 *                      check
 * -----------------------------------------------------------
 */
void FramePos::check(){

    //--- 确保关键数据已载入 ---//
    assert( this->is_rootAnchorPos_set &&
            this->is_rootColliEntHeadIdx_set &&
            (!this->colliEntHeads.empty()) );
    
    //-- 检测 animFrameSet.rootAnchor 与 root ceh 是否对齐 --//
    ColliEntHead &rootCehRef = this->colliEntHeads.at(this->rootColliEntHeadIdx);
    //--
    const MapEntCompass &centerCompass = esrc::colliEntSets.at( rootCehRef.colliEntSetIdx ).get_centerCompass();
    assert( centerCompass == this->rootAnchorPos.compass ); //- 确保对齐 ！！！ --
                // 这一对齐，将使的，所有 frame中，rootAnchorPos点 都必须正好位于 其 ces 指定的 center 点上...

    //-- 检测 regular ceh 是否与 root ceh 对齐 --//
    IntVec2 pposOff;
    for( size_t i=0; i<this->colliEntHeads.size(); i++ ){

        if( i == this->rootColliEntHeadIdx ){
            continue;
        }

        pposOff = this->colliEntHeads.at(i).pposOff_fromRootAnchor - rootCehRef.pposOff_fromRootAnchor;
        assert( (pposOff.x%PIXES_PER_MAPENT==0) && (pposOff.y%PIXES_PER_MAPENT==0) );
    }
}

