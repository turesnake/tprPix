/*
 * ============= create_a_go_in_field.cpp =================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
//#include <set>
//#include <unordered_map>

//-------------------- Engine --------------------//
#include "sectionBuild_inn.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/create_goes.h"
#include "Script/resource/srcs_script.h" //- tmp


#include "debug.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//

//namespace{//-------- namespace: --------------//
//}//------------- namespace: end --------------//

/* ===========================================================
 *             create_Goes_in_field
 * -----------------------------------------------------------
 * -- 一种临时性的写法，在未来，会用更工整的 ecosys规划法来 实现分配
 */
void create_a_go_in_field( fieldKey_t _fieldKey ){

    MapField    &fieldRef       = esrc::find_or_insert_the_field_ref( _fieldKey );
    EcoSysInMap &ecoSysInMapRef = esrc::get_ecoSysInMapRef( fieldRef.ecoSysInMapKey );
    goSpecId_t  goSpecId;

    float randV = (fieldRef.weight * 0.35 + 313.17); //- 确保大于0
    float fract = randV - floor(randV); //- 小数部分
    assert( (fract>=0.0) && (fract<=1.0) );

    if( fieldRef.is_inland() ){
        goSpecId = ecoSysInMapRef.apply_a_rand_goSpecId( fieldRef.density.get_idx(), fieldRef.weight );
        if( fract <= ecoSysInMapRef.get_applyPercent(fieldRef.density) ){
        gameObjs::create_a_Go(  goSpecId,
                                fieldRef.nodeMPos,
                                fieldRef.weight,
                                fieldRef.nodeAlti, //- tmp 有问题
                                fieldRef.density );
        }
    }
}



}//----------------- namespace: sectionBuild: end -------------------//

