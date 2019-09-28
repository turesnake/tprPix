/*
 * =================== build_chunkData.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build chunkData
 * ----------------------------
 */
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cstring>
#include <cmath>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "jobs_all.h"
#include "Job.h"
#include "simplexNoise.h"
#include "config.h"
#include "MapTexture.h" 
#include "Quad.h"
#include "tprCast.h"
#include "random.h"

#include "esrc_chunkData.h"
#include "esrc_gameSeed.h"
#include "esrc_field.h"
#include "esrc_ecoObj.h"

#include "Job_ChunkCreate.h"

/*
#include <iostream>
#include <string>
using std::cout;
using std::endl;
*/

namespace bcd_inn {//----------- namespace: bcd_inn ----------------//

    //-- 不要随意建立 static数据，本文件的代码，会被数个 job线程 调用 --


    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    const std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION, 0 },
        IntVec2{ 0, ENTS_PER_SECTION },
        IntVec2{ ENTS_PER_SECTION, ENTS_PER_SECTION }
    };


    //===== funcs =====//
    void calc_chunkData(    IntVec2 chunkMPos_, 
                            ChunkData &chunkData_ ); 

    void colloect_nearFour_ecoObjDatas( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                        IntVec2 anyMPos_ );
    
    void assign_mapent_to_4_ecoObjs( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                        Job_MapEntInn &mapEnt_ );

}//-------------- namespace: bcd_inn end ----------------//



/* ===========================================================
 *                build_chunkData_main
 * -----------------------------------------------------------
 * 在未来，这个函数需要写进 atom 函数中。
 */
void build_chunkData_main( const Job &job_ ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    tprAssert( job_.argBinary.size() == sizeof(ArgBinary_Build_ChunkData) );
    ArgBinary_Build_ChunkData arg {};
    memcpy( (void*)&arg,
            (void*)&(job_.argBinary.at(0)),
            sizeof(ArgBinary_Build_ChunkData) );

    IntVec2 chunkMPos = chunkKey_2_mpos( arg.chunkKey );

    //------------------------------//
    //           [1]
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoObj )
    //------------------------------//
    // 已经在 主线程 chunkBuild_1_push_job() 中 提前完成
    // 反正再糟糕也不过是，1帧内创建 5 个 ecoObj 实例
    // 这个开销可以接受

    //------------------------------//
    //            [2]
    //  集中生成 周边 4chunk 的 所有 fields
    //------------------------------//
    IntVec2  tmpFieldMPos {};
    for( int h=0; h<FIELDS_PER_CHUNK*2; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK*2; w++ ){ //- each field in 2*2chunks
            tmpFieldMPos.set(   chunkMPos.x + w*ENTS_PER_FIELD,
                                chunkMPos.y + h*ENTS_PER_FIELD );
            esrc::atom_try_to_insert_and_init_the_field_ptr( tmpFieldMPos );
                    // just try, if target-field is in building in other thread
                    // do nothing and return;
                    // so. do not get_fiele_ref in job thread
        }
    } //- each field in 2*2chunks

    //--------------------------//
    //       chunkData
    //--------------------------//
    ChunkData &chunkDataRef = esrc::atom_insert_new_chunkData( arg.chunkKey );
    bcd_inn::calc_chunkData( chunkMPos, chunkDataRef );


    //--------------------------//
    //-- chunkData 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 chunk 数据准备好了
    //--------------------------//
    esrc::atom_push_back_2_chunkDataFlags( arg.chunkKey );
}


namespace bcd_inn {//----------- namespace: bcd_inn ----------------//



/* ===========================================================
 *                 calc_chunkData
 * -----------------------------------------------------------
 */
void calc_chunkData(IntVec2 chunkMPos_, ChunkData &chunkData_ ){

    //------------------------//
    //  nearFour_ecoObjDatas
    //------------------------//
    // 按照 ecoObj.occupyWeight 倒叙排列（值大的在前面）
    std::map<occupyWeight_t,EcoObj_ReadOnly> nearFour_ecoObjDatas {};
    bcd_inn::colloect_nearFour_ecoObjDatas( nearFour_ecoObjDatas, chunkMPos_ );
                // 并不精确，chunk 外延的一圈 mapents，它们的 nearFour_ecoObj 数据不是当前这个值
                // 这是一种简化办法。最终效果近似即可

    //------------------------//
    //   chunkData.mapEntInns
    //------------------------//
    IntVec2    mposOff {};
    for( int h=-1; h<=ENTS_PER_CHUNK; h++ ){
        for( int w=-1; w<=ENTS_PER_CHUNK; w++ ){ // 34*34 mapent 

            mposOff = IntVec2{ w, h };
            Job_MapEntInn &mapEntRef = chunkData_.getnc_mapEntInnRef(mposOff);
            mapEntRef.init( chunkMPos_ + mposOff );
            bcd_inn::assign_mapent_to_4_ecoObjs( nearFour_ecoObjDatas, mapEntRef );
        }
    }

    //------------------------//
    //      fieldKeys
    //------------------------//
    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys，only used inner
    fieldKeys.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK ); // reserve FIRST !!!
    IntVec2    tmpFieldMpos {};
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in 8*8
            tmpFieldMpos.set(   chunkMPos_.x + w*ENTS_PER_FIELD, 
                                chunkMPos_.y + h*ENTS_PER_FIELD );
            fieldKeys.push_back( fieldMPos_2_fieldKey(tmpFieldMpos) );
        }
    }

    //------------
    IntVec2    tmpFieldMPos {};
    IntVec2    tmpEntMPos {};
    
    MapAltitude  minFieldAlti {};  
    MapAltitude  maxFieldAlti {};

    //--- just 32 * 32 mapents in a chunk ---
    for( const auto &fieldKey : fieldKeys ){ //- each field key

        tmpFieldMPos = fieldKey_2_mpos(fieldKey);

        minFieldAlti.set(  100.0 );
        maxFieldAlti.set( -100.0 );

        for( int eh=0; eh<ENTS_PER_FIELD; eh++ ){
            for( int ew=0; ew<ENTS_PER_FIELD; ew++ ){ //- each ent in field

                tmpEntMPos = tmpFieldMPos + IntVec2{ ew, eh };
                mposOff = tmpEntMPos - chunkMPos_;

                Job_MapEntInn &mapEntRef = chunkData_.getnc_mapEntInnRef(mposOff);
                //----- field min/max alti -----
                if( mapEntRef.alti < minFieldAlti ){
                    minFieldAlti = mapEntRef.alti;
                }
                if( mapEntRef.alti > maxFieldAlti ){
                    maxFieldAlti = mapEntRef.alti;
                }

                //--- skip unborder ent ---//
                if( chunkData_.is_borderMapEnt(mposOff) ){
                    mapEntRef.isBorder = true;
                }else{
                    mapEntRef.isBorder = false;
                }

                //--- insert entInnPtr ---//
                chunkData_.insert_a_entInnPtr_2_field( fieldKey, IntVec2{ew, eh}, &mapEntRef );

            }
        }//- each ent in field end -- 
        //----- field data -----//
        chunkData_.set_field_min_max_altis( fieldKey, minFieldAlti, maxFieldAlti );// only once
        chunkData_.apply_field_job_groundGoEnts( fieldKey );

    }//-- each field key end --
}


