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

//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupColor.h"



namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------

namespace bsct_inn {//------------- namespace bsct_inn ----------------


    BioSoupColorTable  bioSoupColorTable {};


}//------------- namespace bsct_inn: end ----------------


void write_ubo_BioSoupColorTable(){



    const Color &baseColor = ColorNodeChain::calc_next_baseColor();


    Color particle {
        baseColor.light + 0.2f,
        baseColor.mid   + 0.2f,
        baseColor.dark  + 0.2f
    };

    bsct_inn::bioSoupColorTable.base_light = floatVec3_2_floatVec4( baseColor.light, 1.0f );
    bsct_inn::bioSoupColorTable.base_mid   = floatVec3_2_floatVec4( baseColor.mid,   1.0f );
    bsct_inn::bioSoupColorTable.base_dark  = floatVec3_2_floatVec4( baseColor.dark,  1.0f );

    bsct_inn::bioSoupColorTable.particle_light = floatVec3_2_floatVec4( particle.light, 1.0f );
    bsct_inn::bioSoupColorTable.particle_mid   = floatVec3_2_floatVec4( particle.mid,   1.0f );
    bsct_inn::bioSoupColorTable.particle_dark  = floatVec3_2_floatVec4( particle.dark,  1.0f );


    // do write
    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::BioSoupColorTable );

    ubo.write(  0, 
                sizeof(BioSoupColorTable),
                static_cast<const GLvoid*>( &bsct_inn::bioSoupColorTable ) );

}


}//------------- namespace gameObjs::bioSoup: end ----------------

