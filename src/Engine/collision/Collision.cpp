/*
 * ====================== Collision.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Collision.h"

//-------------------- Engine --------------------//
#include "GameObjMesh.h"
#include "GameObj.h"
#include "AnimActionPos.h"
#include "MapCoord.h"
#include "MapEnt.h"
#include "collide_oth.h"
#include "calc_colliPoints.h"

#include "esrc_chunk.h"
#include "esrc_gameObj.h"
#include "esrc_time.h"
#include "esrc_coordinate.h"


//===== static =====//
std::vector<glm::dvec2>                 Collision::obstructNormalVecs {};
std::set<NineDirection>                 Collision::confirmedAdjacentMapEnts {};
std::unordered_map<goid_t, glm::dvec2>  Collision::adjacentCirBeGos {};
std::unordered_set<goid_t>              Collision::begoids {}; 
std::unordered_set<goid_t>              Collision::begoids_circular {}; 
std::multiset<double>                   Collision::tVals {};  
std::vector<IntVec2>                    Collision::mapEnts_in_scanBody {};

// [*main-thread*]
void Collision::init_for_static()noexcept{
    init_for_colliOth_inn();
    //---
    Collision::obstructNormalVecs.reserve(100);
    Collision::adjacentCirBeGos.reserve(100);
    Collision::begoids.reserve(100);
    Collision::begoids_circular.reserve(100);
    Collision::mapEnts_in_scanBody.reserve(100);
    //...
}


/* ===========================================================
 *            detect_moveCollide
 * -----------------------------------------------------------
 * 对于 移动碰撞检测来说:
 * dogo 必须是 majorGo: Cir 类型
 * bego 可以为 Cir / Squ 2种类型之一
 * Arc 从不参与 moveCollide, 仅参与 skillCollide
 * Nil 从不参与 moveCollide
 * ---
 * 目前版本的 moveCollide 不存在任何 回调函数，草地不会因为 go 的踩过而摇动
 * 这是最干净的实现。任何 因为踩过而引发的 变化，统统由 skillCollide 来实现（目前也不推荐这么做...）
 */ 
glm::dvec2 Collision::detect_moveCollide( const glm::dvec2 &moveVec_ ){

    GameObj &dogoRef = this->goRef;
    tprAssert( dogoRef.family == GameObjFamily::Major );
    tprAssert( dogoRef.get_colliderType() == ColliderType::Circular );

    //-------------
    glm::dvec2 moveVec = limit_moveSpeed( moveVec_ );

    //----- dogo.isDoPass -----//
    //      isMoveCollide = false
    if( dogoRef.get_collisionRef().get_isDoPass() ||
        (!dogoRef.isMoveCollide) ){                    
        this->reSignUp_dogo_to_chunk_and_mapents( moveVec );
        return moveVec; 
    }

    //===================//
    //   moveCollide
    //===================//

    //----------------//
    //    第一阶段
    auto [isAllowedToMove1, newMoveVec1] = this->collect_AdjacentBegos_and_deflect_moveVec( moveVec_ );
    // dogo 在 起始阶段就被 阻挡了，完全无法移动
    if( !isAllowedToMove1 ){
        return glm::dvec2{ 0.0, 0.0 }; // 完全无法移动，无需重登记 chunk / mapents
    }
    moveVec = newMoveVec1;

    //----------------//
    //    第二阶段
    auto [isAllowedToMove2, newMoveVec2] = this->collect_IntersectBegos_and_truncate_moveVec( moveVec ); // MAJOR !!!! 
    //--- 如果确认 dogo 完全无法移动，直接退出 ---
    if( !isAllowedToMove2 ){
        return glm::dvec2( 0.0, 0.0 ); // 完全无法移动，无需重登记 chunk / mapents
    }
    moveVec = newMoveVec2;

    //=====//
    this->reSignUp_dogo_to_chunk_and_mapents( moveVec );
    return moveVec;
}




/* 检测 dogo 起始dpos，收集所有 相邻关系的，确认可以碰撞的 begos( squ/cir )
 * 计算出它们作用于 dogo 的墙壁法向量，收集到容器
 * 综合这些 墙壁法向量，确认本次是否可移动，如果可以，计算出 偏转后的 moveVec
 * ---
 * return:
 *     bool:   isAllowedToMove  
 *     dvec2:  if it's allowed to move, return new moveVec
 */
