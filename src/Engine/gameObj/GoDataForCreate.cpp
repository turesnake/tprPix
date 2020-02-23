/*
 * ==================== GoDataForCreate.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoDataForCreate.h"


//------------------- Engine --------------------//
#include "GoSpecFromJson.h"
#include "WindClock.h"

#include "esrc_animFrameSet.h"





void GoDataForCreate::assemble_new_goDataForCreate( GoDataForCreate     &goDRef_,
                                                    const glm::dvec2    &dpos_, // 让外部计算好
                                                    goSpeciesId_t       goSpeciesId_,
                                                    goLabelId_t         goLabelId_,
                                                    NineDirection       direction_,
                                                    BrokenLvl           brokenLvl_,
                                                    size_t mapEntUWeight_
                                                     ){

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDRef_.direction = direction_;
    goDRef_.brokenLvl = brokenLvl_;
    goDRef_.goSpeciesId = goSpeciesId_;
    goDRef_.dpos = dpos_;

    //======
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDRef_.goSpeciesId );
    tprAssert( goSpecFromJsonRef.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJsonRef.goAssemblePlanSetUPtr->apply_a_plan( 
                                                                    goLabelId_,
                                                                    mapEntUWeight_ );


    goDRef_.goAltiRangeLabel = planRef.goAltiRangeLabel;

    size_t randUWeightOff = 0;
    for( const auto &jgomesh : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += 17;

        auto gmUPtr = std::make_unique<GoDataForCreate::GoMesh>( &jgomesh );


        //---
        gmUPtr->set_subspeciesId( esrc::apply_a_random_animSubspeciesId(jgomesh.animFrameSetName,
                                                                        jgomesh.animLabel,
                                                                        mapEntUWeight_ + randUWeightOff) );

        // 未来，这应该是个 选配件
        gmUPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDRef_.dpos + jgomesh.dposOff) );
                                    

        goDRef_.goMeshEntUPtrs.push_back( std::move(gmUPtr) ); // move                                        
            
    }  

}








