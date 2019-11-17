/*
 * ====================== Collision.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Collision.h"

//-------------------- CPP --------------------//
#include <unordered_set>
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjMesh.h"
#include "GameObj.h"
#include "AnimActionPos.h"
#include "MapCoord.h"
#include "MapEnt.h"
#include "collide_oth.h"

#include "esrc_chunk.h"
#include "esrc_gameObj.h"

#include "esrc_time.h"
#include "esrc_customCoord.h"

#include "tprDebug.h"



/* ===========================================================
 *               collect_adjacentBeGos
 * -----------------------------------------------------------
 *  牺牲算力，在每一帧移动检测前，先检测一遍当前所在的 signInMapEnts
 *  临时计算出，当前的 相邻bego 集，存入 adjacentBeGos
 *  (而不是使用之前的 登记制)
 */ 
void Collision::collect_adjacentBeGos(){

    // 调用本函数，意味着 goRef.isMoveCollide 一定为 true !!!
    //---------------------------------------//
    GameObj &dogoRef = this->goRef; //- 碰撞检测 主动发起方

        //-- dogo Must be Circular !!! --
        tprAssert( dogoRef.get_colliderType() == ColliderType::Circular );

    Circular dogoCir = dogoRef.calc_circular( CollideFamily::Move );


    this->adjacentBeGos.clear();
    this->begoids.clear();
    this->begoids_withOut_artifact.clear();
    this->artifactBegoids.clear();

    //----------------------------------------//
    //      初步收集 所有有效 begoid
    //  不包含 dogo 自己，不包含 旧的 adjacentBeGos 中的 bego

    const auto &artifactCoordRef = esrc::get_artifactCoordRef();

    std::string funcName = "collect_adjacentBeGos()";
    for( const auto &iMPos : this->signInMapEntsUPtr->get_currentSignINMapEntsRef() ){
        //- 当发现某个 addEnt 处于非 Active 的 chunk。
        //  直接跳过，最简单的处理手段
        auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey( iMPos ) );
        if( chunkState != ChunkMemState::Active ){
            continue; //- skip
        }

        auto mapEntPair = esrc::getnc_memMapEntPtr( iMPos );
        tprAssert( mapEntPair.first == ChunkMemState::Active );
        // 目前，只有开启了 isMoveCollide 的，才会被收集到
        // 未来会被修改 ...
        for( const auto &begoid : mapEntPair.second->get_majorGos() ){//- each bego
            if( begoid == dogoRef.id ){continue;}//-- skip self --


            this->begoids.insert( begoid );

            GameObj &begoRef = esrc::get_goRef( begoid );
            if( begoRef.get_colliderType() == ColliderType::Square ){

                glm::dvec2 innDPos = artifactCoordRef.calc_innDPos( begoRef.get_dpos() );
                                        //-- 在未来，建筑单位要永久存储自己的 innmpos 和 key
                                        //   ...

                this->artifactBegoids.insert({ mpos_2_key( dpos_2_mpos(innDPos) ), begoid });
            }else{
                this->begoids_withOut_artifact.insert( begoid );
            }

                    //-- 折中方案, 同时存储进 3个 容器中
                    //...
        }
    }

    //----------------------------------------//
    // 优先处理所有 人造物begos

    bool isFindAdjacentArtifact {false};

    for( const auto &iPair : this->artifactBegoids ){//- each artifact-bego
        GameObj &begoRef = esrc::get_goRef( iPair.second );
        tprAssert( begoRef.get_colliderType() == ColliderType::Square );

        glm::dvec2 dogo_2_bego = begoRef.get_dpos() - dogoRef.get_dpos();

        //-- goAltiRange --//
        //  过滤掉，在 altiRange 不会碰撞的 bego --
        if( !is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange()) ){
            continue;
        }

        //-- 仅仅计算 dogo,bego 距离，看是否相邻 -- 
        Square begoSqu = begoRef.calc_square();
        if( collideState_from_circular_2_square_simple( dogoCir, begoSqu, 0.2) == CollideState::Adjacent ){
            isFindAdjacentArtifact = true;
            break;
        }
    }

    //- 一旦确认 dogo 与某个 人造物单位相邻，则将所有人造物单位看作一个整体
    //  统一计算出，这个整体，作用于 dogo 的墙壁法向量
    if( isFindAdjacentArtifact ){





    }



    //----------------------------------------//
    for( const auto &begoid : this->begoids_withOut_artifact ){//- each bego without artifacts
        GameObj &begoRef = esrc::get_goRef( begoid );

        glm::dvec2 dogo_2_bego = begoRef.get_dpos() - dogoRef.get_dpos();

        //-- goAltiRange --//
        //  过滤掉，在 altiRange 不会碰撞的 bego --
        if( !is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange()) ){
            continue;
        }

        //-- 仅仅计算 dogo,bego 距离，看是否相邻 --
        tprAssert( begoRef.get_colliderType() == ColliderType::Circular );

        Circular begoCir = begoRef.calc_circular( CollideFamily::Move );

        if( collideState_from_circular_2_circular( dogoCir, begoCir, 1.0) == CollideState::Adjacent ){
            this->adjacentBeGos.insert({begoid, dogo_2_bego});
        }
    }//- each bego

}