std::pair<bool, glm::dvec2> Collision::collect_AdjacentBegos_and_deflect_moveVec( const glm::dvec2 &moveVec_ ){

    GameObj     &dogoRef = this->goRef;
    const auto &dogoDPos = dogoRef.get_dpos();
    IntVec2     dogoMPos = dpos_2_mpos(dogoDPos);
    IntVec2     mpos {};

    //---
    Collision::confirmedAdjacentMapEnts.clear();
    Collision::adjacentCirBeGos.clear();
    Collision::obstructNormalVecs.clear();

    //===================//
    //   square begos
    //===================//
    // 所有 squ begos，需要被当作一个整体来看待
    for( const auto &dir : collect_Adjacent_nearbyMapEnts( dogoDPos, dogoMPos ) ){ // all Adjacent mapents

        mpos = dogoMPos + nineDirection_2_mposOff(dir); 

        //- 当发现某个 ent 处于非 Active 的 chunk。
        //  直接跳过，最简单的处理手段
        auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
        if( mapEntPair.first != ChunkMemState::Active ){
            continue; //- skip
        }

        //----- empty bego -----//
        goid_t squ_goid = mapEntPair.second->get_square_goid();
        if( squ_goid == 0 ){
            continue;
        }

        auto begoOpt = esrc::get_goPtr( squ_goid );
        tprAssert( begoOpt.has_value() );
        GameObj &begoRef = *begoOpt.value();

        //----- isMoveCollide -----//
        //      bego.isBePass
        //      goAltiRange
        if( (!begoRef.isMoveCollide) || 
            begoRef.get_collisionRef().get_isBePass(dogoRef.speciesId) ||
            (!is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange())) ){
            continue;
        }

        //----- collect Adjacent mapents -----//
        auto [insertIt, insertBool] = Collision::confirmedAdjacentMapEnts.insert( dir );
        tprAssert( insertBool );
    }

    //---
    if( !Collision::confirmedAdjacentMapEnts.empty() ){
        glm::dvec2 obstructNormalVec = calc_obstructNormalVec_from_AdjacentMapEnts(
                                            moveVec_,
                                            dogoDPos, 
                                            dogoMPos,
                                            Collision::confirmedAdjacentMapEnts );

        //-- 过滤掉 背向而行的 可能性 --
        if( !is_dogo_leave_begoSquares_easy(moveVec_, obstructNormalVec) ){
            Collision::obstructNormalVecs.push_back( obstructNormalVec );
        }
    }

    //===================//
    //   circular begos
    //===================//
    //  初步收集 所有有效 begoid
    //  不包含 dogo 自己
    Collision::begoids_circular.clear();
    for( const auto &colliPointDPosOff : get_colliPointDPosOffsRef_for_cirDogo() ){
        mpos = dpos_2_mpos( dogoDPos + colliPointDPosOff );

        //- 当发现某个 ent 处于非 Active 的 chunk。
        //  直接跳过，最简单的处理手段
        auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
        if( mapEntPair.first != ChunkMemState::Active ){
            continue; //- skip
        }
        
        for( const auto &begoid : mapEntPair.second->get_circular_goids() ){ //- each cir-bego
            if( begoid == dogoRef.goid ){continue;}//-- skip self --
            Collision::begoids_circular.insert( begoid );
        }
    }

    //--------
    for( const auto &begoid : Collision::begoids_circular ){ //- each cir-bego
        auto begoOpt = esrc::get_goPtr( begoid );
        tprAssert( begoOpt.has_value() );
        GameObj &begoRef = *begoOpt.value();

        //----- isMoveCollide -----//
        //      bego.isBePass
        //      goAltiRange
        if( (!begoRef.isMoveCollide) || 
            begoRef.get_collisionRef().get_isBePass(dogoRef.speciesId) ||
            (!is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange())) ){
            continue;
        }

        //-- calc and collect Adjacent begos --
        Circular begoCir = begoRef.calc_circular( CollideFamily::Move );
        auto colliState = collideState_from_circular_2_circular( dogoDPos, begoCir, 1.0);
        if( colliState == CollideState::Adjacent ){
            Collision::adjacentCirBeGos.insert({begoid, dogoDPos - begoRef.get_dpos() });
        }
    }

    //--------
    if( !Collision::adjacentCirBeGos.empty() ){

        bool is_leftSide_have {false};
        bool is_rightSide_have {false};
        //--
        for( const auto &[iGoid, iVec] : Collision::adjacentCirBeGos ){

            // get go ref
            auto begoOpt = esrc::get_goPtr( iGoid );
            tprAssert( begoOpt.has_value() );
            GameObj &begoRef = *begoOpt.value();
            
            //----- 过滤掉那些 背向而行 的 bego -----
            if( is_dogoCircular_leave_begoCircular(moveVec_, dogoDPos, begoRef.calc_circular(CollideFamily::Move) ) ){
                continue;
            }
                    
            glm::dvec2 innVec = calc_innVec( moveVec_, iVec );
            //-- 几乎和 位移 同方向
            if( is_closeEnough<double>(innVec.y, 0.0, 0.01) ){
                return { false, glm::dvec2{} }; //- 正面遭遇阻挡，完全无法位移. 
            }
            //--
            (innVec.y > 0.0) ?  is_leftSide_have = true :
                                is_rightSide_have = true;
            //--
            if( is_leftSide_have && is_rightSide_have ){
                return { false, glm::dvec2{} }; //- 左右都遭遇阻挡，完全无法位移. 
            }
            //--
            Collision::obstructNormalVecs.push_back( iVec );
        }
    }

    //===================//
    //   calc deflectmoveVec
    //===================//
    if( Collision::obstructNormalVecs.empty() ){
        return { true, moveVec_ };  //- 也许存在相邻go，但完全不影响位移
    }

    glm::dvec2 sum {};
    for( const auto &i : Collision::obstructNormalVecs ){
        sum += i;
    }

    //---- 现在，sum指向 总阻力向量 的反方向 ----
    glm::dvec2 newMoveVec = calc_slideMoveVec(moveVec_, sum );
    return { true, newMoveVec }; // 修正了方向后的 新位移
}



