/*
 * ==================== chunkCreate.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- C --------------------//
#include <cstring>

//-------------------- CPP --------------------//
#include <thread>
#include <chrono>

//-------------------- Engine --------------------//
#include "NineDirection.h"
#include "Chunk.h"
#include "sectionKey.h"
#include "create_goes.h"

#include "esrc_gameObj.h"
#include "esrc_field.h"
#include "esrc_chunk.h"
#include "esrc_player.h"
#include "esrc_ecoObj.h"
#include "esrc_jobQue.h"
#include "esrc_job_chunk.h"
#include "esrc_job_ecoObj.h"
#include "esrc_time.h" // debug

#include "jobs_all.h"
#include "Job.h"

#include "chunkCreate.h"



namespace chunkCreate {//------- namespace: chunkCreate ----------//
namespace cb_inn {//----------- namespace: cb_inn ----------------//

    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    // 此数据 和 MapField.cpp 中存在重复
    const std::vector<IntVec2> nearby_4_sectionMPosOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION<>, 0 },
        IntVec2{ 0, ENTS_PER_SECTION<> },
        IntVec2{ ENTS_PER_SECTION<>, ENTS_PER_SECTION<> }
    };


    //- 周边8个 chunk mpos 偏移，[九宫格]
    const std::vector<IntVec2> nearby_8_chunkMPosOffs {
        IntVec2{ -ENTS_PER_CHUNK<>, -ENTS_PER_CHUNK<> },
        IntVec2{ -ENTS_PER_CHUNK<>, 0 },
        IntVec2{ -ENTS_PER_CHUNK<>, ENTS_PER_CHUNK<> },
        //--
        IntVec2{ 0, -ENTS_PER_CHUNK<> },
        IntVec2{ 0, ENTS_PER_CHUNK<> },
        //--
        IntVec2{ ENTS_PER_CHUNK<>, -ENTS_PER_CHUNK<> },
        IntVec2{ ENTS_PER_CHUNK<>, 0 },
        IntVec2{ ENTS_PER_CHUNK<>, ENTS_PER_CHUNK<> }
    };


    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 


    //===== funcs =====//
    void chunkCreate_1_push_job( chunkKey_t chunkKey_, IntVec2 chunkMPos_ );
    void create_one_chunk( chunkKey_t chunkKey_ );
    void signUp_nearby_chunks_edgeGo_2_mapEnt( chunkKey_t chunkKey_, IntVec2 chunkMPos_ );
    void wait_until_target_chunk_created( chunkKey_t chunkKey_ );

    void push_chunk_2_jobQue( chunkKey_t chunkKey_ );
    bool is_chunk_near4EcoObjs_all_active(  IntVec2 chunkMPos_, bool isInnCall_ );

    NineDirection calc_player_move_dir( chunkKey_t oldKey_, chunkKey_t newKey_ );

}//-------------- namespace: cb_inn end ----------------//


extern void push_ecoObj_2_jobQue( sectionKey_t ecoObjKey_ );



/* 游戏最开始，一口气 创建 周边 n个 chunk （暂定为 9 个）
 */
