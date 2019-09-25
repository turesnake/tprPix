/*
 * ================== UBO_UnifiedColorTable.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "UBO_UnifiedColorTable.h"


//-------------------- Engine --------------------//

#include "esrc_colorTableSet.h"
#include "esrc_uniformBlockObj.h"
#include "esrc_player.h"
#include "esrc_field.h"
#include "esrc_ecoObj.h"

#include "esrc_time.h"

#include "tprDebug.h"

namespace ubo{//------------- namespace ubo ----------------

namespace ubo_unifiedColorTable_inn{//------------- namespace ubo ----------------
    colorTableId_t currentColorTableId {99999};
}//------------- namespace ubo: end ----------------



// called only once, at app.init 
void write_ubo_OriginColorTable(){

    auto &colorTableSetRef = esrc::get_colorTabelSet();
    auto id = colorTableSetRef.get_colorTableId("origin");
    auto &originColorTableRef = colorTableSetRef.get_colorTableRef(id);
    //---
    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::OriginColorTable );
    ubo.write(0, 
            originColorTableRef.get_dataSize(),
            originColorTableRef.get_dataPtr<const GLvoid*>() );
}


//-- called each render frame --
void update_and_write_ubo_UnifiedColorTable(){

    //--------------------------//
    //  check if rebind targetColorTable
    //--------------------------//
    auto &goRef = esrc::get_player().get_goRef();
    const auto &fieldRef = esrc::atom_get_field( anyDPos_2_fieldKey(goRef.get_dpos()) );
    colorTableId_t id = esrc::atom_ecoObj_get_colorTableId( fieldRef.get_ecoObjKey() );


    // 最简方案，仅在 playergo 跨越 ecoObj 时，才绑定新 colorTable --
    if( id != ubo_unifiedColorTable_inn::currentColorTableId ){
        ubo_unifiedColorTable_inn::currentColorTableId = id;
        esrc::rebind_currentColorTabel_target( id );
    }

    //--------------------------//
    //         update
    //--------------------------//
    auto &currentColorTableRef = esrc::get_currentColorTabel();
    currentColorTableRef.update();

    if( !currentColorTableRef.get_isWorking() ){
        return;
    }

        cout << "  -- [] -- "
            << id << "; "
            << esrc::get_timer().get_currentTime() 
            << endl;

    //--------------------------//
    //        write
    //--------------------------//
    auto &colorTableRef = currentColorTableRef.get_colorTableRef();

    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::UnifiedColorTable );
    ubo.write(  0, 
                colorTableRef.get_dataSize(),
                colorTableRef.get_dataPtr<const GLvoid*>() );
                //-- 很多时候，也不需要每帧都 彻底重写数据 --
                //   而是可以只改写 某一段

}


}//------------- namespace ubo: end ----------------