/* ===========================================================
 *               detect_adjacentBeGos
 * -----------------------------------------------------------
 * 精简版，只需要计算 偏转后的 位移向量
 */
glm::dvec2 Collision::detect_adjacentBeGos( const glm::dvec2 &moveVec_ ){

    if( this->adjacentBeGos.empty() ){
        return  moveVec_;
    }

    GameObj &dogoRef = this->goRef; //- 碰撞检测 主动发起方
    Circular dogoCir = dogoRef.calc_circular( CollideFamily::Move );
    //--------------------------------//
    bool is_leftSide_have {false};
    bool is_rightSide_have {false};
    std::vector<glm::dvec2> self_2_oths {};
    //--
    for( const auto &pair : this->adjacentBeGos ){
        GameObj &begoRef = esrc::get_goRef( pair.first );
        const auto begoColliderType = begoRef.get_colliderType();

        //----- 过滤掉那些 背向而行 的 bego -----
        if( begoColliderType == ColliderType::Circular ){
            if( is_dogoCircular_leave_begoCircular(moveVec_, dogoCir, begoRef.calc_circular(CollideFamily::Move) ) ){
                continue;
            }
        }else if( begoColliderType == ColliderType::Square ){
            if( is_dogoCircular_leave_begoSquare(moveVec_, dogoCir, begoRef.calc_square()) ){
                continue;
            }
        }else{
            tprAssert(0);
        }
                
        glm::dvec2 innVec = calc_innVec( moveVec_, pair.second );

        //-- 几乎和 位移 同方向
        if( is_closeEnough<double>(innVec.y, 0.0, 0.01) ){
            return glm::dvec2{0.0, 0.0}; //- 正面遭遇阻挡，位移彻底被阻挡. 
        }
        //--
        (innVec.y > 0.0) ?  is_leftSide_have = true :
                            is_rightSide_have = true;
        //--
        if( is_leftSide_have && is_rightSide_have ){
            return  glm::dvec2{0.0, 0.0}; //- 左右遭遇阻挡，位移彻底被阻挡. 
        }
        //--
        self_2_oths.push_back( pair.second );
    }

    //--------------------------------//
    if( self_2_oths.empty() ){
        return moveVec_;  //- 存在相邻go，但完全不影响位移
    }
    //------ 计算正式的 位移向量 --------//
    glm::dvec2 sum {};
    for( const auto &i : self_2_oths ){
        sum += i;
    }
    //---- 现在，sum指向 总阻力向量 的反方向 ----
    return calc_slideMoveVec( moveVec_, sum );
}



/* ===========================================================
 *            detect_for_move  [Sec]
 * -----------------------------------------------------------
 */ 
