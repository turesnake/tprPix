/*
 * ============= signUp_newGO_to_chunk_and_mapEnt.cpp ===============
 *                          -- tpr --
 *                                        CREATE -- 2020.02.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "create_goes.h"


//-------------------- Engine --------------------//
#include "esrc_gameObj.h"
#include "esrc_chunk.h"




namespace gameObjs{//------------- namespace gameObjs ----------------


/* 将 新建go 的 colliEnts 登记到所有对应的 mapent 上去。
 * 难点：
 *    有些身处 chunk边缘的 “临界go” 的 collient，可以位于 隔壁chunk
 *    而这个 隔壁 chunk，可能尚未创建。（此处会引发各种麻烦）
 *    目前解决方案：
 *      -1- 新建go 跳过这个 colliEnt 的登记工作
 *      -2- 统计自己的 chunkeys,
 *      -3- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
 *      -4- 将 本goid，记录到 主chunk goids 容器中
 * 
 * -- 目前仅用于 create_a_Go() 系列函数
 */
void signUp_newGO_to_chunk_and_mapEnt( GameObj &goRef_ ){

    //-------------------------------//
    // 唯独 UI go，既不参与 chunk 登记，也不参与 mapent 登记 
    if( goRef_.family == GameObjFamily::UI ){
        return;
    }

    //------------------------------//
    //           --1--
    // --- 记录 go.currentChunkKey
    // --- 统计自己的 chunkeys
    // --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
    // --- 将 本goid，记录到 主chunk goids 容器中
    //------------------------------//
    auto [chunkMemState1, chunkPtr1] = esrc::get_chunkPtr( goRef_.currentChunkKey );
    tprAssert( chunkMemState1 == ChunkMemState::Active );
    Chunk &currentChunkRef = *(chunkPtr1);

    currentChunkRef.insert_2_goIds( goRef_.goid ); //- always


    // Floor/GroundGo/WorldUI 仅参与 chunk 登记。 不登记 edgeGoids, mapent
    if( (goRef_.family!=GameObjFamily::Major) &&
        (goRef_.family!=GameObjFamily::BioSoup) ){
        return;
    }

    //------------------------------//
    //            --2--
    //  收集 chunkKeys, 确定是否为 edgeGo
    //------------------------------//
    size_t chunkKeySize = goRef_.reCollect_chunkKeys();
    if( chunkKeySize > 1 ){
        currentChunkRef.insert_2_edgeGoIds( goRef_.goid );
    }

    //------------------------------//
    //            --3--
    //      signUp to mapEnt
    //------------------------------//
    ColliderType colliType = goRef_.get_colliderType();
    if( colliType == ColliderType::Circular ){
        // only MajorGo

        const auto &currentSignINMapEntsRef = goRef_.get_collisionRef().get_current_signINMapEnts_circle_ref();
        tprAssert( !currentSignINMapEntsRef.empty() ); //- tmp
        for( const auto &mpos : currentSignINMapEntsRef ){

            //-- 如果 colliEnt所在 chunk 尚未创建，表明此 go 为 “临界go”。
            // 此时显然不能去调用 esrc::getnc_memMapEntPtr(), 会出错。
            // 将会暂时 忽略掉这个 collient 的登记工作，
            // 这个工作，会等到 目标chunk 创建阶段，再补上: 
            // 在 signUp_nearby_chunks_edgeGo_2_mapEnt() 中
            auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey(mpos) );
            if( (chunkState==ChunkMemState::NotExist) || 
                (chunkState==ChunkMemState::WaitForCreate) ||
                (chunkState==ChunkMemState::OnCreating) ){
                continue;
            }

            //---- 正式注册 go 到 mapents 上 -----
            auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
            tprAssert( mapEntPair.first == ChunkMemState::Active );
            mapEntPair.second->insert_2_circular_goids( goRef_.goid, colliType );
        }


    }else if( colliType == ColliderType::Square ){
        // MajorGo / BioSoupGo

        IntVec2 mpos {};
        IntVec2 goRootMPos = dpos_2_mpos( goRef_.get_dpos() );
        for( const auto &mposOff : goRef_.get_signInMapEnts_square_ref().get_all_mapEntOffs() ){
            mpos = goRootMPos + mposOff;

            //-- 如果 colliEnt所在 chunk 尚未创建，表明此 go 为 “临界go”。
            // 此时显然不能去调用 esrc::getnc_memMapEntPtr(), 会出错。
            // 将会暂时 忽略掉这个 collient 的登记工作，
            // 这个工作，会等到 目标chunk 创建阶段，再补上: 
            // 在 signUp_nearby_chunks_edgeGo_2_mapEnt() 中
            auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey(mpos) );
            if( (chunkState==ChunkMemState::NotExist) || 
                (chunkState==ChunkMemState::WaitForCreate) ||
                (chunkState==ChunkMemState::OnCreating) ){
                continue;
            }

            //---- 正式注册 go 到 mapents 上 -----
            auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
            tprAssert( mapEntPair.first == ChunkMemState::Active );

            if( goRef_.family == GameObjFamily::Major ){
                mapEntPair.second->set_square_goid( goRef_.goid, colliType );
            }else{ // BioSoup
                mapEntPair.second->set_bioSoup_goid( goRef_.goid, goRef_.family );
            }
        }

    }else{
        // do nothing !
    }

}



}//------------- namespace gameObjs: end ----------------

