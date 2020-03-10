/*
 * ================== BioSoupColorTable.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/bioSoup/BioSoupColorTable.h"

//-------------------- Engine --------------------//
#include "uniformBlockObjs.h"

#include "esrc_uniformBlockObj.h"



namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------

namespace bsct_inn {//------------- namespace bsct_inn ----------------


    BioSoupColorTable  bioSoupColorTable {};


}//------------- namespace bsct_inn: end ----------------


void write_ubo_BioSoupColorTable(){

    // update datas

    bsct_inn::bioSoupColorTable.base_light = FloatVec4{ 215.0f/255.0f, 191.0f/255.0f, 135.0f/255.0f, 1.0f };
    bsct_inn::bioSoupColorTable.base_mid   = FloatVec4{ 193.0f/255.0f, 165.0f/255.0f, 120.0f/255.0f, 1.0f };
    bsct_inn::bioSoupColorTable.base_dark  = FloatVec4{ 173.0f/255.0f, 141.0f/255.0f, 105.0f/255.0f, 1.0f };

    bsct_inn::bioSoupColorTable.particle_light = FloatVec4{ 215.0f/255.0f+0.2f, 191.0f/255.0f+0.2f, 135.0f/255.0f+0.2f, 1.0f };
    bsct_inn::bioSoupColorTable.particle_mid   = FloatVec4{ 193.0f/255.0f+0.2f, 165.0f/255.0f+0.2f, 120.0f/255.0f+0.2f, 1.0f };
    bsct_inn::bioSoupColorTable.particle_dark  = FloatVec4{ 173.0f/255.0f+0.2f, 141.0f/255.0f+0.2f, 105.0f/255.0f+0.2f, 1.0f };


    // do write
    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::BioSoupColorTable );

    ubo.write(  0, 
                sizeof(BioSoupColorTable),
                static_cast<const GLvoid*>( &bsct_inn::bioSoupColorTable ) );

}


}//------------- namespace gameObjs::bioSoup: end ----------------