glm::dvec2 Collision::detect_for_move( const glm::dvec2 &moveVec_ ){

    {//-- 简陋的检测，位移距离 不大于 1mapent --
    //-- Avoid Radical Sign / 避免开根号 --
        double moveLen =  moveVec_.x*moveVec_.x + moveVec_.y*moveVec_.y;
        tprAssert( moveLen < static_cast<double>(PIXES_PER_MAPENT*PIXES_PER_MAPENT) );
    }


    //-- 不支持 移动碰撞检测的 regularGo, 仅更新 chunkkey 数据，然后就原样退出 --
    if( !this->goRef.isMoveCollide ){
        this->handle_chunkKeys();
        return moveVec_;
    }

    //-----------------------------//
    bool isObstruct             {false}; //- 碰撞检测返回值，是否检测到 "无法穿过"的碰撞
    bool isSignINMapEntsChanged {false};        

    this->collect_adjacentBeGos();
    glm::dvec2 moveVec = this->detect_adjacentBeGos( moveVec_ );

    if( (moveVec.x==0.0) && (moveVec.y==0.0) ){
        return moveVec;
    }

    isSignINMapEntsChanged = this->signInMapEntsUPtr->forecast_signINMapEnts( this->goRef.get_dpos() + moveVec );
    //-- 不管 forecast 的结果是否为 true，都要做 碰撞检测 --
    auto out = this->for_move_inn( moveVec ); // MAJOR !!!! 

    isObstruct = out.first;
    if( isObstruct ){
        moveVec = out.second;
    }

    //  还需要再次调用 forecast 函数
    isSignINMapEntsChanged = this->signInMapEntsUPtr->forecast_signINMapEnts( this->goRef.get_dpos() + moveVec );


    if( isObstruct == false ){

        //-- update adds/dels --
        if( isSignINMapEntsChanged == true ){

            this->signInMapEntsUPtr->sync_currentSignINMapEnts_from_future();
            //-- adds --
            for( const auto &mpos : this->signInMapEntsUPtr->get_currentAddsRef() ){
                auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
                tprAssert( mapEntPair.first == ChunkMemState::Active );
                mapEntPair.second->insert_2_majorGos( this->goRef.id );
            }
            //-- dels --
            for( const auto &mpos : this->signInMapEntsUPtr->get_currentDelsRef() ){
                auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );  

                //-- 有时，目标 mapent 所在 chunk，已经 not exist 了 
                if( mapEntPair.first == ChunkMemState::Active ){
                    mapEntPair.second->erase_the_onlyOne_from_majorGos( this->goRef.id );
                        //-- 执行正式的注销操作，并确保原初 存在唯一的 目标元素
                }else{
                    //-- debug --
                    cout << "++++ Collision::detect_for_move: catch not Active Chunk!!!" << endl;
                }
                    
            }
        }
    
        //---------------------------//
        this->handle_chunkKeys(); //-- 更新 dogo 在 chunk 中的登记信息
    }

    return moveVec; //- 修正后的 位移向量
}



/* ===========================================================
 *            for_move_inn  [Sec]
 * -----------------------------------------------------------
 * -- return:
 *    bool       -- 位移是否遭遇 阻挡  
 *    glm::dvec2 -- 修正后的位移向量（ 原值／t／偏向 ）
 */  