/* return:
 *     bool:   isAllowedToMove  
 *     dvec2:  if it's allowed to move, return new moveVec 修正后的位移向量（ 原值／t／偏向 ）
 */  
std::pair<bool,glm::dvec2> Collision::collect_IntersectBegos_and_truncate_moveVec( const glm::dvec2 &moveVec_ ){

    auto        &dogoRef = this->goRef;
    const auto  &dogoDPos = dogoRef.get_dpos();
    IntVec2     dogoMPos = dpos_2_mpos(dogoDPos);
    glm::dvec2  dogeTargetDPos = dogoDPos + moveVec_;
    IntVec2     mpos {};

    Collision::tVals.clear();

    //===================//
    //   square begos
    //===================//
    Collision::build_a_scanBody( moveVec_, dogoDPos );
    for( const auto &impos : Collision::mapEnts_in_scanBody ){    

        //- 当发现某个 ent 处于非 Active 的 chunk。
        //  直接跳过，最简单的处理手段
        auto mapEntPair = esrc::getnc_memMapEntPtr( impos );
        if( mapEntPair.first != ChunkMemState::Active ){
            continue; //- skip
        }

        goid_t squ_goid = mapEntPair.second->get_square_goid();
        if( squ_goid == 0 ){
            continue; //- skip
        }

        // get go ref
        auto begoOpt = esrc::get_goPtr( squ_goid );
        tprAssert( begoOpt.has_value() );
        GameObj &begoRef = *begoOpt.value();

        //----- isMoveCollide -----//
        //      bego.isBePass
        //      goAltiRange
        if( (!begoRef.isMoveCollide) || 
            begoRef.get_collisionRef().get_isBePass(dogoRef.speciesId) ||
            (!is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange())) ){
            continue; //- skip
        }

        //-- 剔除 背向而行 --
        if( is_dogo_leave_begoSquares_2( moveVec_, dogoDPos, dogoMPos, impos ) ){ // 严谨版
            continue; //- skip
        }

        //-- 现在确认 bego 为 有效碰撞bego:        
        //   逐个检测每个 bego，计算并收集 t 值，
        auto [isHaveT, t] = cast_with_mapent( moveVec_, dogoDPos, impos );
        if( isHaveT ){
            Collision::tVals.insert( t );
        }
    }

    //===================//
    //   circular begos
    //===================//
    //  初步收集 所有有效 begoid
    //  不包含 dogo 自己，不包含 adjacentCirBeGos 中的 bego
    Collision::begoids.clear();

    for( const auto &colliPointDPosOff : get_colliPointDPosOffsRef_for_cirDogo() ){
        mpos = dpos_2_mpos( dogeTargetDPos + colliPointDPosOff );

        //- 当发现某个 addEnt 处于非 Active 的 chunk。
        //  直接判定此次碰撞 为 阻塞，最简单的处理手段
        auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
        if( mapEntPair.first != ChunkMemState::Active ){
            return { false, glm::dvec2{0.0, 0.0} }; //- IMM!!!
        }

        for( const auto &begoid : mapEntPair.second->get_circular_goids() ){//- each bego
            if( begoid == dogoRef.goid ){continue;}//-- skip self --
            if( Collision::adjacentCirBeGos.find(begoid) != Collision::adjacentCirBeGos.end() ){ continue; }//-- skip old adjacent bego --
            Collision::begoids.insert( begoid );
        }
    }

    //----------------------------------------//
    //   check each bego，and collect tbegoids
    Circular begoCir {};
    for( const auto &begoid : Collision::begoids ){//- each bego (cirs)

        // get go ref
        auto begoOpt = esrc::get_goPtr( begoid );
        tprAssert( begoOpt.has_value() );
        GameObj &begoRef = *begoOpt.value();

        //----- isMoveCollide -----//
        //      bego.isBePass
        //      goAltiRange
        if( (!begoRef.isMoveCollide) || 
            begoRef.get_collisionRef().get_isBePass(dogoRef.speciesId) ||
            (!is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange())) ){
            continue;
        }

        //-- 做二级碰撞检测，如果 targetPoint 确认发生碰撞，再计算出碰撞值 t --
        begoCir = begoRef.calc_circular( CollideFamily::Move );

        //-- 剔除 背向而行 --
        if( is_dogoCircular_leave_begoCircular( moveVec_, dogoDPos, begoCir ) ){
            continue;
        }

        auto colliState = collideState_from_circular_2_circular( dogeTargetDPos, begoCir, 0.1 );
        if( colliState ==  CollideState::Intersect ){
            double t = circularCast( moveVec_, dogoDPos, begoCir ); //- 进一步计算出 t值， 并将 t值 存入 容器...
            Collision::tVals.insert( t );
        }
    }//- each bego


    //------ 没有 tVals 就直接 return -------//
    if( Collision::tVals.empty() ){
        return { true, moveVec_ };//- 没有发生碰撞
    }

    //----------------------------------------//
    //-- 找出最小的 t值 （允许重复）
    double tMin = *Collision::tVals.begin(); //- min

    if( is_closeEnough<double>( tMin, 0.0, 0.0001 ) ){
        return  { false, glm::dvec2{ 0.0, 0.0 }  };
    }else{
        return  { true, moveVec_*tMin }; //- 用 t值 修正 位移向量
    }
}




