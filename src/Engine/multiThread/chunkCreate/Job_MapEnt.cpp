/*
 * ========================== Job_MapEnt.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Job_MapEnt.h"

//-------------------- Engine --------------------//
#include "simplexNoise.h"
#include "MapEnt.h"
#include "random.h"

#include "esrc_gameSeed.h"


namespace jobMP_inn {//----------- namespace: jobMP_inn ----------------//
    //- 在未来，freq 这组值也会收到 ecosys 影响 --
    const double freqSeaLvl { 0.05 };
    const double freqBig    { 0.4 };
    const double freqMid    { 1.6 };
    const double freqSml    { 4.0 };

    const glm::dvec2  worldCenter { 0.0, 0.0 };

}//-------------- namespace: jobMP_inn end ----------------//


//===== static =====//
const IntVec2 Job_MapEnt::pixesPerHalfMapent {HALF_PIXES_PER_MAPENT, HALF_PIXES_PER_MAPENT};


void Job_MapEnt::init( IntVec2 mpos_, chunkKey_t chunkKey_ )noexcept{
    this->mpos = mpos_;
    this->midPPos = mpos_2_ppos(mpos_) + Job_MapEnt::pixesPerHalfMapent;
    this->alti.set( calc_pixAlti( this->midPPos ) );
    this->chunkKey = chunkKey_;

    //--- uWeight ---//
    //glm::dvec2 fDPos = mpos_2_dpos(this->mpos) * 0.177 + esrc::get_gameSeed().get_field_dposOff();//- 未来更换
    //const double freq = 1.0 / 13.0; //- tmp
    //this->originPerlin = simplex_noise2( fDPos * freq );//- [-1.0, 1.0]
    //this->uWeight = blender_the_perlinNoise( this->originPerlin, 130617.7, 10000 ); //[0, 9999]

    this->uWeight = calc_simple_uWeight( this->mpos );

}



void Job_MapEnt::write_2_mapEnt( MemMapEnt &entRef_ )const noexcept{
    entRef_.set_chunkKey( this->chunkKey );
    entRef_.set_ecoObjKey( this->ecoObjKey );
    entRef_.set_colorTableId( this->colorTableId );
    entRef_.set_density( this->density );
    entRef_.set_mapAlti( this->alti );
    //entRef_.set_perlin( this->originPerlin, this->uWeight );
    entRef_.set_uWeight( this->uWeight );
    entRef_.set_isBorder( this->isBorder );
}


/* ===========================================================
 *                 calc_pixAlti
 * -----------------------------------------------------------
 * 计算单个pix 的 alti
 * ---
 * 这部分算法，应当和 waterAnimCanvas 中的完全一致!!!
 */
double Job_MapEnt::calc_pixAlti( IntVec2 pixPPos_ )noexcept{

    GameSeed &gameSeedRef = esrc::get_gameSeed();
    glm::dvec2  altiSeed_pposOffBig = gameSeedRef.get_altiSeed_pposOffBig();
    glm::dvec2  altiSeed_pposOffMid = gameSeedRef.get_altiSeed_pposOffMid();
    glm::dvec2  altiSeed_pposOffSml = gameSeedRef.get_altiSeed_pposOffSml();
                            //-- 此处有问题，从 job线程 访问 gameSeed，不够安全...

    double    pixDistance {}; //- pix 距离 世界中心的距离。 暂时假设，(0,0) 为世界中心
    double    seaLvl {};

    double    pnValBig {};
    double    pnValMid {};
    double    pnValSml {};
    double    altiVal  {};  //- target val

    glm::dvec2 pixCFPos {};

            pixCFPos.x = static_cast<double>(pixPPos_.x) / static_cast<double>(PIXES_PER_CHUNK);
            pixCFPos.y = static_cast<double>(pixPPos_.y) / static_cast<double>(PIXES_PER_CHUNK);
            //------------------//
            //     seaLvl
            //------------------//
            seaLvl = simplex_noise2( pixCFPos * jobMP_inn::freqSeaLvl ) * 50.0; // [-50.0, 50.0]

            /*-------
            pixDistance = glm::distance( pixCFPos, jobMP_inn::worldCenter );
            pixDistance /= 10.0;
            seaLvl += pixDistance;
            */
                // 暂时不改写 seaLvl 值，这样，无论走多远，世界的 水陆分布还是原来的比例
                // 如果开启这个设置，玩家向四周探索时，世界中的水域会变多，直到没有陆地

            //-------------
            // 强制性的，人为缩小水域面积。
            seaLvl += -40.0; // [0.0, 100.0]

                
            if( seaLvl < 0.0 ){ //- land
                seaLvl *= 0.3;  // [-15.0, 50.0]
            }
            //------------------//
            //    alti.val
            //------------------//
            //--- 使用速度最快的 2D-simplex-noise ---
            pnValBig = simplex_noise2( (pixCFPos + altiSeed_pposOffBig) * jobMP_inn::freqBig ) * 100.0 - seaLvl; // [-100.0, 100.0]
            pnValMid = simplex_noise2( (pixCFPos + altiSeed_pposOffMid) * jobMP_inn::freqMid ) * 50.0  - seaLvl; // [-50.0, 50.0]
            pnValSml = simplex_noise2( (pixCFPos + altiSeed_pposOffSml) * jobMP_inn::freqSml ) * 20.0  - seaLvl; // [-20.0, 20.0]
            
            //---------
            altiVal = floor(pnValBig + pnValMid + pnValSml);

            //------- 抹平头尾 -------//
            if( altiVal > 100.0 ){
                altiVal = 100.0;
            }else if( altiVal < -100.0 ){
                altiVal = -100.0;
            }
            // now, altiVal: [-100.0, 100.0]=

    return altiVal;
}

