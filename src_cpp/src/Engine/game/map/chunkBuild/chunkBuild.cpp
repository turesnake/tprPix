/*
 * ==================== chunkBuild.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- C --------------------//
#include <cstring>

//-------------------- CPP --------------------//
#include <set>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Chunk.h"
#include "sectionKey.h"
#include "esrc_field.h"
#include "esrc_chunk.h"
#include "esrc_player.h"
#include "esrc_ecoObj.h"
#include "esrc_jobQue.h"
#include "esrc_chunkData.h"

#include "jobs_all.h"
#include "Job.h"

#include "chunkBuild.h"

#include "tprDebug.h"


namespace chunkBuild {//------- namespace: chunkBuild ----------//
namespace cb_inn {//----------- namespace: cb_inn ----------------//

    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    // 此数据 和 MapField.cpp 中存在重复
    const std::vector<IntVec2> nearby_4_sectionMPosOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION, 0 },
        IntVec2{ 0, ENTS_PER_SECTION },
        IntVec2{ ENTS_PER_SECTION, ENTS_PER_SECTION }
    };

    //- 周边4个 chunk mpos 偏移，
    const std::vector<IntVec2> nearby_4_chunkMPosOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_CHUNK, 0 },
        IntVec2{ 0, ENTS_PER_CHUNK },
        IntVec2{ ENTS_PER_CHUNK, ENTS_PER_CHUNK }
    };
    

    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 

    //===== funcs =====//
    void chunkBuild_1_push_job( chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ );
    void build_one_chunk( chunkKey_t chunkKey_ );

}//-------------- namespace: cb_inn end ----------------//



/* ===========================================================
 *                 build_9_chunks  [tmp]
 * -----------------------------------------------------------
 * 游戏最开始，一口气 创建 周边9个 chunk
 * -------
 *    非常临时随意的写法，在未来修改
 * 
 *    这个名字有待修改，现在，不是固定生成 9个，具体数目由 chunkCreateZone 决定
 * 
 */
void build_9_chunks( const IntVec2 &playerMPos_ ){

    esrc::init_chunkCreateReleaseZone( playerMPos_ );
    //---------
    IntVec2     playerChunkMPos = anyMPos_2_chunkMPos( playerMPos_ );
    IntVec2     tmpChunkMPos {};
    chunkKey_t  chunkKey     {};

    const auto &chunkOffMPoses = esrc::get_chunkCreateZoneRef().get_chunkOffMPoses();
    for( const auto &iOffMPos : chunkOffMPoses ){

        tmpChunkMPos = playerChunkMPos + iOffMPos;
        chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);

        tprAssert( esrc::get_chunkMemState(chunkKey) == ChunkMemState::NotExist ); // MUST
        cb_inn::chunkBuild_1_push_job( chunkKey, tmpChunkMPos ); //-- 正式创建，跨线程新方案
        chunkBuild_4_wait_until_target_chunk_builded( chunkKey  );
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

    IntVec2 playerMPos = esrc::get_player().get_goRef().goPos.get_currentMPos();

    cb_inn::currentChunkKey = anyMPos_2_chunkKey( playerMPos );
    if( cb_inn::is_first_check ){
        cb_inn::is_first_check = false;
        cb_inn::lastChunkKey = cb_inn::currentChunkKey;
        return;
    }

    if( cb_inn::currentChunkKey != cb_inn::lastChunkKey ){
        cb_inn::lastChunkKey = cb_inn::currentChunkKey;
        //-- 全新 跨线程方案 --
        // 检查目标 chunk 周边9个chunk，如果哪个chunk 尚未生成，就将它 push到 全局容器 
        IntVec2      playerChunkMPos = chunkKey_2_mpos( cb_inn::currentChunkKey );
        IntVec2      tmpChunkMPos {};
        chunkKey_t   tmpChunkKey  {};

        const auto &chunkOffMPoses = esrc::get_chunkCreateZoneRef().get_chunkOffMPoses();
        for( const auto &iOffMPos : chunkOffMPoses ){
            
            tmpChunkMPos = playerChunkMPos + iOffMPos;
            tmpChunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);

            auto chunkState = esrc::get_chunkMemState( tmpChunkKey );
            switch (chunkState){
                case ChunkMemState::NotExist:
                    cb_inn::chunkBuild_1_push_job( tmpChunkKey, tmpChunkMPos ); //-- 正式创建，跨线程新方案
                    break;
                case ChunkMemState::OnCreating:
                case ChunkMemState::Active:
                    // do nothing
                    break;
                case ChunkMemState::WaitForRelease:
                    esrc::move_chunkKey_from_WaitForRelease_2_active( tmpChunkKey );
                    break;

                case ChunkMemState::OnReleasing:
                    // 需要新建的 chunk 居然正在被释放中，一定是参数设置出了问题
                    // 目前的选择是直接报错
                    tprAssert(0);
                    break;

                default:
                    tprAssert(0);
                    break;
                }
        }
    }
}



