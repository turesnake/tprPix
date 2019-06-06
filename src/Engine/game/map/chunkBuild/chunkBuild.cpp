/*
 * ==================== chunkBuild.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- C --------------------//
//#include <cassert>
#include <cstring>

//-------------------- CPP --------------------//
#include <set>
#include <unordered_map>
#include <thread>
#include <chrono>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Chunk.h"
#include "sectionKey.h"
#include "esrc_field.h"
#include "esrc_chunk.h"
#include "esrc_player.h"
#include "esrc_shader.h"
#include "esrc_ecoObj.h"
#include "esrc_jobQue.h"
#include "esrc_chunkData.h"

#include "jobs_all.h"
#include "Job.h"

#include "chunkBuild.h"


#include "tprDebug.h"


namespace chunkBuild { //------- namespace: chunkBuild ----------//


namespace{//----------- namespace ----------------//

    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    // 此数据 和 MapField.cpp 中存在重复
    const std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION, 0 },
        IntVec2{ 0, ENTS_PER_SECTION },
        IntVec2{ ENTS_PER_SECTION, ENTS_PER_SECTION }
    };


    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 

    //-- 正在被 build 的 chunk，要登记到这张表中。
    std::set<chunkKey_t> chunkQueBuilding {};

    //===== funcs =====//
    void fst_ecoObjs( const IntVec2 &_sectionMPos );
    void chunkBuild_1_push_job( chunkKey_t _chunkKey );
    void build_one_chunk( chunkKey_t _chunkKey );

    //-- 有关 chunkQueBuilding 状态表 的函数 --
    bool find_from_chunkQueBuilding( chunkKey_t _chunkKey );
    void insert_2_chunkQueBuilding( chunkKey_t _chunkKey );
    void erase_from_chunkQueBuilding( chunkKey_t _chunkKey );

}//-------------- namespace : end ----------------//



/* ===========================================================
 *                 build_9_chunks  [tmp]
 * -----------------------------------------------------------
 * 游戏最开始，一口气 创建 周边9个 chunk
 * -------
 *    非常临时随意的写法，在未来修改
 */
void build_9_chunks( const IntVec2 &_playerMPos ){

    IntVec2     playerChunkMPos = anyMPos_2_chunkMPos( _playerMPos );
    IntVec2     tmpChunkMPos;
    chunkKey_t  chunkKey;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            tmpChunkMPos.set(   playerChunkMPos.x + w*ENTS_PER_CHUNK,
                                playerChunkMPos.y + h*ENTS_PER_CHUNK );
            chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
            if( !esrc::find_from_chunks( chunkKey ) ){

                //-- 全新 跨线程方案 --
                chunkBuild_1_push_job( chunkKey );
                chunkBuild_4_wait_until_target_chunk_builded( chunkKey  );
            }
        }
    }
}


/* ===========================================================
 *    collect_chunks_need_to_be_build_in_update
 * -----------------------------------------------------------
 * 在游戏运行时，定期检查 玩家位置。及时生成 新的 chunk
 * 确保，玩家周边 9个chunk 始终存在
 * -------
 */
void collect_chunks_need_to_be_build_in_update(){

    IntVec2 playerMPos = esrc::get_player().goPtr->goPos.get_currentMPos();

    currentChunkKey = anyMPos_2_chunkKey( playerMPos );
    if( is_first_check ){
        is_first_check = false;
        lastChunkKey = currentChunkKey;
        return;
    }

    if( currentChunkKey != lastChunkKey ){
        lastChunkKey = currentChunkKey;
        //-- 全新 跨线程方案 --
        // 检查目标 chunk 周边9个chunk，如果哪个chunk 尚未生成，就将它 push到 全局容器 
        IntVec2      playerChunkMPos = chunkKey_2_mpos( currentChunkKey );
        IntVec2      tmpChunkMPos;
        chunkKey_t   tmpChunkKey;
        for( int h=-1; h<=1; h++ ){
            for( int w=-1; w<=1; w++ ){ //- nearby 9 个 chunk
                tmpChunkMPos.set(   playerChunkMPos.x + w*ENTS_PER_CHUNK,
                                    playerChunkMPos.y + h*ENTS_PER_CHUNK );
                tmpChunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
                if( !esrc::find_from_chunks(tmpChunkKey) ){

                    //-- 多一道判断，对于那些已经 进入build流程的 chunk 
                    //   也不用再重复登记了。不然会有 bug 
                    if( !find_from_chunkQueBuilding(tmpChunkKey) ){
                        chunkBuild_1_push_job( tmpChunkKey ); //-- 跨线程新方案
                    }
                }
            }
        } //- nearby 9 个 chunk: end --
    }
}