// 不需要每次都调用，只需要在 sectionMPos 发生改变时调用 --
void colloect_nearFour_ecoObjDatas( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                    IntVec2 anyMPos_ ){
    IntVec2 sectionMPos = anyMPos_2_sectionMPos( anyMPos_ );
    sectionKey_t tmpKey {};
    container_.clear();
    for( const auto &whOff : bcd_inn::quadSectionKeyOffs ){
        tmpKey = sectionMPos_2_sectionKey( sectionMPos + whOff );
        container_.insert( esrc::atom_get_ecoObj_readOnly( tmpKey ) );
    }
}


void assign_mapent_to_4_ecoObjs( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                Job_MapEntInn &mapEnt_ ){

    sectionKey_t sectionKey = anyMPos_2_sectionKey( mapEnt_.mpos );

    double         vx        {};
    double         vy        {};
    IntVec2        mposOff   {};
    double         freqBig   { 0.9 }; //- 值越小，ecoObj 边界越平滑
    float          freqSml   { 2.3f };

    double         pnVal     {}; //- 围绕 0 波动的 随机值
    double         off       {};
    size_t        count     {};

    double targetDistance = 1.4 * (0.5 * ENTS_PER_SECTION) * 1.04; //- 每个field 最终的 距离比较值。

    vx = static_cast<double>(mapEnt_.mpos.x) / static_cast<double>(ENTS_PER_CHUNK);
    vy = static_cast<double>(mapEnt_.mpos.y) / static_cast<double>(ENTS_PER_CHUNK);

    const glm::dvec2 &field_pposOff = esrc::get_gameSeed().get_field_dposOff();
    vx += field_pposOff.x;
    vy += field_pposOff.y;
                    //-- 一个简单的偏移值，也许未来可以为其 改名字 ...
                    //   毕竟，现在和 mapent 关联了 ...

    double pnValBig = simplex_noise2(    (vx + 51.15) * freqBig,
                                        (vy + 151.15) * freqBig ) * 17; // [-x.0, x.0]
    float pnValSml = simplex_noise2(    (vx + 244.41f) * freqSml,
                                        (vy + 144.41f) * freqSml ) * 5; // [-x.0, x.0]

    pnVal = pnValBig + pnValSml;
    if( pnVal > 20.0 ){
        pnVal = 20.0;
    }
    if( pnVal < -20.0 ){
        pnVal = -20.0;
    }
    // now, pnVal: [-20.0, 20.0]

    count = 0;
    for( auto &ecoDataPair : container_ ){
        count++;
        const auto &ecoReadOnly = ecoDataPair.second;

        if( count != container_.size()){ //- 前3个 eco

            mposOff = mapEnt_.mpos - sectionKey_2_mpos( ecoReadOnly.sectionKey );
            off = static_cast<double>(sqrt( mposOff.x*mposOff.x + mposOff.y*mposOff.y )); // [ ~ 90.0 ~ ]
            off += pnVal * 0.7; // [-x.0, x.0] + 90.0

            if( off < targetDistance ){ //- tmp

                mapEnt_.ecoObjKey = ecoReadOnly.sectionKey;
                mapEnt_.colorRableId = ecoReadOnly.colorTableId;
                mapEnt_.density.set( mapEnt_.mpos, 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
                break;
            }
        }else{ //- 第四个 eco
            mapEnt_.ecoObjKey = ecoReadOnly.sectionKey;
            mapEnt_.colorRableId = ecoReadOnly.colorTableId;
            mapEnt_.density.set( mapEnt_.mpos, 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
        }
    }
}









}//-------------- namespace: bcd_inn end ----------------//

