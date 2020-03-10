/*
 * ======================= UBO_Window.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ubo_all.h"

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "esrc_uniformBlockObj.h"

namespace ubo{//------------- namespace ubo ----------------

// only called in:
// -1- app.init
// -2- windowSZ changed (never yet...)
void write_ubo_Window(){

    UBO_Window data {};
    data.gameSZ = FloatVec2 {   static_cast<float>(ViewingBox::gameSZ.x),
                                static_cast<float>(ViewingBox::gameSZ.y) };

    auto &uboRef = esrc::get_uniformBlockObjRef( UBOType::Window );
    uboRef.write(  0, 
                sizeof(UBO_Window),
                static_cast<const GLvoid*>( &data ) );
}

}//------------- namespace ubo: end ----------------
