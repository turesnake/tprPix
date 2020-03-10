/*
 * ====================== assemble_go.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "assemble_go.h"

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"
#include "GoSpecFromJson.h"
#include "dyParams.h"


// 被 具象go类 调用，将 GoDataForCreate 中的数据，配置进 regularGo 中
const GoDataForCreate *assemble_regularGo( GameObj &goRef_,const DyParam &dyParams_ ){

    //================ dyParams =================//
    // 假定都使用 DyParams_GoDataForCreate
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_GoDataForCreate).hash_code() );
    const DyParams_GoDataForCreate *bpParamPtr = dyParams_.get_binaryPtr<DyParams_GoDataForCreate>();
    //===
    return assemble_regularGo( goRef_, bpParamPtr->goDataPtr );
}

// 直接解析 goDataForCreate 版
// 部分 具象go-class 可以提前解开 dyParam, 改动 goData 后，再传入本函数
const GoDataForCreate *assemble_regularGo( GameObj &goRef_, const GoDataForCreate *goDataPtr_ ){

    //-- set lAltiRange ---
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr_->get_goSpeciesId() );
    goRef_.set_pos_lAltiRange( goSpecFromJsonRef.get_lAltiRange( goDataPtr_->get_goAltiRangeLabel() ) );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.goLabelId = goDataPtr_->get_goLabelId();
    goRef_.actionDirection.reset( goDataPtr_->get_direction() );
    goRef_.brokenLvl.reset( goDataPtr_->get_brokenLvl() );

    tprAssert( goDataPtr_->get_colliDataFromJsonPtr() );
    goRef_.set_colliDataFromJsonPtr( goDataPtr_->get_colliDataFromJsonPtr() );


    //===== goMeshs =====//
    for( const auto &sptrRef : goDataPtr_->get_goMeshs_autoInit() ){ // only autoInit 
        const GoDataForCreate::GoMeshBase &gmRef = *sptrRef;

        auto &goMeshRef = goRef_.goMeshSet.creat_new_goMesh( 
                                gmRef.get_goMeshName(),
                                gmRef.get_subspeciesId(),
                                gmRef.get_animActionEName(),
                                gmRef.get_renderLayerType(),
                                gmRef.get_shaderType(),  // pic shader
                                gmRef.get_dposOff(), //- pposoff
                                gmRef.get_zOff(),  //- zOff
                                gmRef.get_uWeight(),
                                gmRef.get_isVisible() //- isVisible
                                );
    } 
    //===
    return goDataPtr_;
}






