/*
 * =================== build_chunkData.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build chunkData
 * ----------------------------
 */
//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
#include <cassert>
#include <cstring>
#include <cmath>

//-------------------- CPP --------------------//


//-------------------- Engine --------------------//
#include "jobs_all.h"
#include "Job.h"
#include "simplexNoise.h"
#include "config.h"
#include "esrc_chunkData.h"
#include "esrc_gameSeed.h"
#include "esrc_field.h"
#include "esrc_ecoSysInMap.h"

/*
#include <iostream>
#include <string>
using std::cout;
using std::endl;
*/

namespace{//----------- namespace ----------------//

    //-- 不要随意建立 static数据，本文件的代码，会被数个 job线程 调用 --


    //- 在未来，freq 这组值也会收到 ecosys 影响 --
    const float freqSeaLvl { 0.05 };
    const float freqBig    { 0.4 };
    const float freqMid    { 1.6 };
    const float freqSml    { 4.0 };

    const float zOffBig  { 0.2 };
    const float zOffMid  { 7.5 };
    const float zOffSml  { 17.8 };

    const glm::vec2  worldCenter { 0.0, 0.0 };



    //===== funcs =====//
    void calc_pixAltis( const IntVec2 &_chunkMPos, ChunkData *_chunkDataPtr );

    

}//-------------- namespace : end ----------------//



/* ===========================================================
 *                build_chunkData_main
 * -----------------------------------------------------------
 */
void build_chunkData_main( const Job &_job ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    assert( _job.argBinary.size() == sizeof(ArgBinary_Build_ChunkData) );
    ArgBinary_Build_ChunkData arg;
    memcpy( (void*)&arg,
            (void*)&(_job.argBinary.at(0)),
            sizeof(ArgBinary_Build_ChunkData) );


    //-- 制作一个 ChunkData 数据实例 --
    ChunkData *chunkDataPtr = esrc::atom_insert_new_chunkData( arg.chunkKey );
    IntVec2 chunkMPos = chunkKey_2_mpos( arg.chunkKey );

            //-- 执行 chunkData 数据计算
            //...

    //------------------------------//
    //           [1]
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoSysInMap )
    //------------------------------//
    // 已经在 主线程 chunkBuild_1_push_job() 中 提前完成
    // 反正再糟糕也不过是，1帧内创建 5 个 ecosysinmap 实例


    //------------------------------//
    //            [2]
    //  集中生成 周边 4chunk 的 所有 fields
    //------------------------------//
    IntVec2  tmpFieldMPos;
    for( size_t h=0; h<FIELDS_PER_CHUNK*2; h++ ){
        for( size_t w=0; w<FIELDS_PER_CHUNK*2; w++ ){ //- each field in 2*2chunks
            tmpFieldMPos.set(   chunkMPos.x + w*ENTS_PER_FIELD,
                                chunkMPos.y + h*ENTS_PER_FIELD );
            esrc::atom_try_to_insert_and_init_the_field_ptr( tmpFieldMPos );
        }
    } //- each field in 2*2chunks


    //--------------------------//
    //       pixAltis
    //--------------------------//
    calc_pixAltis( chunkMPos, chunkDataPtr );


    //--------------------------//
    //-- chunkData 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 chunk 数据准备好了
    //--------------------------//
    esrc::atom_push_back_2_chunkDataFlags( arg.chunkKey );
    
        /*
        cout << "    build_chunkData_main(): "
            << "chunkMPos: " << chunkMPos.x
            << ", " << chunkMPos.y
            << "; DONE;"
            << endl;
        */
}


namespace{//----------- namespace ----------------//


/* ===========================================================
 *                 calc_pixAltis
 * -----------------------------------------------------------
 */
void calc_pixAltis( const IntVec2 &_chunkMPos, ChunkData *_chunkDataPtr ){


    IntVec2      chunkPPos = mpos_2_ppos(_chunkMPos);
    glm::vec2    pixCFPos; //- 以 chunk 为晶格的 fpos

    glm::vec2  altiSeed_pposOffBig = esrc::gameSeed.altiSeed_pposOffBig;
    glm::vec2  altiSeed_pposOffMid = esrc::gameSeed.altiSeed_pposOffMid;
    glm::vec2  altiSeed_pposOffSml = esrc::gameSeed.altiSeed_pposOffSml;

                            //-- 此处有问题，从 job线程 访问 gameSeed，不够安全...


    float      pixDistance; //- pix 距离 世界中心的距离。 暂时假设，(0,0) 为世界中心
    float      seaLvl;

    float    pnValBig;
    float    pnValMid;
    float    pnValSml;
    float    altiVal;  //- target val

    size_t   pixIdx;

    _chunkDataPtr->resize_pixAltis();
    for( int h=0; h<PIXES_PER_CHUNK; h++ ){
        for( int w=0; w<PIXES_PER_CHUNK; w++ ){//- each pix in chunk


            pixCFPos.x = static_cast<float>(chunkPPos.x+w) / PIXES_PER_CHUNK;
            pixCFPos.y = static_cast<float>(chunkPPos.y+h) / PIXES_PER_CHUNK;

            //------------------//
            //     seaLvl
            //------------------//
            pixDistance = glm::distance( pixCFPos, worldCenter );
            pixDistance /= 10.0;
            //--------
            seaLvl = simplex_noise2( pixCFPos * freqSeaLvl ) * 50.0; // [-50.0, 50.0]
            seaLvl += pixDistance;
            if( seaLvl < 0.0 ){ //- land
                seaLvl *= 0.3;  // [-15.0, 50.0]
            }

            //------------------//
            //    alti.val
            //------------------//
            //--- 使用速度最快的 2D-simplex-noise ---
            pnValBig = simplex_noise2( (pixCFPos + altiSeed_pposOffBig) * freqBig ) * 100.0 - seaLvl; // [-100.0, 100.0]
            pnValMid = simplex_noise2( (pixCFPos + altiSeed_pposOffMid) * freqMid ) * 50.0  - seaLvl; // [-50.0, 50.0]
            pnValSml = simplex_noise2( (pixCFPos + altiSeed_pposOffSml) * freqSml ) * 20.0  - seaLvl; // [-20.0, 20.0]
            //---------
            altiVal = floor(pnValBig + pnValMid + pnValSml);

            //------- 抹平头尾 -------//
            if( altiVal > 100.0 ){
                altiVal = 100.0;
            }else if( altiVal < -100.0 ){
                altiVal = -100.0;
            }
            // now, altiVal: [-100.0, 100.0]

            //------------------//
            //   写入 chunkData
            //------------------//
            pixIdx = h * PIXES_PER_CHUNK + w;
            _chunkDataPtr->set_ent_in_pixAltis( pixIdx, altiVal );

        }
    } //- each pix in chunk: end --
}



}//-------------- namespace : end ----------------//

