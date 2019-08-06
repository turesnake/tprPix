/*
 * ==================== chunkCreate.cpp ======================
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
#include "NineBox.h"
#include "Chunk.h"
#include "sectionKey.h"
#include "esrc_gameObj.h"
#include "esrc_field.h"
#include "esrc_chunk.h"
#include "esrc_player.h"
#include "esrc_ecoObj.h"
#include "esrc_jobQue.h"
#include "esrc_chunkData.h"
#include "esrc_colliEntSet.h"

#include "jobs_all.h"
#include "Job.h"

#include "chunkCreate.h"

#include "tprDebug.h"


namespace chunkCreate {//------- namespace: chunkCreate ----------//
namespace cb_inn {//----------- namespace: cb_inn ----------------//

    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    // 此数据 和 MapField.cpp 中存在重复
    const std::vector<IntVec2> nearby_4_sectionMPosOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION, 0 },
        IntVec2{ 0, ENTS_PER_SECTION },
        IntVec2{ ENTS_PER_SECTION, ENTS_PER_SECTION }
    };

    //- 周边4个 chunk mpos 偏移，[left-bottom]
    const std::vector<IntVec2> nearby_4_chunkMPosOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_CHUNK, 0 },
        IntVec2{ 0, ENTS_PER_CHUNK },
        IntVec2{ ENTS_PER_CHUNK, ENTS_PER_CHUNK }
    };


    //- 周边8个 chunk mpos 偏移，[九宫格]
    const std::vector<IntVec2> nearby_8_chunkMPosOffs {
        IntVec2{ -ENTS_PER_CHUNK, -ENTS_PER_CHUNK },
        IntVec2{ -ENTS_PER_CHUNK, 0 },
        IntVec2{ -ENTS_PER_CHUNK, ENTS_PER_CHUNK },
        //--
        IntVec2{ 0, -ENTS_PER_CHUNK },
        IntVec2{ 0, ENTS_PER_CHUNK },
        //--
        IntVec2{ ENTS_PER_CHUNK, -ENTS_PER_CHUNK },
        IntVec2{ ENTS_PER_CHUNK, 0 },
        IntVec2{ ENTS_PER_CHUNK, ENTS_PER_CHUNK }
    };


    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 

    //===== funcs =====//
    void chunkBuild_1_push_job( chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ );
    void build_one_chunk( chunkKey_t chunkKey_ );
    void signUp_nearby_chunks_edgeGo_2_mapEnt( chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ );
    void wait_until_target_chunk_builded( chunkKey_t chunkKey_ );

    NineBoxIdx calc_player_move_dir( chunkKey_t oldKey_, chunkKey_t newKey_ );

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

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 统一生成 周边 5*5 个 chunk
            tmpChunkMPos.set(   playerChunkMPos.x + w*ENTS_PER_CHUNK,
                                playerChunkMPos.y + h*ENTS_PER_CHUNK );
            chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
            tprAssert( esrc::get_chunkMemState(chunkKey) == ChunkMemState::NotExist ); // MUST
            cb_inn::chunkBuild_1_push_job( chunkKey, tmpChunkMPos ); //-- 正式创建，跨线程新方案
            cb_inn::wait_until_target_chunk_builded(chunkKey);
                        // 此处禁止优化，必须逐个创建，逐个确认
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

    IntVec2 playerMPos = esrc::get_player().get_goRef().goPos.get_currentMPos();

    cb_inn::currentChunkKey = anyMPos_2_chunkKey( playerMPos );
    if( cb_inn::is_first_check ){
        cb_inn::is_first_check = false;
        cb_inn::lastChunkKey = cb_inn::currentChunkKey;
        return;
    }

    if( cb_inn::currentChunkKey != cb_inn::lastChunkKey ){

        //-- 根据 新chunk 和 旧chunk 的位置关系，可以得知，player 的运动方向
        //   根据这个运动方向，调整 创建周边 chunk 的排序。
        //   从而让 运动方向前方的 chunk，更早被创建。
        NineBoxIdx moveDir = cb_inn::calc_player_move_dir(  cb_inn::lastChunkKey,
                                                            cb_inn::currentChunkKey );

        cb_inn::lastChunkKey = cb_inn::currentChunkKey;
        //-- 全新 跨线程方案 --
        // 检查目标 chunk 周边9个chunk，如果哪个chunk 尚未生成，就将它 push到 全局容器 
        IntVec2      playerChunkMPos = chunkKey_2_mpos( cb_inn::currentChunkKey );
        IntVec2      tmpChunkMPos {};
        chunkKey_t   tmpChunkKey  {};

        const auto &chunkOffMPoses = esrc::get_chunkCreateReleaseZoneRef().get_createZoneOffMPoses( moveDir );
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
                    // 需要新建的 chunk 居然在 释放队列中，一定是参数设置出了问题 [严格版本]
                    // 目前的选择是直接报错
                    tprAssert(0);
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
 */
std::pair<bool,chunkKey_t> chunkBuild_3_receive_data_and_build_one_chunk(){

    //-- 没有需要 生成的 chunk 时，直接退出 --
    if( esrc::atom_is_chunkDataFlags_empty() ){
        return std::pair<bool,chunkKey_t>{ false, 0 };
    }

    //-- 从 已经制作好 chunkData 的队列中，取出一个 chunk
    chunkKey_t chunkKey = esrc::atom_pop_from_chunkDataFlags();
    
        //-- 正式生成这个 chunk 实例
        cb_inn::build_one_chunk( chunkKey );
                //-- 实际上，目前的 job线程 是空的，
                //   所有运算 都在这个 函数中...

    //-- 及时删除 chunkData 数据本体 --
    esrc::atom_erase_from_chunkDatas( chunkKey ); //- MUST !!!  
    return std::pair<bool,chunkKey_t>{ true, chunkKey };
}