/* ===========================================================
 *     chunkBuild_3_receive_data_and_build_one_chunk
 * -----------------------------------------------------------
 * 三步：第三步：
 *  （第二步 由 job线程 完成）
 * 每一渲染帧，检查 esrc::chunkDataFlags, 从中取出一个 以及计算好 chunkData 的 chunk，
 * 然后生成 这个chunk 实例。
 * 每一帧仅限 1 个。
 * ----
 * 这个方法存在漏洞，所以还需要一道补救：在 get_memMapEntPtr() 中
 */
chunkKey_t chunkBuild_3_receive_data_and_build_one_chunk(){

    //-- 没有需要 生成的 chunk 时，直接退出 --
    if( esrc::atom_is_chunkDataFlags_empty() ){
        return 1; //- 不该这么写...
    }

    //-- 从 已经制作好 chunkData 的队列中，取出一个 chunk
    chunkKey_t chunkKey = esrc::atom_pop_from_chunkDataFlags();

        //-- 正式生成这个 chunk 实例
        build_one_chunk( chunkKey );
                //-- 实际上，目前的 job线程 是空的，
                //   所有运算 都在这个 函数中...


    //-- 及时删除 chunkData 数据本体 --
    esrc::atom_erase_from_chunkDatas( chunkKey ); //- MUST !!!
    erase_from_chunkQueBuilding( chunkKey ); //- MUST !!!
    return chunkKey;
}


/* ===========================================================
 *      chunkBuild_4_wait_until_target_chunk_builded   
 * -----------------------------------------------------------
 * 被用于 esrc::get_memMapEntPtr()
 *       build_9_chunks()
 */
void chunkBuild_4_wait_until_target_chunk_builded( chunkKey_t _chunkKey ){

    chunkKey_t tmpChunkKey;
    while( true ){
        //-- 没有需要 生成的 chunk 时，待机一会儿，再次 while 循环 --
        if( esrc::atom_is_chunkDataFlags_empty() ){
            std::this_thread::sleep_for( std::chrono::milliseconds(5) );
            continue;
        }
        tmpChunkKey = chunkBuild_3_receive_data_and_build_one_chunk();
        tprAssert( tmpChunkKey != 1 );
        if( tmpChunkKey == _chunkKey ){
            return;
        }
    }
}


