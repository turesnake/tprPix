/*
 * ===================== MapField.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   4*4mapent 构成一个 field   [第二版]
 * ----------------------------
 */
#include "MapField.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "random.h"
#include "IntVec.h"
#include "EcoObj.h"
#include "simplexNoise.h"
#include "MapEnt.h"

#include "esrc_gameSeed.h"
#include "esrc_ecoObj.h"
#include "tprCast.h"

#include "tprDebug.h"


//namespace field_inn {//----------- namespace: mapField_inn ---------------//
//}//-------------- namespace: mapField_inn end ---------------//

//===== static =====//
const glm::dvec2 MapField::halfDPosOff {
            static_cast<double>( PIXES_PER_FIELD ) * 0.5,
            static_cast<double>( PIXES_PER_FIELD ) * 0.5 };

/* ===========================================================
 *                    init
 * -----------------------------------------------------------
 *  仅 初始化 一阶数据
 * param: _mpos      -- 此 field 中的任意 mapent.mpos 
 * param: _chunkMPos -- 此 field 所属的 chunk mpos
 */
void MapField::init( IntVec2 anyMPos_ ){

    this->mcpos.set_by_mpos( anyMPos_2_fieldMPos(anyMPos_) );
    this->fieldKey = fieldMPos_2_fieldKey( this->mcpos.get_mpos() );

    //--- fieldFPos ----
    this->FDPos = this->mcpos.get_dpos() * 0.27 + esrc::get_gameSeed().get_field_dposOff();

    //--- field.nodeMPos ---
    this->init_nodeDPos();

    //--- originPerlin ---
    // 3*3 个 field 组成一个 pn晶格
    const double freq = 1.0 / 3.0; //- tmp
    this->originPerlin = simplex_noise2( this->FDPos * freq );//- [-1.0, 1.0]

    //--- weight ---
    this->weight  = this->originPerlin * 100.0;        //- [-100.0, 100.0]
    this->uWeight = blender_the_perlinNoise( this->originPerlin, 699.7, 97 ); //[0.0, 97.0]

    //--- occupyWeight ---
    this->init_occupyWeight();
}




/* ===========================================================
 *                init_nodeDPos
 * -----------------------------------------------------------
 */
void MapField::init_nodeDPos(){

    double    freq  { 7.0 };
    double    pnX   {};
    double    pnY   {};

    const glm::dvec2 yModifier { 370.1, 1971.7 };

    pnX = simplex_noise2( this->FDPos * freq ); //- [-1.0, 1.0]
    pnY = simplex_noise2( (this->FDPos+yModifier) * freq ); //- [-1.0, 1.0]
    //- perlin is too close to center, do some balance
    pnX *= 1.2;
    pnY *= 1.2;
    if(         pnX >=  1.0 ){ pnX =  0.99; }
    else if(    pnX <= -1.0 ){ pnX = -0.99; }
    if(         pnY >=  1.0 ){ pnY =  0.99; }
    else if(    pnY <= -1.0 ){ pnY = -0.99; }

    double scaleX = MapField::halfDPosOff.x - 16.0; // not too close between two field-gos
    double scaleY = MapField::halfDPosOff.y - 4.0; 

    pnX = floor(pnX*scaleX); //- align to pix
    pnY = floor(pnY*scaleY); //- align to pix

    this->nodeDPos = this->get_dpos() + MapField::halfDPosOff + glm::dvec2{pnX, pnY};
}


/* ===========================================================
 *                init_occupyWeight
 * -----------------------------------------------------------
 */
void MapField::init_occupyWeight(){

    //-- 本 field 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2 v = floorDiv( this->get_mpos(), static_cast<double>(ENTS_PER_FIELD) );
    IntVec2 oddEven = floorMod( v, 2.0 );

    //-- 相邻 field 间的 occupyWeight 没有关联性，就是 白噪音 --
    double Fidx = simplex_noise2(this->FDPos + glm::dvec2{17.1, 17.1}) * 30.0 + 60.0; //- [30.0, 90.0]

    tprAssert( Fidx > 0 );
    size_t randIdx = (size_t)floor(Fidx); //- [30, 90]

    this->occupyWeight = calc_occupyWeight( oddEven, randIdx );
}


