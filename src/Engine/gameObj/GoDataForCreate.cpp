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
#include "random.h"

#include "esrc_animFrameSet.h"


// 将目标 GoSpecFromJson 中的数据，配合部分 蓝图/手动 数据
// 装配出一个 完整的 GoDataForCreate 数据
std::unique_ptr<GoDataForCreate> GoDataForCreate::assemble_new_goDataForCreate( 
                                                                    IntVec2             mpos_,
                                                                    const glm::dvec2    &dpos_,
                                                                    goSpeciesId_t       goSpeciesId_,
                                                                    goLabelId_t         goLabelId_,
                                                                    //std::optional<NineDirection> direction_,
                                                                    //std::optional<BrokenLvl>     brokenLvl_
                                                                    NineDirection direction_,
                                                                    BrokenLvl     brokenLvl_

                                                                    ){

    std::unique_ptr<GoDataForCreate> goDUPtr ( new GoDataForCreate() ); // can't use std::make_unique


    size_t mapEntUWeight = calc_simple_uWeight( mpos_ );

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDUPtr->direction = direction_;
    goDUPtr->brokenLvl = brokenLvl_;

    goDUPtr->goSpeciesId = goSpeciesId_;
    goDUPtr->goLabelId = goLabelId_;
    goDUPtr->dpos = dpos_;
    goDUPtr->uWeight = mapEntUWeight;

    //======
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDUPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goLabelId_, mapEntUWeight );

    goDUPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    tprAssert( planRef.colliDataFromJsonUPtr );
    goDUPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    size_t randUWeightOff = mapEntUWeight;
    for( const auto &jgomesh : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += 17;

        auto gmUPtr = std::make_unique<GoDataForCreate::GoMesh>( &jgomesh, randUWeightOff );
        //---
        //gmUPtr->
        gmUPtr->set_subspeciesId( esrc::apply_a_random_animSubspeciesId(jgomesh.animFrameSetName,
                                                                        jgomesh.animLabel,
                                                                        randUWeightOff) );

        // 未来，这应该是个 选配件
        gmUPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDUPtr->dpos + jgomesh.dposOff) );
                                    
        goDUPtr->goMeshEntUPtrs.push_back( std::move(gmUPtr) ); // move                                        
    }  
    //===
    return goDUPtr;
}




