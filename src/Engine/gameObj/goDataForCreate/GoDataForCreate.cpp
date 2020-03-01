/*
 * ==================== GoDataForCreate.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
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

#include "tprDebug.h"

extern const std::string &calc_groundGoMeshName( FieldFractType fieldFractType_, colorTableId_t colorTableId_ )noexcept;


// [-RegularGo-]
std::unique_ptr<GoDataForCreate> GoDataForCreate::create_new_goDataForCreate( 
                                                                    IntVec2             mpos_,
                                                                    const glm::dvec2    &dpos_,
                                                                    goSpeciesId_t       goSpeciesId_,
                                                                    goLabelId_t         goLabelId_,
                                                                    //std::optional<NineDirection> direction_,
                                                                    //std::optional<BrokenLvl>     brokenLvl_
                                                                    NineDirection direction_,
                                                                    BrokenLvl     brokenLvl_
                                                                    ){

    auto goDUPtr = std::make_unique<GoDataForCreate>();

    size_t mapEntUWeight = calc_simple_uWeight( mpos_ );

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDUPtr->direction = direction_;
    goDUPtr->brokenLvl = brokenLvl_;

    goDUPtr->goSpeciesId = goSpeciesId_;
    goDUPtr->goLabelId = goLabelId_;
    goDUPtr->dpos = dpos_;
    goDUPtr->uWeight = mapEntUWeight;
    tprAssert( goDUPtr->uWeight != 0 );

    //======
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDUPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goLabelId_, mapEntUWeight );

    goDUPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    tprAssert( planRef.colliDataFromJsonUPtr );
    goDUPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    size_t randUWeightOff = mapEntUWeight;
    for( const auto &[jgomeshName, jgomesh] : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += 17;

        // 有些 gomesh 不支持在 init 阶段被 创建
        if( !jgomesh.isAutoInit ){
            continue;
        }

        auto gmUPtr = std::make_unique<GoDataForCreate::GoMeshByLink>( &jgomesh, randUWeightOff );

        // 未来，这应该是个 选配件
        gmUPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDUPtr->dpos + jgomesh.dposOff) );
                                    
        goDUPtr->goMeshEntUPtrs.push_back( std::move(gmUPtr) ); // move                                        
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

    size_t mapEntUWeight = calc_simple_uWeight( mpos_ );

    //--- 为 GoDataForCreate 实例 装填数据 --
    goDUPtr->direction = direction_;
    goDUPtr->brokenLvl = BrokenLvl::Lvl_0; // meaningless

    goDUPtr->goSpeciesId = goSpeciesId_;
    goDUPtr->goLabelId = goLabelId_;
    goDUPtr->dpos = dpos_;
    goDUPtr->uWeight = mapEntUWeight;
    tprAssert( goDUPtr->uWeight != 0 );

    //======
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDUPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goLabelId_, mapEntUWeight );

    goDUPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    tprAssert( planRef.colliDataFromJsonUPtr );
    goDUPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    size_t randUWeightOff = mapEntUWeight;
    for( const auto &[jgomeshName, jgomesh] : planRef.gomeshs ){ // each json goMesh
        randUWeightOff += 17;

        // 有些 gomesh 不支持在 init 阶段被 创建
        if( !jgomesh.isAutoInit ){
            continue;
        }
        auto gmUPtr = std::make_unique<GoDataForCreate::GoMeshByHand>( &jgomesh, randUWeightOff );
        
        //------------------------//
        //     IMPORTANT!!!
        // calc gomesh.zOff, based on floorGoLayer
        //------------------------//
        tprAssert( jgomesh.floorGoLayer.has_value() );
        gmUPtr->zOff = calc_floorGoMesh_zOff( jgomesh.floorGoLayer.value(), randUWeightOff );

        // 未来，这应该是个 选配件
        gmUPtr->set_windDelayIdx( calc_goMesh_windDelayIdx(goDUPtr->dpos + jgomesh.dposOff) );
                                    
        goDUPtr->goMeshEntUPtrs.push_back( std::move(gmUPtr) ); // move                                        
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
    double halfPixesPerField = static_cast<double>(PIXES_PER_FIELD) * 0.5;
    //===   
    auto goDUPtr = std::make_unique<GoDataForCreate>();
    GoDataForCreate *goDataPtr = goDUPtr.get();
    //---
    goDataPtr->goSpeciesId = GoSpecFromJson::str_2_goSpeciesId("groundGo"),
    goDataPtr->goLabelId = GoAssemblePlanSet::str_2_goLabelId(""); // 其实是错的，GroundGo 压根就没有 asm 分配方案
    goDataPtr->dpos = mpos_2_dpos(fieldMPos) + glm::dvec2{ halfPixesPerField, halfPixesPerField };
    goDataPtr->direction = NineDirection::Center;
    goDataPtr->brokenLvl = BrokenLvl::Lvl_0;
    goDataPtr->uWeight = jobFieldRef_.get_leftBottomMapEnt_uWeight(); // 暂时等于 左下角 mapent.uWeight
    tprAssert(goDataPtr->uWeight != 0  );

    // find plan
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goDataPtr->goLabelId, goDataPtr->uWeight );
    //---
    goDataPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    goDataPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();
    //=== goMeshs
    // 根据 groundGoEnts 数据，动态创建
    size_t goMeshIdx {0};
    std::string specialGoMeshName {};
    size_t randUWeightOff = goDataPtr->uWeight;
    for( const auto &uptr : groundGoEnts_ ){
        const Job_GroundGoEnt &job_groundGoEntRef = *uptr;
        randUWeightOff += 17;

        std::string goMeshName = calc_groundGoMeshName( job_groundGoEntRef.fieldFractType, job_groundGoEntRef.colorTableId );
              
        const GoAssemblePlanSet::GoMeshEnt &gmeRef = planRef.get_goMeshEntRef( goMeshName );

        // 手动生成 goMeshName, 防止重名
        if( goMeshIdx == 0 ){
            specialGoMeshName = "root";
        }else{
            specialGoMeshName = tprGeneral::nameString_combine("m_", goMeshIdx, "");
        }

        auto goMeshUPtr = std::make_unique<GoDataForCreate::GoMeshByHand>( &gmeRef, randUWeightOff );
        //---
        goMeshUPtr->goMeshName = specialGoMeshName;
        goMeshUPtr->dposOff = job_groundGoEntRef.dposOff;
        goMeshUPtr->zOff = calc_uWeight_fractValue(randUWeightOff); // (0.0, 1.0)

        goMeshUPtr->isVisible = true;
        goMeshUPtr->isAutoInit = true; // meaningless
    
        goMeshUPtr->set_windDelayIdx( 1 ); // meaningless
    
        //---
        goDataPtr->goMeshEntUPtrs.push_back( std::move( goMeshUPtr ) );
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


