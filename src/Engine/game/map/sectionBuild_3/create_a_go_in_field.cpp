/*
 * ============= create_a_go_in_field.cpp =================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "MapField.h"
#include "esrc_ecoSysInMap.h"
#include "esrc_field.h"
#include "esrc_gameObj.h" 
#include "esrc_chunk.h" 


//-------------------- Script --------------------//
//#include "Script/gameObjs/allGoes.h"
#include "Script/resource/srcs_script.h"
#include "Script/gameObjs/create_goes.h"


//#include "debug.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *             create_Goes_in_field
 * -----------------------------------------------------------
 * -- 
 * 目前被 check_and_build_sections_3.cpp -> build_one_chunk_3() 使用
 *    
 */
void create_a_go_in_field( fieldKey_t _fieldKey ){

    MapField    *fieldPtr       = esrc::atom_get_fieldPtr( _fieldKey );
    EcoSysInMap *ecoSysInMapPtr = esrc::atom_get_ecoSysInMapPtr( fieldPtr->get_ecoSysInMapKey() );
    goSpecId_t  goSpecId;

    float randV = (fieldPtr->get_weight() * 0.35 + 313.17); //- 确保大于0
    float fract = randV - floor(randV); //- 小数部分
    assert( (fract>=0.0) && (fract<=1.0) );

    if( fieldPtr->is_land() ){
        goSpecId = ecoSysInMapPtr->apply_a_rand_goSpecId( fieldPtr->get_density().get_idx(), fieldPtr->get_weight() );
        if( fract <= ecoSysInMapPtr->get_applyPercent(fieldPtr->get_density()) ){
        gameObjs::create_a_Go(    goSpecId,
                        fieldPtr->get_nodeMPos(),
                        fieldPtr->get_weight(),
                        fieldPtr->get_nodeAlti(), //- tmp 有问题
                        fieldPtr->get_density() );
        }
    }
}




}//----------------- namespace: sectionBuild: end -------------------//

