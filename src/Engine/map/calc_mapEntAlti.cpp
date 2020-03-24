/*
 * ===================== calc_mapEntAlti.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "MapCoord.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"


namespace cmea_inn {//----------- namespace: cmea_inn ----------------//

    // 在未来，这组值也会实现为 动态值 
    constexpr double freqSeaLvl { 0.05 };
    constexpr double freqBig    { 0.4 };
    constexpr double freqMid    { 1.6 };
    constexpr double freqSml    { 4.0 };
    constexpr glm::dvec2  worldCenter { 0.0, 0.0 };

}//-------------- namespace: cmea_inn end ----------------//


// 计算单个pix 的 alti
double calc_mapEntAlti( IntVec2 mpos_ )noexcept{

    // 默认计算 mapent 中点 pix 的 mapAlti 值
    IntVec2 midPPos = mpos_2_ppos(mpos_) + IntVec2{ HALF_PIXES_PER_MAPENT<>, HALF_PIXES_PER_MAPENT<> };

    GameSeed &gameSeedRef = esrc::get_gameSeed();
    glm::dvec2  altiSeed_pposOffBig = gameSeedRef.get_altiSeed_pposOffBig();
    glm::dvec2  altiSeed_pposOffMid = gameSeedRef.get_altiSeed_pposOffMid();
    glm::dvec2  altiSeed_pposOffSml = gameSeedRef.get_altiSeed_pposOffSml();
                            //-- 此处有问题，从 job线程 访问 gameSeed，不够安全...

    double    pixDistance {}; //- pix 距离 世界中心的距离。 暂时假设，(0,0) 为世界中心
    double    seaLvl {};

    double    pnValBig {};
    double    pnValMid {};
    //double    pnValSml {};
    double    altiVal  {};  //- target val

    glm::dvec2 pixCFPos {};

            pixCFPos.x = static_cast<double>(midPPos.x) / PIXES_PER_CHUNK_D;
            pixCFPos.y = static_cast<double>(midPPos.y) / PIXES_PER_CHUNK_D;
            //------------------//
            //     seaLvl
            //------------------//
            seaLvl = simplex_noise2( pixCFPos * cmea_inn::freqSeaLvl ) * 50.0; // [-50.0, 50.0]

            /*-------
            pixDistance = glm::distance( pixCFPos, cmea_inn::worldCenter );
            pixDistance /= 10.0;
            seaLvl += pixDistance;
            */
                // 暂时不改写 seaLvl 值，这样，无论走多远，世界的 水陆分布还是原来的比例
                // 如果开启这个设置，玩家向四周探索时，世界中的水域会变多，直到没有陆地

            //-------------
            // 强制性的，人为缩小水域面积。
            //seaLvl += -40.0; // [0.0, 100.0]
            seaLvl += -50.0; // [0.0, 100.0]


                
            if( seaLvl < 0.0 ){ //- land
                seaLvl *= 0.3;  // [-15.0, 50.0]
            }
            //------------------//
            //    alti.val
            //------------------//
            //--- 使用速度最快的 2D-simplex-noise ---
            pnValBig = simplex_noise2( (pixCFPos + altiSeed_pposOffBig) * cmea_inn::freqBig ) * 100.0 - seaLvl; // [-100.0, 100.0]
            pnValMid = simplex_noise2( (pixCFPos + altiSeed_pposOffMid) * cmea_inn::freqMid ) * 40.0  - seaLvl; // [-50.0, 50.0]
            //pnValSml = simplex_noise2( (pixCFPos + altiSeed_pposOffSml) * cmea_inn::freqSml ) * 10.0  - seaLvl; // [-20.0, 20.0]
                                // 不再累加 Sml，biosoup 边界更加柔和平整
                                // 也更利于 被合并为 2m2 / 4m4 size, 减少 go数量
            
            //---------
            //altiVal = floor(pnValBig + pnValMid + pnValSml);
            altiVal = floor(pnValBig + pnValMid );

            //------- 抹平头尾 -------//
            if( altiVal > 100.0 ){
                altiVal = 100.0;
            }else if( altiVal < -100.0 ){
                altiVal = -100.0;
            }
            // now, altiVal: [-100.0, 100.0]=

    return altiVal;
}