namespace{//----------- namespace ----------------//


/* ===========================================================
 *                   build_one_chunk
 * -----------------------------------------------------------
 */
void build_one_chunk( chunkKey_t _chunkKey ){

            //   调用本函数，说明一定处于 “无视存储” 的早期阶段。

    IntVec2 targetChunkMPos = chunkKey_2_mpos( _chunkKey );

    //------------------------------//
    //           [1]
    // 创建 周边 4个 ecoObj 实例
    //------------------------------//
    //-- 已被移到 chunkBuild_1_push_job() 中
    

    //------------------------------//
    //            [2]
    //  集中生成 周边 4chunk 的 所有 fields
    //------------------------------//
    //-- 已被移到 job线程中 运行
    

    //------------------------------//
    //            [3]
    //    单独生成 主chunk 实例
    //------------------------------//
    Chunk *chunkPtr = esrc::insert_and_init_new_chunk( targetChunkMPos, &(esrc::get_rect_shader()) );//- 一定不存在


    //------------------------------//
    //            [4]
    //  为 chunk 中的 8*8 个 field，分配 goes
    //------------------------------//
    for( const auto &fieldKey : chunkPtr->get_fieldKeys() ){ //- each field key

        esrc::atom_create_a_go_in_field( fieldKey );
    } //-- each field key end --


    //------------------------------//
    //            [5]
    //  生成 刷怪笼（基于field）
    //          tmp...
    //------------------------------//



}


/* ===========================================================
 *              chunkBuild_1_push_job
 * -----------------------------------------------------------
 * 三步：第一步：
 * 根据 目标chunk 制作成job，发送到 jobQue 
 */
void chunkBuild_1_push_job( chunkKey_t _chunkKey ){

    //------------------------------//
    //           [1]
    // 创建 周边 4个 ecoObj 实例
    //------------------------------//
    //  在最坏的情况下，这部分会一口气 创建 5个 ecoObj 实例（1个渲染帧内）
    //  而且是在 主线程上计算。如果 ecoObj 实例 创建成本不高，
    //  那么还可以接受
    IntVec2 targetChunkMPos = chunkKey_2_mpos( _chunkKey );
    IntVec2  tmpChunkMPos;
    for( size_t h=0; h<=1; h++ ){
        for( size_t w=0; w<=1; w++ ){ //- 周边 4 个chunk
            tmpChunkMPos.set(   targetChunkMPos.x + static_cast<int>(w*ENTS_PER_CHUNK),
                                targetChunkMPos.y + static_cast<int>(h*ENTS_PER_CHUNK) );
            fst_ecoObjs( anyMPos_2_sectionMPos(tmpChunkMPos) );
        }
    }

    //--------------------------//
    //       push job
    //--------------------------//
    ArgBinary_Build_ChunkData arg;
    arg.chunkKey = _chunkKey;
    //----------
    Job  job {};
    job.jobType = JobType::Build_ChunkData;
    job.argBinary.resize( sizeof(ArgBinary_Build_ChunkData), 0 );
    memcpy( (void*)&(job.argBinary.at(0)),
            (void*)&arg,
            sizeof(ArgBinary_Build_ChunkData) );
    //----------
    esrc::atom_push_back_2_jobQue( job );

    //--------------------------//
    //  进入被 build流程的 chunk 需要被登记
    //  防止被重复创建
    //--------------------------//
    insert_2_chunkQueBuilding( _chunkKey );
}


/* ===========================================================
 *             fst_ecoObjs
 * -----------------------------------------------------------
 * 第一阶段
 */
void fst_ecoObjs( const IntVec2 &_sectionMPos ){

    sectionKey_t  tmpSectionKey;
    for( const auto &whOff : quadSectionKeyOffs ){
        tmpSectionKey = sectionMPos_2_sectionKey( _sectionMPos + whOff );
                //ecoObj::find_or_create_the_ecoObj( tmpSectionKey );
                //-- 这个函数 应该内置到 esrc 原子函数内

        esrc::atom_try_to_inert_and_init_a_ecoObj( tmpSectionKey );
    }
}



/* ===========================================================
 *            chunkQueBuilding funcs
 * -----------------------------------------------------------
 *  building 表。由于只在 主线程存在，所以不用 加锁
 */
bool find_from_chunkQueBuilding( chunkKey_t _chunkKey ){
    return (chunkQueBuilding.find(_chunkKey) != chunkQueBuilding.end());
}
void insert_2_chunkQueBuilding( chunkKey_t _chunkKey ){
        tprAssert( chunkQueBuilding.find(_chunkKey) == chunkQueBuilding.end() );
    chunkQueBuilding.insert(_chunkKey );
}
void erase_from_chunkQueBuilding( chunkKey_t _chunkKey ){
    tprAssert( chunkQueBuilding.erase(_chunkKey) == 1 );
}



}//-------------- namespace : end ----------------//
}//----------------- namespace: chunkBuild: end -------------------//
