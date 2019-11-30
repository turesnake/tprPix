/*
 * =================== create_job_chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build job_chunk
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

#include "esrc_job_chunk.h"
#include "esrc_gameSeed.h"
#include "esrc_field.h"
#include "esrc_ecoObj.h"

#include "Job_MapEnt.h"
#include "Job_Field.h"


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
    void calc_job_chunk( Job_Chunk &job_chunk_ ); 
    void colloect_nearFour_ecoObjDatas( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                        IntVec2 anyMPos_ );
    void assign_mapent_to_nearFour_ecoObjs( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                        Job_MapEnt &mapEnt_ );
    
}//-------------- namespace: bcd_inn end ----------------//



/* ===========================================================
 *                create_job_chunk_main
 * -----------------------------------------------------------
 * 在未来，这个函数需要写进 atom 函数中 ...
 */
void create_job_chunk_main( const Job &job_ ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    const auto *jobParamPtr = job_.get_param<ArgBinary_Create_Job_Chunk>();

    IntVec2 chunkMPos = chunkKey_2_mpos( jobParamPtr->chunkKey );

    //--------------------------//
    //      create job_chunk
    //--------------------------//
    Job_Chunk &job_chunkRef = esrc::atom_insert_new_job_chunk( jobParamPtr->chunkKey, chunkMPos );

    //------------------------------//
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoObj )
    //------------------------------//
    // 已经在 主线程 chunkCreate_1_push_job() 中 提前完成


    //--------------------------//
    //      calc job_chunk
    //--------------------------//
    bcd_inn::calc_job_chunk( job_chunkRef );


    

    //--------------------------//
    //-- job_chunk 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 chunk 数据准备好了
    //--------------------------//
    esrc::atom_push_back_2_job_chunkFlags( jobParamPtr->chunkKey );
}


namespace bcd_inn {//----------- namespace: bcd_inn ----------------//


/* ===========================================================
 *                 calc_job_chunk
 * -----------------------------------------------------------
 */
void calc_job_chunk( Job_Chunk &job_chunkRef_ ){

    IntVec2 chunkMPos = job_chunkRef_.get_chunkMPos();
    //------------------------//
    //  nearFour_ecoObjDatas
    //------------------------//
    // 按照 ecoObj.occupyWeight 倒叙排列（值大的在前面）
    std::map<occupyWeight_t,EcoObj_ReadOnly> nearFour_ecoObjDatas {};
    bcd_inn::colloect_nearFour_ecoObjDatas( nearFour_ecoObjDatas, chunkMPos );
                // 并不精确，chunk 外延的一圈 mapents，它们的 nearFour_ecoObj 数据不是当前这个值
                // 这是一种简化办法。最终效果近似即可

    //------------------------//
    //   job_chunk.mapEntInns
    //------------------------//
    IntVec2    mposOff {};
    for( int h=-1; h<=ENTS_PER_CHUNK; h++ ){
        for( int w=-1; w<=ENTS_PER_CHUNK; w++ ){ // 34*34 mapent 

            mposOff = IntVec2{ w, h };
            Job_MapEnt &mapEntRef = job_chunkRef_.getnc_mapEntInnRef(mposOff);
            mapEntRef.init( chunkMPos + mposOff );
            assign_mapent_to_nearFour_ecoObjs( nearFour_ecoObjDatas, mapEntRef );
        }
    }

    //------------------------//
    //      fieldKeys
    //------------------------//
    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys，only used inner
    fieldKeys.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK ); // reserve FIRST !!!

    IntVec2    tmpFieldMPos {};
    fieldKey_t tmpFieldKey {};
    IntVec2    fieldNodeOff {};

    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk (8*8)
            tmpFieldMPos.set(   chunkMPos.x + w*ENTS_PER_FIELD, 
                                chunkMPos.y + h*ENTS_PER_FIELD );
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            fieldKeys.push_back(tmpFieldKey);

