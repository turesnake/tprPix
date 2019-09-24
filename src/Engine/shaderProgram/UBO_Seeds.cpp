/*
 * ======================= UBO_Seeds.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "UBO_Seeds.h"


//-------------------- Engine --------------------//
#include "esrc_gameSeed.h"
#include "esrc_uniformBlockObj.h"


void write_ubo_Seeds(){

    UBO_Seeds data {};

    //-- 每个游戏存档的这组值 其实是固定的，游戏运行期间，只需传输一次 --
    GameSeed &gameSeedRef = esrc::get_gameSeed();

    data.altiSeed_pposOffSeaLvl = glmDvec2_2_floatVec2( gameSeedRef.get_altiSeed_pposOffSeaLvl() );
    data.altiSeed_pposOffBig = glmDvec2_2_floatVec2( gameSeedRef.get_altiSeed_pposOffBig() );
    data.altiSeed_pposOffMid = glmDvec2_2_floatVec2( gameSeedRef.get_altiSeed_pposOffMid() );
    data.altiSeed_pposOffSml = glmDvec2_2_floatVec2( gameSeedRef.get_altiSeed_pposOffSml() );

    auto &ubo_seeds = esrc::get_uniformBlockObjRef( UBOType::Seeds );

    ubo_seeds.write(0, 
                    sizeof(UBO_Seeds),
                    static_cast<const GLvoid*>( &data ) );
}

