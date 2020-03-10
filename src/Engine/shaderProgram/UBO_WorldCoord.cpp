/*
 * ==================== UBO_WorldCoord.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ubo_all.h"

//-------------------- Engine --------------------//
#include "esrc_uniformBlockObj.h"
#include "esrc_state.h"
#include "esrc_coordinate.h"


namespace ubo{//------------- namespace ubo ----------------



// only called in init
void write_ubo_WorldCoord(){

    tprAssert( esrc::is_setState("coordinate") );

    const auto &worldCoordRef = esrc::get_worldCoordRef();

    UBO_WorldCoord data {};
    data.xVec = glmDvec2_2_floatVec2( worldCoordRef.get_xVec() );
    data.yVec = glmDvec2_2_floatVec2( worldCoordRef.get_yVec() );
    data.denominator = static_cast<float>( worldCoordRef.get_denominator() );

    auto &uboRef = esrc::get_uniformBlockObjRef( UBOType::WorldCoord );
    uboRef.write(  0, 
                sizeof(UBO_WorldCoord),
                static_cast<const GLvoid*>( &data ) );

}

}//------------- namespace ubo: end ----------------
