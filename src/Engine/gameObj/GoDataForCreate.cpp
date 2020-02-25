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


// 将目标 GoSpecFromJson 中的数据，配合部分 蓝图/手动 数据
// 装配出一个 完整的 GoDataForCreate 数据
std::unique_ptr<GoDataForCreate> GoDataForCreate::assemble_new_goDataForCreate( 
                                                    const glm::dvec2    &dpos_, // 让外部计算好
                                                    goSpeciesId_t       goSpeciesId_,
                                                    goLabelId_t         goLabelId_,
                                                    NineDirection       direction_,
                                                    BrokenLvl           brokenLvl_,
                                                    size_t              mapEntUWeight_ 
                                                     ){

    std::unique_ptr<GoDataForCreate> goDUPtr ( new GoDataForCreate() ); // can't use std::make_unique

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDUPtr->direction = direction_;
    goDUPtr->brokenLvl = brokenLvl_;
    goDUPtr->goSpeciesId = goSpeciesId_;
    goDUPtr->dpos = dpos_;

    //======
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDUPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goLabelId_,mapEntUWeight_ );

    goDUPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    goDUPtr->colliDataFromJpngPtr = planRef.colliDataFromJpngUPtr.get();

    size_t randUWeightOff = 0;
    for( const auto &jgomesh : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += 17;

        auto gmUPtr = std::make_unique<GoDataForCreate::GoMesh>( &jgomesh );
        //---
        gmUPtr->set_subspeciesId( esrc::apply_a_random_animSubspeciesId(jgomesh.animFrameSetName,
                                                                        jgomesh.animLabel,
                                                                        mapEntUWeight_ + randUWeightOff) );

        // 未来，这应该是个 选配件
        gmUPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDUPtr->dpos + jgomesh.dposOff) );
                                    
        goDUPtr->goMeshEntUPtrs.push_back( std::move(gmUPtr) ); // move                                        
    }  
    //===
    return goDUPtr;
}