/* ===========================================================
 *     chunkBuild_3_receive_data_and_build_one_chunk
 * -----------------------------------------------------------
 * 三步：第三步：
 *  （第二步 由 job线程 完成）
 * 每一渲染帧，检查 esrc::chunkDataFlags, 从中取出一个 已经计算好 chunkData 的 chunk，
 * 然后生成 这个chunk 实例。
 * 每一帧仅限 1 个。
 * ----
 * 这个方法存在漏洞，所以还需要一道补救：在 get_memMapEntRef() 中
 */
chunkKey_t chunkBuild_3_receive_data_and_build_one_chunk(){

    //-- 没有需要 生成的 chunk 时，直接退出 --
    if( esrc::atom_is_chunkDataFlags_empty() ){
        return 1; //- 不该这么写...
    }

    //-- 从 已经制作好 chunkData 的队列中，取出一个 chunk
    chunkKey_t chunkKey = esrc::atom_pop_from_chunkDataFlags();
    esrc::move_chunkKey_from_onCreating_2_active(chunkKey);  //- MUST before build_one_chunk

        //-- 正式生成这个 chunk 实例
        cb_inn::build_one_chunk( chunkKey );
                //-- 实际上，目前的 job线程 是空的，
                //   所有运算 都在这个 函数中...

    //-- 及时删除 chunkData 数据本体 --
    esrc::atom_erase_from_chunkDatas( chunkKey ); //- MUST !!!    
    return chunkKey;
}


/* ===========================================================
 *      chunkBuild_4_wait_until_target_chunk_builded   
 * -----------------------------------------------------------
 * 被用于 esrc::get_memMapEntRef()
 *       build_9_chunks()
 */
void chunkBuild_4_wait_until_target_chunk_builded( chunkKey_t chunkKey_ ){

    chunkKey_t tmpChunkKey {};
    while( true ){
        //-- 没有需要 生成的 chunk 时，待机一会儿，再次 while 循环 --
        if( esrc::atom_is_chunkDataFlags_empty() ){
            std::this_thread::sleep_for( std::chrono::milliseconds(5) );
            continue;
        }
        tmpChunkKey = chunkBuild_3_receive_data_and_build_one_chunk();
        tprAssert( tmpChunkKey != 1 );
        if( tmpChunkKey == chunkKey_ ){
            return;
        }
    }
}


namespace cb_inn {//----------- namespace: cb_inn ----------------//


/* ===========================================================
 *                   build_one_chunk
 * -----------------------------------------------------------
 */
void build_one_chunk( chunkKey_t chunkKey_ ){

            //   调用本函数，说明一定处于 “无视存储” 的早期阶段。

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
    Chunk &chunkRef = esrc::insert_and_init_new_chunk(chunkKey_);//- 一定不存在

    //------------------------------//
    //            [4]
    //  为 chunk 中的 8*8 个 field，分配 goes
    //------------------------------//
    for( const auto &fieldKey : chunkRef.get_fieldKeys() ){ //- each field key
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
void chunkBuild_1_push_job( chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ ){
    //------------------------------//
    //           [1]
    // 创建 周边 4个 ecoObj 实例
    //------------------------------//
    //  在最坏的情况下，这部分会一口气 创建 5个 ecoObj 实例（1个渲染帧内）
    //  而且是在 主线程上计算。如果 ecoObj 实例 创建成本不高，
    //  那么还可以接受
    std::set<sectionKey_t> sectionKeys {}; //- 为了去除重复
    IntVec2 tmpSectionMPos {};
    for( auto &iOff : nearby_4_chunkMPosOffs ){
        tmpSectionMPos = anyMPos_2_sectionMPos( iOff + chunkMPos_ );
        for( const auto &jOff : nearby_4_sectionMPosOffs ){
            sectionKeys.insert( sectionMPos_2_sectionKey( tmpSectionMPos + jOff ) );
        }
    }
    for( auto &key : sectionKeys ){
        esrc::atom_try_to_inert_and_init_a_ecoObj( key );
    }

    //--------------------------//
    //       push job
    //--------------------------//
    ArgBinary_Build_ChunkData arg {};
    arg.chunkKey = chunkKey_;
    //----------
    auto jobSPtr = std::make_shared<Job>();
    jobSPtr->jobType = JobType::Build_ChunkData;
    jobSPtr->argBinary.resize( sizeof(ArgBinary_Build_ChunkData), 0 );
    memcpy( (void*)&(jobSPtr->argBinary.at(0)),
            (void*)&arg,
            sizeof(ArgBinary_Build_ChunkData) );
    //----------
    esrc::atom_push_back_2_jobQue( jobSPtr );
    //--------------------------//
    //  进入被 build流程的 chunk 需要被登记
    //  防止被重复创建
    //--------------------------//
    esrc::insert_2_chunkKeys_onCreating( chunkKey_ );
}


}//-------------- namespace: cb_inn end ----------------//
}//----------------- namespace: chunkBuild: end -------------------//
