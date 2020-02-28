/*
 * ====================== assemble_go.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/assemble_go.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "DyParam.h"
#include "GoSpecFromJson.h"
#include "dyParams.h"

#include "esrc_shader.h" 


// 被 具象go类 调用，将 GoDataForCreate 中的数据，配置进 regularGo 中
// 在未来，参数组 可能会扩展
void assemble_regularGo( GameObj &goRef_,const DyParam &dyParams_ ){

    //================ dyParams =================//
    // 假定都使用 DyParams_GoDataForCreate
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_GoDataForCreate).hash_code() );
    const DyParams_GoDataForCreate *bpParamPtr = dyParams_.get_binaryPtr<DyParams_GoDataForCreate>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;

    //-- set lAltiRange ---
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    goRef_.set_pos_lAltiRange( goSpecFromJsonRef.get_lAltiRange( goDataPtr->goAltiRangeLabel ) );


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.goLabelId = goDataPtr->goLabelId;
    goRef_.actionDirection.reset( goDataPtr->direction );
    goRef_.brokenLvl.reset( goDataPtr->brokenLvl );

    tprAssert( goDataPtr->colliDataFromJsonPtr );
    goRef_.set_colliDataFromJsonPtr( goDataPtr->colliDataFromJsonPtr );


    //===== goMeshs =====//
    for( const auto &uptrRef : goDataPtr->goMeshEntUPtrs ){
        const GoDataForCreate::GoMeshBase &gmRef = *uptrRef;

        auto &goMeshRef = goRef_.creat_new_goMesh( 
                                gmRef.get_goMeshName(),
                                gmRef.get_subspeciesId(),
                                gmRef.get_animActionEName(),
                                gmRef.get_renderLayerType(),
                                gmRef.get_shaderType(),  // pic shader
                                gmRef.get_dposOff(), //- pposoff
                                gmRef.get_zOff(),  //- zOff
                                gmRef.get_isVisible() //- isVisible
                                );
    } 
}