/* ===========================================================
 *         reSignUp_dogo_to_chunk_and_mapents
 * -----------------------------------------------------------
 */  
void Collision::reSignUp_dogo_to_chunk_and_mapents( const glm::dvec2 &moveVec_ )noexcept{

    GameObj &dogoRef = this->goRef;
    SignInMapEnts_Circle &signInMapEntsRef = *this->signInMapEnts_cir_uptr;

    //--------------------------------//
    //  更新 dogo 在 mapents 中的登记信息
    //--------------------------------//
    bool isSignINMapEntsChanged = signInMapEntsRef.forecast_signINMapEnts( dogoRef.get_dpos() + moveVec_ );
            // 针对 dogo 完整半径 碰撞区的 forecast 操作

    //-- update adds/dels --
    if( isSignINMapEntsChanged == true ){

            signInMapEntsRef.sync_currentSignINMapEnts_from_future();
            //-- adds --
            for( const auto &mpos : signInMapEntsRef.get_currentAddsRef() ){
                auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
                //-- 有时，目标 mapent 所在 chunk，尚未 active 了，暂时直接忽略
                if( mapEntPair.first == ChunkMemState::Active ){
                    mapEntPair.second->insert_2_circular_goids( dogoRef.goid, dogoRef.get_colliderType() );
                }else{
                    //-- debug --
                    tprDebug::console( "++++ Collision::detect_for_move: catch not Active Chunk in adds!!!" );
                }
            }
            //-- dels --
            for( const auto &mpos : signInMapEntsRef.get_currentDelsRef() ){
                auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );  
                //-- 有时，目标 mapent 所在 chunk，已经 not exist 了，那种的直接忽略
                if( mapEntPair.first == ChunkMemState::Active ){
                    mapEntPair.second->erase_from_circular_goids( dogoRef.goid, dogoRef.get_colliderType() );
                        //-- 执行正式的注销操作，并确保原初 存在唯一的 目标元素
                }else{
                    //-- debug --
                    tprDebug::console( "++++ Collision::detect_for_move: catch not Active Chunk in dels!!!" );
                }
                    
            }
    }

    //--------------------------------//
    //  更新 dogo 在 chunk 中的登记信息
    //--------------------------------//
    //-- 检查本go 的 新chunk，如果发生变化，释放旧chunk 中的 goids, edgeGoIds
    //     登记到 新chunk 的 goids
    chunkKey_t newChunkKey = anyDPos_2_chunkKey( dogoRef.get_dpos() + moveVec_ );
    auto chunkPair1 = esrc::get_chunkPtr( newChunkKey );
    tprAssert( chunkPair1.first == ChunkMemState::Active );
    Chunk &newChunkRef = *(chunkPair1.second);

    if( newChunkKey != dogoRef.currentChunkKey ){
        auto chunkPair2 = esrc::get_chunkPtr( dogoRef.currentChunkKey );
        tprAssert( chunkPair2.first == ChunkMemState::Active );
        Chunk &oldChunkRef = *(chunkPair2.second);

        size_t eraseNum = oldChunkRef.erase_from_goIds( dogoRef.goid );
        tprAssert( eraseNum == 1 );
        oldChunkRef.erase_from_edgeGoIds( dogoRef.goid ); // maybe 
        //---
        dogoRef.currentChunkKey = newChunkKey;
        newChunkRef.insert_2_goIds( dogoRef.goid );
    }

    //---------------------------//
    //-- 重新统计 本go 的 chunkKeys，如果确认为 临界go，  
    //     登记到 主chunk 的 edgegoids 容器中
    size_t chunkKeysSize = dogoRef.reCollect_chunkKeys();
    if( chunkKeysSize == 1 ){
        newChunkRef.erase_from_edgeGoIds( dogoRef.goid ); // maybe
    }else if( chunkKeysSize > 1 ){
        newChunkRef.insert_2_edgeGoIds( dogoRef.goid );
    }else{
        tprAssert(0);
    }
}



