/*
 * ==================== GoDataForCreate.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GoDataForCreate.h"

//-------------------- Libs --------------------//
#include "tprGeneral.h"

//------------------- Engine --------------------//
#include "GoSpecFromJson.h"
#include "WindClock.h"
#include "random.h"

#include "Job_GroundGoEnt.h"
#include "Job_Field.h"

#include "esrc_animFrameSet.h"


extern const std::string &calc_groundGoMeshName( FieldFractType fieldFractType_, colorTableId_t colorTableId_ )noexcept;



namespace gdfc_inn {//----------- namespace: gdfc_inn ----------------//

    // 即便是 在同一个 mapent 上生成数个go，它们的 goUWeight 也不一样
    size_t calc_go_uWeight( IntVec2 mpos_, goSpeciesId_t goSpeciesId_, goLabelId_t goLabelId_, size_t extra_ )noexcept{
        size_t mapEntUWeight = calc_simple_mapent_uWeight( mpos_ );
        size_t u =  mapEntUWeight ^ 
                    ( static_cast<size_t>(goSpeciesId_) << 1) ^ 
                    ( static_cast<size_t>(goLabelId_)   << 2);
                        // 不使用 cast_2_size_t, 因为不关心 参数是否为 正负
                        // maybe stackOverFlow, but not care
        return (u + extra_);
    }


    size_t calc_goMesh_uWeightStep( size_t goUWeight_ )noexcept{
        return (goUWeight_ % 1031);
                        // 计算出来的返回值，不一定是 素数，是这种实现的缺点
    }


}//-------------- namespace: gdfc_inn end ----------------//



// [-RegularGo-]
std::unique_ptr<GoDataForCreate> GoDataForCreate::create_new_goDataForCreate( 
                                                                    IntVec2             mpos_,
                                                                    const glm::dvec2    &dpos_,
                                                                    goSpeciesId_t       goSpeciesId_,
                                                                    goLabelId_t         goLabelId_,
                                                                    NineDirection       direction_,
                                                                    BrokenLvl           brokenLvl_,
                                                                    size_t              ExtraGoUWeight_
                                                                    ){

    auto goDUPtr = std::make_unique<GoDataForCreate>();

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDUPtr->direction = direction_;
    goDUPtr->brokenLvl = brokenLvl_;

    goDUPtr->goSpeciesId = goSpeciesId_;
    goDUPtr->goLabelId = goLabelId_;
    goDUPtr->dpos = dpos_;
    goDUPtr->goUWeight = gdfc_inn::calc_go_uWeight( mpos_, goSpeciesId_, goLabelId_, ExtraGoUWeight_ );
    tprAssert( goDUPtr->goUWeight != 0 );

    //======
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDUPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goLabelId_, goDUPtr->goUWeight );

    goDUPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    tprAssert( planRef.colliDataFromJsonUPtr );
    goDUPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    //-------------------//
    //      goMeshs
    //-------------------//
    size_t uWeightStep = gdfc_inn::calc_goMesh_uWeightStep( goDUPtr->goUWeight );
    size_t randUWeightOff = goDUPtr->goUWeight;
    for( const auto &[jgomeshName, jgomesh] : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += uWeightStep;

        auto gmSPtr = std::make_shared<GoDataForCreate::GoMeshByLink>( &jgomesh, randUWeightOff );

        // 未来，这应该是个 选配件
        gmSPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDUPtr->dpos + jgomesh.dposOff) );
                                       
        (jgomesh.isAutoInit) ?
            goDUPtr->goMeshs_autoInit.push_back( std::move(gmSPtr) ) :
            goDUPtr->goMeshs_notAutoInit.push_back( std::move(gmSPtr) ); // move
    }  
    //===
    return goDUPtr;
}


// [-FloorGo-]
// 和普通版的唯一区别就是：需要特殊计算 gomesj.zOff
std::unique_ptr<GoDataForCreate> GoDataForCreate::create_new_floorGoDataForCreate( 
                                                                    IntVec2             mpos_,
                                                                    const glm::dvec2    &dpos_,
                                                                    goSpeciesId_t       goSpeciesId_,
                                                                    goLabelId_t         goLabelId_,
                                                                    NineDirection       direction_
                                                                    ){

    auto goDUPtr = std::make_unique<GoDataForCreate>();

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDUPtr->direction = direction_;
    goDUPtr->brokenLvl = BrokenLvl::Lvl_0; // meaningless

    goDUPtr->goSpeciesId = goSpeciesId_;
    goDUPtr->goLabelId = goLabelId_;
    goDUPtr->dpos = dpos_;
    goDUPtr->goUWeight = gdfc_inn::calc_go_uWeight( mpos_, goSpeciesId_, goLabelId_, 0 ); // no ExtraGoUWeight
    tprAssert( goDUPtr->goUWeight != 0 );

    //======
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDUPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goLabelId_, goDUPtr->goUWeight );

    goDUPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    tprAssert( planRef.colliDataFromJsonUPtr );
    goDUPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    //-------------------//
    //      goMeshs
    //-------------------//
    size_t uWeightStep = gdfc_inn::calc_goMesh_uWeightStep( goDUPtr->goUWeight );
    size_t randUWeightOff = goDUPtr->goUWeight;
    for( const auto &[jgomeshName, jgomesh] : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += uWeightStep;

        auto gmSPtr = std::make_shared<GoDataForCreate::GoMeshByHand>( &jgomesh, randUWeightOff );
        
        //------------------------//
        //     IMPORTANT!!!
        // calc gomesh.zOff, based on floorGoLayer
        //------------------------//
        tprAssert( jgomesh.floorGoLayer.has_value() );
        gmSPtr->zOff = calc_floorGoMesh_zOff( jgomesh.floorGoLayer.value(), randUWeightOff );

        // 未来，这应该是个 选配件
        gmSPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDUPtr->dpos + jgomesh.dposOff) );
                                    
        (jgomesh.isAutoInit) ?
            goDUPtr->goMeshs_autoInit.push_back( std::move(gmSPtr) ) :
            goDUPtr->goMeshs_notAutoInit.push_back( std::move(gmSPtr) ); // move                                       
    }  
    //===
    return goDUPtr;
}


// [-GroundGo-]
// [*job-thread*]
std::unique_ptr<GoDataForCreate> GoDataForCreate::create_new_groundGoDataForCreate( 
                                                        const Job_Field &jobFieldRef_,
                                                        const std::vector<std::unique_ptr<Job_GroundGoEnt>> &groundGoEnts_ 
                                                        ){

   

    IntVec2 fieldMPos = fieldKey_2_mpos(jobFieldRef_.get_fieldKey());
    double halfPixesPerField = PIXES_PER_FIELD_D * 0.5;
    //===   
    auto goDUPtr = std::make_unique<GoDataForCreate>();
    GoDataForCreate *goDataPtr = goDUPtr.get();
    //---
    goDataPtr->goSpeciesId = GoSpecFromJson::str_2_goSpeciesId("groundGo"),
    goDataPtr->goLabelId = GoAssemblePlanSet::str_2_goLabelId(""); // 其实是错的，GroundGo 压根就没有 asm 分配方案
    goDataPtr->dpos = mpos_2_dpos(fieldMPos) + glm::dvec2{ halfPixesPerField, halfPixesPerField };
    goDataPtr->direction = NineDirection::Center;
    goDataPtr->brokenLvl = BrokenLvl::Lvl_0;
    goDataPtr->goUWeight = gdfc_inn::calc_go_uWeight(jobFieldRef_.get_fieldMidMPos(), goDataPtr->goSpeciesId, goDataPtr->goLabelId, 0 );// no ExtraGoUWeight
    tprAssert(goDataPtr->goUWeight != 0  );

    // find plan
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goDataPtr->goLabelId, goDataPtr->goUWeight );
    //---
    goDataPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    goDataPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    //-------------------//
    //      goMeshs
    //-------------------//
    size_t uWeightStep = gdfc_inn::calc_goMesh_uWeightStep( goDataPtr->goUWeight );

    // 根据 groundGoEnts 数据，动态创建
    size_t goMeshIdx {0};
    std::string specialGoMeshName {};
    size_t randUWeightOff = goDataPtr->goUWeight;
    for( const auto &uptr : groundGoEnts_ ){
        const Job_GroundGoEnt &job_groundGoEntRef = *uptr;
        randUWeightOff += uWeightStep;

        std::string goMeshName = calc_groundGoMeshName( job_groundGoEntRef.fieldFractType, job_groundGoEntRef.colorTableId );
              
        const GoAssemblePlanSet::GoMeshEnt &gmeRef = planRef.get_goMeshEntRef( goMeshName );

        // 手动生成 goMeshName, 防止重名
        if( goMeshIdx == 0 ){
            specialGoMeshName = "root";
        }else{
            specialGoMeshName = tprGeneral::nameString_combine("m_", goMeshIdx, "");
        }

        auto goMeshSPtr = std::make_shared<GoDataForCreate::GoMeshByHand>( &gmeRef, randUWeightOff );
        //---
        goMeshSPtr->goMeshName = specialGoMeshName;
        goMeshSPtr->dposOff = job_groundGoEntRef.dposOff;
        goMeshSPtr->zOff = calc_uWeight_fractValue(randUWeightOff); // (0.0, 1.0)

        goMeshSPtr->isVisible = true;
        goMeshSPtr->isAutoInit = true; // meaningless
    
        goMeshSPtr->set_windDelayIdx( 1 ); // meaningless
    
        //---
        goDUPtr->goMeshs_autoInit.push_back( std::move(goMeshSPtr) ); // move 

        //===
        goMeshIdx++;
    }
    //===
    return goDUPtr;

}






void GoDataForCreate::GoMeshByLink::init_subspeciesId()noexcept{
    this->subspeciesId = esrc::apply_a_random_animSubspeciesId( this->goMeshEntPtr->animFrameSetName,
                                                                this->goMeshEntPtr->animLabel,
                                                                this->uWeight );
}


void GoDataForCreate::GoMeshByHand::init_subspeciesId( const std::string &animFrameSetName_, const std::string &label_, size_t uWeight_ )noexcept{   
    this->subspeciesId = esrc::apply_a_random_animSubspeciesId( animFrameSetName_, label_, uWeight_ );
}