std::pair<bool,glm::dvec2> Collision::for_move_inn( const glm::dvec2 &moveVec_ ){

        //- for debug
        double currentTime = esrc::get_timer().get_currentTime();

    // 调用本函数，意味着 goRef.isMoveCollide 一定为 true !!!
    //---------------------------------------//
    GameObj     &dogoRef = this->goRef; //- 碰撞检测 主动发起方
    glm::dvec2   dogeTargetDPos = dogoRef.get_dpos() + moveVec_;


        //-- dogo Must be Circular !!! --
        tprAssert( dogoRef.get_colliderType() == ColliderType::Circular );


    //----------------------------------------//
    //      初步收集 所有有效 begoid
    //  不包含 dogo 自己，不包含 adjacentBeGos 中的 bego
    this->begoids.clear();
    //--
    for( const auto &iMPos : this->signInMapEntsUPtr->get_futureSignINMapEntsRef() ){

        //- 当发现某个 addEnt 处于非 Active 的 chunk。
        //  直接判定此次碰撞 为 阻塞，最简单的处理手段
        auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey( iMPos ) );
        if( chunkState != ChunkMemState::Active ){
            return { true, glm::dvec2{0.0, 0.0} }; //- IMM!!!
        }

        auto mapEntPair = esrc::getnc_memMapEntPtr( iMPos );
        tprAssert( mapEntPair.first == ChunkMemState::Active );
        for( const auto &begoid : mapEntPair.second->get_majorGos() ){//- each bego
            if( begoid == dogoRef.id ){continue;}//-- skip self --
            if( this->adjacentBeGos.find(begoid) != this->adjacentBeGos.end() ){ continue; }//-- skip old adjacent bego --
            this->begoids.insert( begoid );
        }
    }

    //-------- 没有 bego 就直接 return ---------//
    if( this->begoids.empty() ){
        return { false, moveVec_ };
    }

    //----------------------------------------//
    //   check each bego，and collect tbegoids
    Circular dogoCir = dogoRef.calc_circular( CollideFamily::Move );
    Circular futureDogoCir = dogoCir.calc_new_circular( moveVec_ ); //- 位于 位移向量的终点

    Circular begoCir {};
    Square   begoSqu {};
    this->tbegoids.clear();
    //--
    for( const auto &begoid : this->begoids ){//- each bego

        GameObj &begoRef = esrc::get_goRef( begoid );
        const auto begoColliderType = begoRef.get_colliderType();

        //-- goAltiRange --//
        //  过滤掉，在 altiRange 不会碰撞的 bego --
        if( !is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange()) ){
            continue;
        }

        //-- 做二级碰撞检测，如果 targetPoint 确认发生碰撞，再计算出碰撞值 t --
        CollideState colliState {CollideState::Separate};
        std::pair<CollideState, glm::dvec2> squOut {};
        switch ( begoColliderType ){
            case ColliderType::Circular:

                begoCir = begoRef.calc_circular( CollideFamily::Move );
                //-- 剔除 背向而行 --
                if( is_dogoCircular_leave_begoCircular( moveVec_, dogoCir, begoCir ) ){
                    continue;
                }

                colliState = collideState_from_circular_2_circular( futureDogoCir, begoCir, 0.1 );
                if( colliState ==  CollideState::Separate ){ //- 相离
                    //-- do nothing ...
                }else if( colliState ==  CollideState::Adjacent ){ //- 相临                        
                        this->tbegoids.insert({ 1.0, begoRef.id }); //- 说明本次 位移的终点，正好和 bego相邻
                        
                }else if( colliState ==  CollideState::Intersect ){ //- 相交
                    double t = circularCast( moveVec_, dogoCir, begoCir ); //- 进一步计算出 t值， 并将 t值 存入 容器...
                    this->tbegoids.insert({ t, begoRef.id });

                }else{
                    tprAssert(0);
                }
                break;

            case ColliderType::Square:

                begoSqu = begoRef.calc_square();

                //-- 剔除 背向而行 --
                if( is_dogoCircular_leave_begoSquare( moveVec_, dogoCir, begoSqu ) ){
                    continue;
                }

                squOut = collideState_from_circular_2_square( futureDogoCir, begoSqu, 0.2 );
                if( squOut.first ==  CollideState::Separate ){ //- 相离
                    //-- do nothing ...
                }else if( squOut.first ==  CollideState::Adjacent ){ //- 相临                        
                    this->tbegoids.insert({ 1.0, begoRef.id }); //- 说明本次 位移的终点，正好和 bego相邻
                        
                }else if( squOut.first ==  CollideState::Intersect ){ //- 相交

                    //double t = squareCast( moveVec_, dogoCir, begoSqu ); //- 进一步计算出 t值， 并将 t值 存入 容器...
                    //this->tbegoids.insert({ t, begoRef.id });

                    this->tbegoids.insert({ 0.2, begoRef.id });
                
                }else{
                    tprAssert(0);
                }
                //*/

                break;


            default:
                tprAssert(0);
                break;
        }
    }//- each bego


    //------ 没有 tbegoids 就直接 return -------//
    if( this->tbegoids.empty() ){
        return { false, moveVec_ };//- 没有发生碰撞
    }


    //----------------------------------------//
    //-- 1~n 个新的 相邻bego,它们的 t值 是一样的
    double tMin = this->tbegoids.begin()->first; //- min
    for( const auto &tbego : this->tbegoids ){

        if( !is_closeEnough<double>(tbego.first, tMin, 0.01) ){
            continue;
        }

        //- 确认发生碰撞，调用对应的 functor
        // ... 暂未实现 ...


    }

    return  { true, moveVec_*tMin }; //- 用 t值 修正 位移向量

            /*
                //--------------//
                //   若确认碰撞 
                //--------------//
                // 现在，beGoPtr 就是 确认碰撞的 被动go
                //-- 优先执行 doAffect;
                if( doCehRef.isBody==true ){
                    if( (doCehRef.isCarryAffect==true) && (doGoRef.DoAffect_body!=nullptr) ){
                        doGoRef.DoAffect_body( doGoRef, beGoRef );
                    }
                }else{
                    if( (doCehRef.isCarryAffect==true) && (doGoRef.DoAffect_virtual!=nullptr) ){
                        doGoRef.DoAffect_virtual( doGoRef, beGoRef );
                    }
                }
                //-- 再执行 beAffect;
                if( (goDataRef.isCarryAffect==true) && (beGoRef.BeAffect_body!=nullptr) ){
                    beGoRef.BeAffect_body( doGoRef, beGoRef );
                }
                                    //-- 如果一个 被动go 已经被碰撞了。很可能在 后续帧中，
                                    //   被 同一个主动go 的其他 collient 再次碰撞
                                    //   应该确立一种方法，来避免这类重复碰撞。
                                    //   比如，一旦某个go被碰撞，它自己应该进入到一种状态。比如播放一段动画。
                                    //   此时外界再次施加的 碰撞技能
                                    //   都将对它失效.直到它从这个状态中结束。才再次允许被 碰撞并施加技能
                                    // -----
                                    //  这方面内容，应该在 affect 函数中实现
                                    //...

                //-- 如果确认的碰撞为 "可以穿过"，将继续后续检测
                //-- 只要出现一次 “无法穿过”式的碰撞，后续检测就被终止 --
                if( isPass_Check( this->isDoPass, beGoRef.get_collision_isBePass() )==false ){
                    isObstruct = true;
                    break;
                }

            */
}


