/*
 * ======================= Section.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#include "Section.h"

//-------------------- C --------------------//
#include <cassert>
#include <math.h>

//-------------------- Engine --------------------//
#include "random.h"
#include "EcoSysInMap.h"
#include "srcs_engine.h"
#include "Quad.h"


#include "debug.h"

namespace{//-------- namespace: --------------//

                                          
    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },
        IntVec2{ 1, 0 },
        IntVec2{ 0, 1 },
        IntVec2{ 1, 1 }
    };

}//------------- namespace: end --------------//



/* ===========================================================
 *                     init
 * -----------------------------------------------------------
 */
void Section::init(){
    
    init_quadSectionKeys();
}


/* ===========================================================
 *               init_quadSectionKeys
 * -----------------------------------------------------------
 * -- 填充 quadSectionKeys
 *    共4个元素
 */
void Section::init_quadSectionKeys(){
    if( this->is_quadSectionKeys_set ){
        return;
    }
    //-----
    IntVec2        mpos = this->get_mpos();
    IntVec2        tmpMPos;
    sectionKey_t   tmpKey;
    quadSectionKeys.clear();
    for( const auto &whOff : quadSectionKeyOffs ){
        tmpMPos.x = mpos.x + whOff.x*ENTS_PER_SECTION;
        tmpMPos.y = mpos.y + whOff.y*ENTS_PER_SECTION;
        tmpKey = sectionMPos_2_sectionKey( tmpMPos );
        //---
        quadSectionKeys.push_back( tmpKey );//- copy
    }
    assert( quadSectionKeys.size() == QUAD_NUM );
    this->is_quadSectionKeys_set = true;
}



/* ===========================================================
 *                bind_ecoSysInMapPtrs
 * -----------------------------------------------------------
 * -- 绑定 4个端点的 ecosys 指针。
 *   调用之前，应确保 目标 ecosysInMap 实例 已经创建了
 */
void Section::bind_ecoSysInMapPtrs(){
    if( this->is_ecoSysInMapPtrs_set ){
        return;
    }
    assert( this->is_quadSectionKeys_set );//- tmp
    //---
    EcoSysInMap  *ecoSysInMapPtr;
    ecoSysInMapPtrs.clear();
    for( const auto &key : quadSectionKeys ){ //- each key val
        ecoSysInMapPtr = esrc::get_ecoSysInMapPtr(key);
        ecoSysInMapPtrs.push_back( ecoSysInMapPtr ); //- copy        
    }
    this->is_ecoSysInMapPtrs_set = true;
}