namespace cb_inn {//----------- namespace: cb_inn ----------------//

/* ===========================================================
 *            wait_until_target_chunk_builded   
 * -----------------------------------------------------------
 * 被用于 build_9_chunks()
 */
void wait_until_target_chunk_builded( chunkKey_t chunkKey_ ){

    std::pair<bool,chunkKey_t> pairRet {};
    while( true ){
        //-- 没有需要 生成的 chunk 时，待机一会儿，再次 while 循环 --
        if( esrc::atom_is_chunkDataFlags_empty() ){
            std::this_thread::sleep_for( std::chrono::milliseconds(5) );
            continue;
        }
        pairRet = chunkBuild_3_receive_data_and_build_one_chunk();
        tprAssert( pairRet.first );
        if( pairRet.second == chunkKey_ ){
            return;
        }
    }
}


/* ===========================================================
 *                   build_one_chunk
 * -----------------------------------------------------------
 */
void build_one_chunk( chunkKey_t chunkKey_ ){

            //   调用本函数，说明一定处于 “无视存储” 的早期阶段。

    IntVec2 chunkMPos = chunkKey_2_mpos( chunkKey_ );

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
    Chunk &chunkRef = esrc::insert_and_init_new_chunk(chunkKey_);
                                    // Now, the chunkState is Active !!! 

    //------------------------------//
    //            [4]
    // 遍历周边 8 个chunk，如果某一chunk 已经存在
    // 遍历此chunk 的 所有 edgeGoIds, 如果某个 edgeGo，其 collient 位于本chunk
    // 则将这些 collients 登记到 对应的 mapent 上
    // 针对 GameObj::signUp_newGO_to_mapEnt() 的一个补充
    //------------------------------//
    cb_inn::signUp_nearby_chunks_edgeGo_2_mapEnt( chunkKey_, chunkMPos );

    //------------------------------//
    //            [5]
    //  为 chunk 中的 8*8 个 field，分配 goes
    //------------------------------//
    for( const auto &fieldKey : chunkRef.get_fieldKeys() ){ //- each field key
        esrc::atom_create_a_go_in_field( fieldKey );
    } //-- each field key end --

    //------------------------------//
    //            [6]
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
    for( const auto &iOff : nearby_4_chunkMPosOffs ){
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


/* ===========================================================
 *        signUp_nearby_chunks_edgeGo_2_mapEnt
 * -----------------------------------------------------------
 * 主动访问 本chunk 的周边 8 个chunk 的 edgeGoIds，
 * 如果某个 edgeGo，与本chunk 有关系。
 * 就将这个 edgeGO 的 特定 colliEnts，登记到 本chunk 的 对应 mapent 上
 * ---
 * 这是一道 补充工作
 */
void signUp_nearby_chunks_edgeGo_2_mapEnt( chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ ){

    IntVec2  cesMPos {};      //- rootCES left-bottom mcpos [world-abs-pos]
    IntVec2  colliEntMPos {}; //- each collient mcpos [world-abs-pos]

    IntVec2    tmpChunkMPos {};
    chunkKey_t tmpChunkKey  {};
    for( const auto &iOff : nearby_8_chunkMPosOffs ){
        tmpChunkMPos = chunkMPos_ + iOff;
        tmpChunkKey = chunkMPos_2_chunkKey( tmpChunkMPos );

        if( ChunkMemState::Active == esrc::get_chunkMemState(tmpChunkKey) ){
            
            auto &chunkRef = esrc::get_chunkRef( tmpChunkKey );
                                    //- 此函数内部也会做一道检测，且只允许 active 状态

            for( auto &goid : chunkRef.get_edgeGoIds() ){//- foreach edgeGoId

                auto &goRef = esrc::get_goRef(goid);
                if( goRef.find_in_chunkKeys(chunkKey_) ){

                    const ColliEntHead &cehRef = goRef.get_rootColliEntHeadRef();
                    const ColliEntSet &cesRef = esrc::get_colliEntSetRef( cehRef.colliEntSetIdx );
                    cesMPos = goRef.get_rootCES_leftBottom_MPos();

                    for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in rootCES

                        colliEntMPos = i.get_mpos() + cesMPos;
                        if( chunkKey_ == anyMPos_2_chunkKey(colliEntMPos) ){

                            //---- 正式注册 collient 到 mapents 上 -----
                            auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( colliEntMPos );

                            mapEntRef.insert_2_major_gos(   goRef.id, 
                                                            cehRef.lGoAltiRange,
                                                            cehRef.isCarryAffect );
                        }
                    }
                }
            }
        }
    }
}


/* ===========================================================
 *                calc_player_move_dir
 * -----------------------------------------------------------
 */
NineBoxIdx calc_player_move_dir( chunkKey_t oldKey_, chunkKey_t newKey_ ){

    IntVec2 offMPos = chunkKey_2_mpos(newKey_) - chunkKey_2_mpos(oldKey_);
    offMPos = offMPos.floorDiv( static_cast<double>(ENTS_PER_CHUNK) );
    //- 想要创建成功，xy值必须在 [-1,1] 区间内
    NineBox nb { offMPos.x, offMPos.y };
    return NineBox_XY_2_Idx( nb );
}





}//-------------- namespace: cb_inn end ----------------//
}//----------------- namespace: chunkCreate: end -------------------//
