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


//-------------------- Script --------------------//
#include "Script/gameObjs/create_goes.h"
#include "Script/resource/srcs_script.h" //- tmp


//#include "debug.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *             create_Goes_in_field
 * -----------------------------------------------------------
 * -- 已使用正式的 规划法来分配
 */
void create_a_go_in_field( fieldKey_t _fieldKey ){

    MapField    &fieldRef       = esrc::find_or_insert_the_field_ref( _fieldKey );
    EcoSysInMap &ecoSysInMapRef = esrc::get_ecoSysInMapRef( fieldRef.get_ecoSysInMapKey() );
    goSpecId_t  goSpecId;

    float randV = (fieldRef.get_weight() * 0.35 + 313.17); //- 确保大于0
    float fract = randV - floor(randV); //- 小数部分
    assert( (fract>=0.0) && (fract<=1.0) );

    if( fieldRef.is_land() ){
        goSpecId = ecoSysInMapRef.apply_a_rand_goSpecId( fieldRef.get_density().get_idx(), fieldRef.get_weight() );
        if( fract <= ecoSysInMapRef.get_applyPercent(fieldRef.get_density()) ){
        gameObjs::create_a_Go(  goSpecId,
                                fieldRef.get_nodeMPos(),
                                fieldRef.get_weight(),
                                fieldRef.get_nodeAlti(), //- tmp 有问题
                                fieldRef.get_density() );
        }
    }
}



}//----------------- namespace: sectionBuild: end -------------------//