/* ===========================================================
 *                 build_a_scanBody   [static]
 * -----------------------------------------------------------
 * 收集 扫掠体 内的所有 mapents
 */  
void Collision::build_a_scanBody(   const glm::dvec2 &moveVec_,
                                    const glm::dvec2 &dogoDPos_ ){
    
    glm::dvec2 dogoTargetDPos = dogoDPos_ + moveVec_;

    //-------------------------//
    //  计算 扫掠体 4条边
    //-------------------------//
    double leftLine {};
    double rightLine {};
    double topLine {};
    double bottomLine {};
    if( moveVec_.x >= 0.0 ){
        leftLine = dogoDPos_.x - Circular::radius_for_dogo;
        rightLine = dogoTargetDPos.x + Circular::radius_for_dogo;
    }else{
        leftLine = dogoTargetDPos.x - Circular::radius_for_dogo;
        rightLine = dogoDPos_.x + Circular::radius_for_dogo;
    }
    if( moveVec_.y >= 0.0 ){
        topLine = dogoTargetDPos.y + Circular::radius_for_dogo;
        bottomLine = dogoDPos_.y - Circular::radius_for_dogo;
    }else{
        topLine = dogoDPos_.y + Circular::radius_for_dogo;
        bottomLine = dogoTargetDPos.y - Circular::radius_for_dogo;
    }

    //-------------------------//
    //  如果某条边，临近 mapent 边界，主动收缩此边的值
    //  以此来 减少 扫掠体内包含的 mapents 个数
    //  与扫掠体 呈相邻关系 的 mp， 是不需要做检测的
    //  --
    //  目前可以把 扫掠体收集的 mapents 数量控制在 1～4 个（高峰为6个）
    //-------------------------//
    constexpr double mpSideLen = PIXES_PER_MAPENT_D;
    constexpr double threshold = 0.5; // Adjacent

    double l = floor(leftLine / mpSideLen) * mpSideLen;
    double r = floor(rightLine / mpSideLen) * mpSideLen;
    double t = floor(topLine / mpSideLen) * mpSideLen;
    double b = floor(bottomLine / mpSideLen) * mpSideLen;

    if( is_closeEnough<double>( l + mpSideLen, leftLine, threshold ) ){
        leftLine = l + mpSideLen + threshold;
    }
    if( is_closeEnough<double>( r, rightLine, threshold ) ){
        rightLine = r - threshold;
    }
    if( is_closeEnough<double>( b + mpSideLen, bottomLine, threshold ) ){
        bottomLine = b + mpSideLen + threshold;
    }
    if( is_closeEnough<double>( t, topLine, threshold ) ){
        topLine = t - threshold;
    }   

    //-------------------------//
    //   正式收集 mapents
    //-------------------------//
    IntVec2 mp_leftBottom = dpos_2_mpos( glm::dvec2{ leftLine, bottomLine } );
    IntVec2 mp_rightTop   = dpos_2_mpos( glm::dvec2{ rightLine, topLine } );

    Collision::mapEnts_in_scanBody.clear();
    for( int j=mp_leftBottom.y; j<=mp_rightTop.y; j++ ){
        for( int i=mp_leftBottom.x; i<=mp_rightTop.x; i++ ){
            //Collision::mapEnts_in_scanBody.push_back( IntVec2{i,j} );
            Collision::mapEnts_in_scanBody.emplace_back( i, j );
        }
    }
}