            //--- copy nodeMapEnt datas to field --
            auto &field = job_chunkRef_.getnc_fieldRef(tmpFieldKey);
            fieldNodeOff = dpos_2_mpos( field.get_nodeDPos() ) - anyMPos_2_chunkMPos(field.get_mpos());
            const auto &mapEntInnRef = job_chunkRef_.getnc_mapEntInnRef( fieldNodeOff );
            field.set_ecoObjKey( mapEntInnRef.ecoObjKey );
            field.set_colorTableId( mapEntInnRef.colorTableId );
            field.set_density( mapEntInnRef.density );
            field.set_nodeMapAlti( mapEntInnRef.alti );
            field.set_perlin( mapEntInnRef.originPerlin, mapEntInnRef.uWeight );
        }
    }

    //------------
    IntVec2         tmpEntMPos {};
    MapAltitude     minFieldAlti {};  
    MapAltitude     maxFieldAlti {};

    //--- just 32 * 32 mapents in a chunk ---
    for( const auto &fieldKey : fieldKeys ){ //- each field key

        tmpFieldMPos = fieldKey_2_mpos(fieldKey);

        minFieldAlti.set(  100.0 );
        maxFieldAlti.set( -100.0 );

        for( int eh=0; eh<ENTS_PER_FIELD; eh++ ){
            for( int ew=0; ew<ENTS_PER_FIELD; ew++ ){ //- each ent in field

                tmpEntMPos = tmpFieldMPos + IntVec2{ ew, eh };
                mposOff = tmpEntMPos - chunkMPos;

                Job_MapEnt &mapEntRef = job_chunkRef_.getnc_mapEntInnRef(mposOff);
                //----- field min/max alti -----
                if( mapEntRef.alti < minFieldAlti ){ minFieldAlti = mapEntRef.alti; }
                if( mapEntRef.alti > maxFieldAlti ){ maxFieldAlti = mapEntRef.alti; }
                //--- skip unborder ent ---//
                mapEntRef.isBorder = job_chunkRef_.is_borderMapEnt(mposOff);
                //--- insert entInnPtr ---//
                job_chunkRef_.insert_a_entInnPtr_2_field( fieldKey, IntVec2{ew, eh}, &mapEntRef );
            }
        }//- each ent in field end -- 
        //----- field data -----//
        job_chunkRef_.set_field_min_max_altis( fieldKey, minFieldAlti, maxFieldAlti );// only once
        job_chunkRef_.apply_field_job_groundGoEnts( fieldKey );

    }//-- each field key end --

    job_chunkRef_.write_2_field_from_jobData();
    // Now，the Fields all inited !!!

    //------------------------//
    //  job_field.goSpecDatas
    //------------------------//
    job_chunkRef_.create_field_goSpecDatas();


}




// 不需要每次都调用，只需要在 sectionMPos 发生改变时调用 --
void colloect_nearFour_ecoObjDatas( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                    IntVec2 anyMPos_ ){
    IntVec2 sectionMPos = anyMPos_2_sectionMPos( anyMPos_ );
    sectionKey_t tmpKey {};
    container_.clear();
    for( const auto &whOff : bcd_inn::quadSectionKeyOffs ){
        tmpKey = sectionMPos_2_sectionKey( sectionMPos + whOff );
        auto outPair = container_.insert( esrc::get_ecoObj_readOnly( tmpKey ) );
        tprAssert( outPair.second );
    }
}



void assign_mapent_to_nearFour_ecoObjs( std::map<occupyWeight_t,EcoObj_ReadOnly> &container_,
                                        Job_MapEnt &mapEnt_ ){
    double         vx        {};
    double         vy        {};
    IntVec2        mposOff   {};
    double         freqBig   { 0.9 }; //- 值越小，ecoObj 边界越平滑
    double         freqSml   { 2.3 };

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
                                        (vy + 151.15) * freqBig ) * 17.0; // [-x.0, x.0]
    double pnValSml = simplex_noise2(    (vx + 244.41) * freqSml,
                                        (vy + 144.41) * freqSml ) * 5.0; // [-x.0, x.0]

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
                mapEnt_.colorTableId = ecoReadOnly.colorTableId;
                mapEnt_.density.set(mapEnt_.mpos, 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
                break; // MUST 
            }
        }else{ //- 第四个 eco
            mapEnt_.ecoObjKey = ecoReadOnly.sectionKey;
            mapEnt_.colorTableId = ecoReadOnly.colorTableId;
            mapEnt_.density.set(mapEnt_.mpos, 
                                ecoReadOnly.densitySeaLvlOff,
                                ecoReadOnly.densityDivideValsPtr );
        }
    }
}




}//-------------- namespace: bcd_inn end ----------------//

