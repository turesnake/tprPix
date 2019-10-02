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
    this->init_nodeMPos_and_nodeDPosOff();

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
 *                init_nodeMPos_and_nodeDPosOff
 * -----------------------------------------------------------
 * 
 *         nodeDPosOff 存在问题 ..... 
 *                     调试中 .....
 * 
 */
void MapField::init_nodeMPos_and_nodeDPosOff(){

    double    freq  { 13.0 };
    double    pnX   {};
    double    pnY   {};

    const glm::dvec2 yModifier { 70.7,70.7 };

    pnX = simplex_noise2( this->FDPos * freq ); //- [-1.0, 1.0]
    pnY = simplex_noise2( (this->FDPos + yModifier) * freq ); //- [-1.0, 1.0]

    pnX = pnX * 71.0 + 100.0; //- [29.0, 171.0]
    pnY = pnY * 71.0 + 100.0; //- [29.0, 171.0]
        tprAssert( (pnX>0) && (pnY>0) );

    //-- nodeMPos --//
    //-- [0,3] 
    //size_t idxX = cast_2_size_t(floor(pnX)) % (ENTS_PER_FIELD-1);
    //size_t idxY = cast_2_size_t(floor(pnY)) % (ENTS_PER_FIELD-1);

    size_t idxX = cast_2_size_t(floor(pnX)) % (ENTS_PER_FIELD);
    size_t idxY = cast_2_size_t(floor(pnY)) % (ENTS_PER_FIELD);

    //this->nodeMPos = this->get_mpos() + IntVec2{static_cast<int>(idxX), 
    //                                            static_cast<int>(idxY) };

    //-- nodeDPosOff --//
    //size_t pposOffRange { PIXES_PER_MAPENT - 10 }; //- 54
    size_t pposOffRange { 40 };

    //size_t pX = cast_2_size_t(floor(pnX + 971.0)) % pposOffRange;
    //size_t pY = cast_2_size_t(floor(pnY + 137.0)) % pposOffRange;
    size_t pX  = 0;
    size_t pY  = 1;

    //this->nodeDPosOff = {   static_cast<double>( pX ),
    //                        static_cast<double>( pY ) }; // [ 0.0, 54.0 ] 
    //this->nodeDPosOff = glm::dvec2{0.0, 0.0};    

                //- 尝试允许 node 在整个 field 内分布，会获得较好的 视觉效果
                //  临时版，有待修整 .....
                //  目前 nodeDPosOff 被设为0了


    glm::dvec2 v = glm::dvec2{  static_cast<double>(idxX * PIXES_PER_MAPENT + pX  ),  
                                static_cast<double>(idxY * PIXES_PER_MAPENT + pY  ) };

            double limit = static_cast<double>( PIXES_PER_FIELD );

            tprAssert( (v.x>=0.0) && (v.x<limit) &&
                        (v.y>=0.0) && (v.y<limit) );


    this->nodeDPosOff = v;



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