void create_9_chunks( IntVec2 playerMPos_ ){

    esrc::init_chunkCreateReleaseZone( playerMPos_ );
    //---------
    IntVec2     playerChunkMPos = anyMPos_2_chunkMPos( playerMPos_ );
    IntVec2     playerSectionMPos = anyMPos_2_sectionMPos( playerMPos_ );
    //----------------------------//
    // 一次性生成 周边 5*5 个 ecoObj
    //----------------------------//
    //-- 收集 所有 ecoObjKeys --
    //   并将它们压入 job 线程
    std::set<sectionKey_t> near_25_ecoObjKeys {};
    for( int h=-2; h<=2; h++ ){
        for( int w=-2; w<=2; w++ ){            
            IntVec2 tmpSectionMPos = playerSectionMPos + IntVec2{ w*ENTS_PER_SECTION<>, h*ENTS_PER_SECTION<> };
            sectionKey_t ecoObjKey = sectionMPos_2_sectionKey( tmpSectionMPos );
            near_25_ecoObjKeys.insert( ecoObjKey );            
        }
    }

    //-- 全部 压入 job线程
    tprAssert( near_25_ecoObjKeys.size() == 25 );
    for( const auto &key : near_25_ecoObjKeys ){
        push_ecoObj_2_jobQue( key );
        esrc::insert_ecoObjKey_2_onCreating( key );
    }

    //-- 不停检查，直到 25个 ecoobj 全部创建完成 --
    size_t finishedNum {0};
    size_t tmpNum {};
    
    while( true ){
        //-- 收集所有 已在 job线程中 创建完毕的 ecoobjs --
        tmpNum = esrc::atom_move_all_ecoObjUptrs_from_job_2_esrc();

        finishedNum += tmpNum;
        if( finishedNum == 25 ){
            break;
        }
        //--
        std::this_thread::sleep_for( std::chrono::milliseconds(5) ); // 待机一段时间
    }

        tprDebug::console( "5*5 ecoobjs create Done! " );


    //----------------------------//
    // 一次性生成 周边 3*3 个 chunk
    //----------------------------//
    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){
            IntVec2 tmpChunkMPos = playerChunkMPos + IntVec2{ w*ENTS_PER_CHUNK<>, h*ENTS_PER_CHUNK<> };          
            chunkKey_t chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
            tprAssert( esrc::get_chunkMemState(chunkKey) == ChunkMemState::NotExist ); // MUST
            cb_inn::chunkCreate_1_push_job( chunkKey, tmpChunkMPos ); //-- 正式创建，跨线程新方案
            cb_inn::wait_until_target_chunk_created(chunkKey);
                        // 此处禁止优化，必须逐个创建，逐个确认
        }
    }

    tprDebug::console( " nearby 3*3 chunks create Done! " );
}

    


/* 在游戏运行时，定期检查 玩家位置。及时生成 新的 chunk
 * 确保，玩家周边 9个chunk 始终存在
 * -------
 */