/* ===========================================================
 *                 handle_chunkKeys
 * -----------------------------------------------------------
 */ 
void Collision::handle_chunkKeys(){

        //---------------------------//
        //  如果确认需要位移
        //   -- 检查本go 的 新chunk，如果发生变化，释放旧chunk 中的 goids, edgeGoIds
        //       登记到 新chunk 的 goids
        //   -- 重新统计 本go 的 chunkKeys，如果确认为 临界go，  
        //       登记到 主chunk 的 edgegoids 容器中
        //---------------------------//
        goid_t   goid = this->goRef.id;
        chunkKey_t newChunkKey = anyDPos_2_chunkKey( this->goRef.get_dpos() );
        Chunk &newChunkRef = esrc::get_chunkRef( newChunkKey );
        size_t eraseNum {};

        if( newChunkKey != this->goRef.currentChunkKey ){
            Chunk &oldChunkRef = esrc::get_chunkRef( this->goRef.currentChunkKey );
            eraseNum = oldChunkRef.erase_from_goIds(goid);
            tprAssert( eraseNum == 1 );
            oldChunkRef.erase_from_edgeGoIds(goid); // maybe 
            //---
            this->goRef.currentChunkKey = newChunkKey;
            newChunkRef.insert_2_goIds(goid);
        }
        
        //-- 不执行移动碰撞检测的 go，一定不是 edgeGo --
        if( this->goRef.isMoveCollide ){

            size_t chunkKeysSize = this->goRef.reCollect_chunkKeys();
            if( chunkKeysSize == 1 ){
                newChunkRef.erase_from_edgeGoIds(goid); // maybe
            }else if( chunkKeysSize > 1 ){
                newChunkRef.insert_2_edgeGoIds(goid);
            }else{
                tprAssert(0);
            }
        }
}





