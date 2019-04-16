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

namespace{//-------- namespace: --------------//

    std::uniform_int_distribution<int> uDistribution_regular(0,1000); // [0,1000]




}//------------- namespace: end --------------//

/* ===========================================================
 *             create_Goes_in_field
 * -----------------------------------------------------------
 * -- 一种临时性的写法，在未来，会用更工整的 ecosys规划法来 实现分配
 */
void create_a_go_in_field( fieldKey_t _fieldKey ){

    MapField  &fieldRef         = esrc::find_or_insert_the_field_ref( _fieldKey );
    EcoSysInMap &ecoSysInMapRef = esrc::get_ecoSysInMapRef( fieldRef.ecoSysInMapKey );


    //--- 优先计算，是否种植 mapSurface/bush 层 goes ---
    if( (uDistribution_regular(esrc::gameSeed.randEngine)<400) && 
        fieldRef.is_inland() &&
        (fieldRef.minAlti.val > 30) &&   //- 海拔
        (fieldRef.density.lvl<3) && (fieldRef.density.lvl>-2) ){  //- 密度

        if( ecoSysInMapRef.ecoSysType == EcoSysType::Desert ){

            //- 80% 小麦，20% 石头 --
            if( uDistribution_regular(esrc::gameSeed.randEngine) < 800 ){
                //gameObjs::create_a_Wheat(  fieldRef.nodeMPos, fieldRef.weight );
                gameObjs::create_a_Go(   ssrc::get_goSpecId( "wheat" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );
                    
                     //fieldRef.nodeMPos, fieldRef.weight );
            }else{
                //gameObjs::create_a_SingleStone_Desert(  fieldRef.nodeMPos, fieldRef.weight );
                gameObjs::create_a_Go(   ssrc::get_goSpecId( "singleStone_Desert" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );
            }
            
        }else if( ecoSysInMapRef.ecoSysType == EcoSysType::DarkForest ){

            //- 80% lichen_DForest，20% leaf --
            /*
            if( uDistribution_regular(esrc::gameSeed.randEngine) < 800 ){
                gameObjs::create_a_Lichen_DForest(  fieldRef.nodeMPos, fieldRef.weight );
                        //- 这个lichen的效果很一般, ...
            }else{
                gameObjs::create_a_Leaf_DForest(  fieldRef.nodeMPos, fieldRef.weight );
                        //- 这个叶子的效果很一般, ...
            }
            */

            //gameObjs::create_a_Lichen_Forest(  fieldRef.nodeMPos, fieldRef.weight );
            gameObjs::create_a_Go(   ssrc::get_goSpecId( "lichen_Forest" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );
            
        
        }else{
            //gameObjs::create_a_Lichen_Forest(  fieldRef.nodeMPos, fieldRef.weight );
            gameObjs::create_a_Go(   ssrc::get_goSpecId( "lichen_Forest" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );
        }
        return;
    }


    //-- 没有分配到 地衣时，才会种树 ---
    int randV = uDistribution_regular(esrc::gameSeed.randEngine);
    switch ( fieldRef.density.lvl ){
        case 3:
            if( (randV < 900) &&  // [600/1000]
                fieldRef.is_inland() ){ 

                (ecoSysInMapRef.ecoSysType == EcoSysType::DarkForest) ?
                    gameObjs::create_a_Go(   ssrc::get_goSpecId( "pineTree" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density ) :
                    gameObjs::create_a_Go(   ssrc::get_goSpecId( "oakTree" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );

            }
            break;
        case 2:
            if( (randV < 800) &&  // [400/1000]
                fieldRef.is_inland() ){ 

                (ecoSysInMapRef.ecoSysType == EcoSysType::DarkForest) ?
                    gameObjs::create_a_Go(   ssrc::get_goSpecId( "pineTree" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density ) :
                    gameObjs::create_a_Go(   ssrc::get_goSpecId( "oakTree" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );

            }
            break;
        case 1:
            if( (randV < 200) &&  // [400/1000]
                fieldRef.is_inland() ){ 

                (ecoSysInMapRef.ecoSysType == EcoSysType::DarkForest) ?
                    gameObjs::create_a_Go(   ssrc::get_goSpecId( "pineTree" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density ) :
                    gameObjs::create_a_Go(   ssrc::get_goSpecId( "oakTree" ),
                                                fieldRef.nodeMPos,
                                                fieldRef.weight,
                                                fieldRef.nodeAlti, //- tmp 有问题
                                                fieldRef.density );
            }
            break;
        default:
            break;
    }



}






}//----------------- namespace: sectionBuild: end -------------------//

