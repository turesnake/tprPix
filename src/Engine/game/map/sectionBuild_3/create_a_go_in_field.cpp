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


#include "debug.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//

namespace{//-------- namespace: --------------//

    std::uniform_int_distribution<int> uDistribution_regular(0,1000); // [0,1000]




}//------------- namespace: end --------------//

/* ===========================================================
 *             create_Goes_in_field
 * -----------------------------------------------------------
 */
void create_a_go_in_field( fieldKey_t _fieldKey ){

    MapField  *fieldPtr = esrc::find_or_insert_the_field( _fieldKey );
    EcoSysInMap *ecoSysInMapPtr = esrc::get_ecoSysInMapPtr( fieldPtr->ecoSysInMapKey );


    //--- 优先计算，是否种植 mapSurface/bush 层 goes ---
    if( (uDistribution_regular(esrc::gameSeed.randEngine)<400) && 
        fieldPtr->is_inland() &&
        (fieldPtr->minAlti.val > 30) &&   //- 海拔
        (fieldPtr->density.lvl<3) && (fieldPtr->density.lvl>-2) ){  //- 密度

        if( ecoSysInMapPtr->ecoSysType == EcoSysType::Desert ){
            gameObjs::create_a_Wheat(  fieldPtr->nodeMPos, fieldPtr->weight );
        }else{
            gameObjs::create_a_Lichen(  fieldPtr->nodeMPos, fieldPtr->weight );
        }
        return;
    }


    //-- 没有分配到 地衣时，才会种树 ---
    bool isSingleTrunk = (uDistribution_regular(esrc::gameSeed.randEngine) < 500);
    bool isFlipOver = (uDistribution_regular(esrc::gameSeed.randEngine) < 500);
    int randV = uDistribution_regular(esrc::gameSeed.randEngine);
    switch ( fieldPtr->density.lvl ){
        case 3:
            if( (randV < 900) &&  // [600/1000]
                fieldPtr->is_inland() ){ 
                gameObjs::create_a_OakTree( fieldPtr->nodeMPos, 
                                            3, isSingleTrunk, isFlipOver );
            }
            break;
        case 2:
            if( (randV < 800) &&  // [400/1000]
                fieldPtr->is_inland() ){ 
                gameObjs::create_a_OakTree( fieldPtr->nodeMPos, 
                                            2, isSingleTrunk, isFlipOver );
            }
            break;
        case 1:
            if( (randV < 200) &&  // [400/1000]
                fieldPtr->is_inland() ){ 
                gameObjs::create_a_OakTree( fieldPtr->nodeMPos, 
                                            1, isSingleTrunk, isFlipOver );
            }
            break;
        default:
            break;
    }



}






}//----------------- namespace: sectionBuild: end -------------------//