void collect_chunks_need_to_be_create_in_update(){

    const glm::dvec2 &playerDPos = esrc::get_player().get_goRef().get_dpos();

    cb_inn::currentChunkKey = anyDPos_2_chunkKey( playerDPos );
    if( cb_inn::is_first_check ){
        cb_inn::is_first_check = false;
        cb_inn::lastChunkKey = cb_inn::currentChunkKey;
        return;
    }

    if( cb_inn::currentChunkKey != cb_inn::lastChunkKey ){

        //-- 根据 新chunk 和 旧chunk 的位置关系，可以得知，player 的运动方向
        //   根据这个运动方向，调整 创建周边 chunk 的排序。
        //   从而让 运动方向前方的 chunk，更早被创建。
        NineDirection moveDir = cb_inn::calc_player_move_dir(  cb_inn::lastChunkKey,
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
                    cb_inn::chunkCreate_1_push_job( tmpChunkKey, tmpChunkMPos ); //-- 正式创建，跨线程新方案
                    break;

                case ChunkMemState::WaitForCreate:
                            tprDebug::console( "find chunk: WaitForCreate" );
                    // do nothing
                case ChunkMemState::OnCreating:
                case ChunkMemState::Active:
                    // do nothing
                    break;
                case ChunkMemState::WaitForRelease:
                case ChunkMemState::OnReleasing:
                    // 需要新建的 chunk 居然在 释放队列中(或正在被释放)，一定是参数设置出了问题 [严格版本]
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



/* 第三步：
 * （第二步 由 job线程 完成）
 * 每一渲染帧，检查 esrc::job_chunkFlags, 从中取出一个 已经计算好 job_chunk 的 chunk，
 * 然后生成 这个chunk 实例。
 * 每一帧仅限 1 个。
 */
std::optional<chunkKey_t> chunkCreate_3_receive_data_and_create_one_chunk(){

    //-- 从 已经制作好 job_chunk 的队列中，取出一个 chunk
    auto retOpt = esrc::atom_pop_from_job_chunkFlags();
    if( !retOpt.has_value() ){
        //-- 没有需要 生成的 chunk 时，直接退出 --
        return std::nullopt;
    }

    chunkKey_t chunkKey = retOpt.value();
    //-- 正式生成这个 chunk 实例
    cb_inn::create_one_chunk( chunkKey );
    //-- 及时删除 job_chunk 数据本体 --
    esrc::atom_erase_from_job_chunks( chunkKey ); //- MUST !!!  
    return { chunkKey };
}



/* 如今，ecoobj 也要在 job线程中被创建，这会导致，很多 chunk 无法被第一时间创建
 * 这些 chunk 要呆在 等待容器里，直到相关 ecoobj 都被创建完成后，才能开始被创建
 * 每帧都要调用此函数，以便在第一时间 创建 chunk 
 * 也许会有性能损失 ...
 */
void create_chunks_from_waitingQue(){

    const auto &chunkKeys_waitForCreate = esrc::get_chunkKeys_waitForCreate();
    if( chunkKeys_waitForCreate.empty() ){
        return;
    }
    //---
    std::set<chunkKey_t> chunkKeys {};
    bool isAllEcoObjActive {};
    for( const auto &key : chunkKeys_waitForCreate ){
        isAllEcoObjActive = cb_inn::is_chunk_near4EcoObjs_all_active( chunkKey_2_mpos(key), false );
        if( isAllEcoObjActive ){
            chunkKeys.insert( key );
        }
    }
    //---
    for( const auto &key : chunkKeys ){
        cb_inn::push_chunk_2_jobQue( key );
        esrc::move_chunkKey_from_waitForCreate_2_onCreating( key );
    }
}



namespace cb_inn {//----------- namespace: cb_inn ----------------//


// only be called by create_9_chunks()
void wait_until_target_chunk_created( chunkKey_t chunkKey_ ){

    while( true ){

        auto retOpt = chunkCreate_3_receive_data_and_create_one_chunk();
        if( !retOpt.has_value() ){
            //-- 没有需要 生成的 chunk 时，待机一会儿，再次 while 循环 --
            std::this_thread::sleep_for( std::chrono::milliseconds(5) );
            tprDebug::console( " waiting for chunkCreate " );
            continue;
        }else{
            if( retOpt.value() == chunkKey_ ){
                return;
            }
        }
    }
}




// job 线程完成工作后，在主线程中完成的 最后一步
void create_one_chunk( chunkKey_t chunkKey_ ){

            //   调用本函数，说明一定处于 “无视存储” 的早期阶段。

    IntVec2 chunkMPos = chunkKey_2_mpos( chunkKey_ );

    //------------------------------//
    //           [1]
    // 创建 周边 4个 ecoObj 实例
    //------------------------------//
    //-- 已被移到 chunkCreate_1_push_job() 中

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
    // 将 job_chunk 中的 fields，移动到 主容器
    //------------------------------//
    auto job_chunk_opt = esrc::atom_getnc_job_chunk_ptr( chunkKey_ );
    tprAssert( job_chunk_opt.has_value() );
    auto &job_chunkRef = *(job_chunk_opt.value());


    esrc::move_fieldUPtrs( job_chunkRef.get_fields() );


    //------------------------------//
    //            [5]
    // 遍历周边 8 个chunk，如果某一chunk 已经存在
    // 遍历此chunk 的 所有 edgeGoIds, 如果某个 edgeGo，其 collient 位于本chunk
    // 则将这些 collients 登记到 对应的 mapent 上
    // 针对 GameObj::signUp_newGO_to_chunk_and_mapEnt() 的一个补充
    //------------------------------//
    cb_inn::signUp_nearby_chunks_edgeGo_2_mapEnt( chunkKey_, chunkMPos );

    //------------------------------//
    //            [6]
    //  为 chunk 中的 8*8 个 field，分配 all kind of goes
    //------------------------------//

            double timeP_1 = esrc::get_timer().get_currentTime();


    IntVec2     fieldMPos   {};
    for( int h=0; h<FIELDS_PER_CHUNK<>; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK<>; w++ ){     
            fieldMPos = chunkRef.get_mpos() + IntVec2{ w*ENTS_PER_FIELD<>, h*ENTS_PER_FIELD<> };
            create_gos_in_field( fieldMPos_2_fieldKey(fieldMPos), chunkRef, job_chunkRef );
        }
    }


            double timeP_2 = esrc::get_timer().get_currentTime();

            tprDebug::console( 
                "chunkCreate: build_go_time: {}",
                (timeP_2-timeP_1)
            );

            // 应该把这部分数据，记录到 log 中
            // ...



    //------------------------------//
    //            [7]
    //  生成 刷怪笼（基于field）
    //          tmp...
    //------------------------------//
}


/* 三步：第一步：
 * 根据 目标chunk 制作成job，发送到 jobQue 
 */
void chunkCreate_1_push_job( chunkKey_t chunkKey_, IntVec2 chunkMPos_ ){

    // 只能惰性地，临时将需要的 ecoObjs 压入 job线程
    // 想要提高效率，还得在另一个地方，主动将更多 ecoobjs 压入 job线程

    // 检测 目标 chunk 依赖的 4个 ecoobj，是否已经 Active。    
    bool isAllEcoObjActive = is_chunk_near4EcoObjs_all_active( chunkMPos_, true );

    if( !isAllEcoObjActive ){
        // 如果周边4个 ecoobj 并未完全生成，chunk 将进入 等待队列
        esrc::insert_chunkKey_2_waitForCreate( chunkKey_ );

    }else{
        // push job
        push_chunk_2_jobQue( chunkKey_ );
        esrc::insert_chunkKey_2_onCreating( chunkKey_ );//  进入被 build流程的 chunk 需要被登记 防止被重复创建
    }    
}



/* 收集 chunk 周边4 ecoobjkeys，
 * 并检测它们，是否都已经被生成
 * param: isInnCall_: 本函数在两处地方被调用
 *    -- chunkCreate_1_push_job (inn)  
 *    -- create_chunks_from_waitingQue (out)
 */
bool is_chunk_near4EcoObjs_all_active(  IntVec2 chunkMPos_, bool isInnCall_ ){
    
    std::set<sectionKey_t> ecoObjKeys {};
    IntVec2 currentSectionMPos = anyMPos_2_sectionMPos( chunkMPos_ );
    for( const auto &jOff : nearby_4_sectionMPosOffs ){
        ecoObjKeys.insert( sectionMPos_2_sectionKey( currentSectionMPos + jOff ) );
    }
    tprAssert( ecoObjKeys.size() == 4 ); // tmp
    //---
    bool ret {true};
    for( const auto &key : ecoObjKeys ){
        auto state = esrc::get_ecoObjMemState(key);
        if( state != EcoObjMemState::Active ){
            ret = false;
            if( isInnCall_ ){
                break; // 当为外部态检测时，并不负责 ecoobj 的 创建
            }
        }

        switch( state ){
            case EcoObjMemState::OnCreating:
                // do nothing
                break;

            case EcoObjMemState::OnReleasing:
                tprAssert(0);
                break;

            case EcoObjMemState::NotExist:

                // 将 目标 ecoObj 压入 job 队列
                push_ecoObj_2_jobQue( key );
                esrc::insert_ecoObjKey_2_onCreating( key );

                break;
            
            default:
                break;
        }
    }
    return ret;
}




//-- 调用完本函数后，还应该处理 chunkMemState  
void push_chunk_2_jobQue( chunkKey_t chunkKey_ ){

    auto jobSPtr = std::make_shared<Job>();
    jobSPtr->set_jobType( JobType::Create_Job_Chunk );
    auto *paramPtr = jobSPtr->init_param<ArgBinary_Create_Job_Chunk>();
    paramPtr->chunkKey = chunkKey_;
    //----------
    esrc::atom_push_back_2_jobQue( jobSPtr );
}


NineDirection calc_player_move_dir( chunkKey_t oldKey_, chunkKey_t newKey_ ){

    IntVec2 offMPos = chunkKey_2_mpos(newKey_) - chunkKey_2_mpos(oldKey_);
    offMPos = offMPos.floorDiv( ENTS_PER_CHUNK_D );
    return intVec2_2_nineDirection( offMPos );
}



/* 主动访问 本chunk 的周边 8 个chunk 的 edgeGoIds，
 * 如果某个 edgeGo，与本chunk 有关系。
 * 就将这个 edgeGO 的 特定 colliEnts，登记到 本chunk 的 对应 mapent 上
 * ---
 * 这是一道 针对 signUp_newGO_to_chunk_and_mapEnt() 的补充工作
 */
void signUp_nearby_chunks_edgeGo_2_mapEnt( chunkKey_t chunkKey_, IntVec2 chunkMPos_ ){

    IntVec2    tmpChunkMPos {};
    chunkKey_t tmpChunkKey  {};
    for( const auto &iOff : nearby_8_chunkMPosOffs ){
        tmpChunkMPos = chunkMPos_ + iOff;
        tmpChunkKey = chunkMPos_2_chunkKey( tmpChunkMPos );

        if( ChunkMemState::Active == esrc::get_chunkMemState(tmpChunkKey) ){
            
            auto chunkPair = esrc::get_chunkPtr( tmpChunkKey );
            tprAssert( chunkPair.first == ChunkMemState::Active );
            auto &chunkRef = *(chunkPair.second);

            for( const auto &goid : chunkRef.get_edgeGoIds() ){//- each edgeGoId

                // get go ref
                auto goOpt = esrc::get_goPtr( goid );
                tprAssert( goOpt.has_value() );
                GameObj &goRef = *goOpt.value();

                if( goRef.find_in_chunkKeys(chunkKey_) ){// target go


                    ColliderType colliType = goRef.get_colliderType();
                    if( colliType == ColliderType::Circular ){
                        // only MajorGo
                        for( const auto &mpos : goRef.get_collisionRef().get_current_signINMapEnts_circle_ref() ){ 
                        
                            if( chunkKey_ == anyMPos_2_chunkKey(mpos) ){
                                //---- 正式注册 collient 到 mapents 上 -----
                                auto mapEntPair = esrc::getnc_memMapEntPtr(mpos);
                                tprAssert( mapEntPair.first == ChunkMemState::Active );
                                mapEntPair.second->insert_2_circular_goids( goid, goRef.get_colliderType() );
                            }
                        }

                    }else if( colliType == ColliderType::Square ){
                        // MajorGo / BioSoupGo

                        IntVec2 mpos {};
                        IntVec2 goRootMPos = dpos_2_mpos( goRef.get_dpos() );
                        for( const auto &mposOff : goRef.get_signInMapEnts_square_ref().get_all_mapEntOffs() ){
                            mpos = goRootMPos + mposOff;

                            if( chunkKey_ == anyMPos_2_chunkKey(mpos) ){
                                //---- 正式注册 collient 到 mapents 上 -----
                                auto mapEntPair = esrc::getnc_memMapEntPtr(mpos);
                                tprAssert( mapEntPair.first == ChunkMemState::Active );

                                if( goRef.family == GameObjFamily::Major ){
                                    mapEntPair.second->set_square_goid( goid, colliType );
                                }else if( goRef.family == GameObjFamily::BioSoup ){
                                    mapEntPair.second->set_bioSoup_goid( goid, goRef.family );
                                }else{
                                    tprAssert(0);
                                }
                            }
                        }
                    }else{
                        // do nothing...
                    }

                }// target go
            }//- each edgeGoId
        }
    }
}






}//-------------- namespace: cb_inn end ----------------//
}//----------------- namespace: chunkCreate: end -------------------//
