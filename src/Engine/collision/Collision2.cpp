/*
 * ====================== Collision2.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    碰撞模块，暂没想好怎么用
 *    关键性数据 全在 mapEnt 中
 * ----------------------------
 */
#include "Collision2.h"

//-------------------- CPP --------------------//
#include <unordered_set>
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjMesh.h"
#include "GameObj.h"
#include "FramePos2.h"
#include "MapCoord.h"
#include "MapEnt.h"
#include "collide_oth.h"
#include "esrc_chunk.h"
#include "esrc_gameObj.h"

#include "esrc_time.h"


//#include "tprDebug.h"



/* ===========================================================
 *               collect_adjacentBeGos
 * -----------------------------------------------------------
 *  牺牲算力，在每一帧移动检测前，先检测一遍当前所在的 signInMapEnts
 *  临时计算出，当前的 相邻bego 集，存入 adjacentBeGos
 *  (而不是使用之前的 登记制)
 */ 
void Collision2::collect_adjacentBeGos(){

    // 调用本函数，意味着 goRef.isMoveCollide 一定为 true !!!
    //---------------------------------------//
    GameObj     &dogoRef = this->goRef; //- 碰撞检测 主动发起方

        //-- dogo Must be Circular !!! --
        tprAssert( dogoRef.get_rootFramePos2Ref().get_colliderType() == ColliderType::Circular );

    Circular dogoCir = dogoRef.calc_circular( CollideFamily::Move );


    this->adjacentBeGos.clear();
    this->begoids.clear();
    //----------------------------------------//
    //      初步收集 所有有效 begoid
    //  不包含 dogo 自己，不包含 旧的 adjacentBeGos 中的 bego
    for( const auto &iMPos : this->signInMapEntsUPtr->get_currentSignINMapEntsRef() ){
        //- 当发现某个 addEnt 处于非 Active 的 chunk。
        //  直接跳过，最简单的处理手段
        auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey( iMPos ) );
        if( chunkState != ChunkMemState::Active ){
            continue; //- skip
        }

        for( const auto &begoid : esrc::get_memMapEntRef_in_activeChunk(iMPos).get_majorGos() ){//- each bego
            if( begoid == dogoRef.id ){continue;}//-- skip self --
            this->begoids.insert( begoid );
        }
    }

    //----------------------------------------//
    for( const auto &begoid : this->begoids ){//- each bego
        GameObj &begoRef = esrc::get_goRef( begoid );

        glm::dvec2 dogo_2_bego = begoRef.get_currentDPos() - dogoRef.get_currentDPos();

        //-- goAltiRange --//
        //  过滤掉，在 altiRange 不会碰撞的 bego --
        if( !is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange()) ){
            continue;
        }

        //-- 仅仅计算 dogo,bego 距离，看是否相邻 --
        CollideState  colliState {CollideState::Separate};
        Circular begoCir {};
        Capsule begoCap  {};
        std::pair<CollideState, glm::dvec2> capOut {};
        switch ( begoRef.get_rootFramePos2Ref().get_colliderType() ){
            case ColliderType::Circular:

                begoCir = begoRef.calc_circular( CollideFamily::Move );
                colliState = collideState_from_circular_2_circular( dogoCir, begoCir, 1.0);

                if( colliState == CollideState::Adjacent ){
                    this->adjacentBeGos.insert({begoid, dogo_2_bego});
                }else{
                    //- nothing ...
                }

                break;
            case ColliderType::Capsule:

                begoCap = begoRef.calc_capsule( CollideFamily::Move );
                capOut = collideState_from_circular_2_capsule( dogoCir, begoCap, 1.0 );
                
                if( capOut.first == CollideState::Adjacent ){
                    this->adjacentBeGos.insert({begoid,  capOut.second }); //- 这是不对的
                }else{
                    //- nothing
                }       
                
                break;
            default:
                tprAssert(0);
                break;
        }
    }//- each bego
}

/* ===========================================================
 *               detect_adjacentBeGos
 * -----------------------------------------------------------
 * 精简版，只需要计算 偏转后的 位移向量
 */
glm::dvec2 Collision2::detect_adjacentBeGos( const glm::dvec2 &moveVec_ ){

    if( this->adjacentBeGos.empty() ){
        return  moveVec_;
    }

    GameObj     &dogoRef = this->goRef; //- 碰撞检测 主动发起方

    Circular dogoCir = dogoRef.calc_circular( CollideFamily::Move );
    Circular begoCir {};
    Capsule  begoCap {};
    //--------------------------------//
    bool is_leftSide_have {false};
    bool is_rightSide_have {false};
    std::vector<glm::dvec2> self_2_oths {};
    //--
    for( const auto &pair : this->adjacentBeGos ){
        
        GameObj &begoRef = esrc::get_goRef( pair.first );
        const auto &begoColliderType = begoRef.get_rootFramePos2Ref().get_colliderType();

        //----- 过滤掉那些 背向而行 的 bego -----
        if( begoColliderType == ColliderType::Circular ){

            begoCir = begoRef.calc_circular( CollideFamily::Move );
            if( is_dogoCircular_leave_begoCircular(moveVec_, dogoCir, begoCir ) ){
                continue;
            }
        }else if( begoColliderType == ColliderType::Capsule ){

            begoCap = begoRef.calc_capsule( CollideFamily::Move );
            if( is_dogoCircular_leave_begoCapsule(moveVec_, dogoCir, begoCap) ){
                continue;
            }

        }else{
            tprAssert(0);
        }
                
        glm::dvec2 innVec = calc_innVec( moveVec_, pair.second );

        //-- 几乎和 位移 同方向
        if( std::abs(innVec.y) < 0.01 ){
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
    //- 现在，sum指向 总阻力向量 的反方向
    double sumLen = glm::length( sum );

    //-- 计算 偏移向量 --
    double pct = std::abs(calc_innVec(sum, moveVec_).x) / sumLen;
    glm::dvec2 newSum = sum * pct;
    return (moveVec_ - newSum); //- 偏转后新的 位移向量
}



/* ===========================================================
 *            detect_for_move2  [Sec]
 * -----------------------------------------------------------
 *  暂时接替，原来的 Move::for_regularGo_inn2()
 */ 
glm::dvec2 Collision2::detect_for_move2( const glm::dvec2 &moveVec_ ){


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

    isSignINMapEntsChanged = this->signInMapEntsUPtr->forecast_signINMapEnts( this->goRef.get_currentDPos() + moveVec );
    //-- 不管 forecast 的结果是否为 true，都要做 碰撞检测 --
    auto out = this->for_move2_inn( moveVec ); // MAJOR !!!! 

    isObstruct = out.first;
    if( isObstruct ){
        moveVec = out.second;
    }

    //  还需要再次调用 forecast 函数
    isSignINMapEntsChanged = this->signInMapEntsUPtr->forecast_signINMapEnts( this->goRef.get_currentDPos() + moveVec );


    if( isObstruct == false ){

        //-- update adds/dels --
        if( isSignINMapEntsChanged == true ){

            this->signInMapEntsUPtr->sync_currentSignINMapEnts_from_future();
            //-- adds --
            for( const auto &mpos : this->signInMapEntsUPtr->get_currentAddsRef() ){
                auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( mpos );
                mapEntRef.insert_2_majorGos( this->goRef.id );
            }
            //-- dels --
            for( const auto &mpos : this->signInMapEntsUPtr->get_currentDelsRef() ){
                auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( mpos );
                mapEntRef.erase_the_onlyOne_from_majorGos( this->goRef.id );
                        //-- 执行正式的注销操作，并确保原初 存在唯一的 目标元素
            }
        }
    
        //---------------------------//
        this->handle_chunkKeys(); //-- 更新 dogo 在 chunk 中的登记信息
    }

    return moveVec; //- 修正后的 位移向量
}



/* ===========================================================
 *            for_move2_inn  [Sec]
 * -----------------------------------------------------------
 * -- return:
 *    bool       -- 位移是否遭遇 阻挡  
 *    glm::dvec2 -- 修正后的位移向量（ 原值／t／偏向 ）
 */  
std::pair<bool,glm::dvec2> Collision2::for_move2_inn( const glm::dvec2 &moveVec_ ){

        //- only for debug
        double currentTime = esrc::get_timer().get_currentTime();

    // 调用本函数，意味着 goRef.isMoveCollide 一定为 true !!!
    //---------------------------------------//
    GameObj     &dogoRef = this->goRef; //- 碰撞检测 主动发起方
    const auto  &dogoRootFramePos2Ref = dogoRef.get_rootFramePos2Ref();
    glm::dvec2   dogeTargetDPos = dogoRef.get_currentDPos() + moveVec_;

    
        //-- dogo Must be Circular !!! --
        tprAssert( dogoRootFramePos2Ref.get_colliderType() == ColliderType::Circular );


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

        const auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( iMPos );
        for( const auto &begoid : mapEntRef.get_majorGos() ){//- each bego
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
    Circular dogoCir = dogoRootFramePos2Ref.calc_circular( dogoRef.get_currentDPos(), CollideFamily::Move );
    Circular futureDogoCir = dogoCir.calc_new_circular( moveVec_ ); //- 位于 位移向量的终点

    Circular begoCir {};
    Capsule  begoCap {};
    this->tbegoids.clear();
    //--
    for( const auto &begoid : this->begoids ){//- each bego

        GameObj &begoRef = esrc::get_goRef( begoid );
        const auto &begoRootFramePos2Ref = begoRef.get_rootFramePos2Ref();
        const auto &begoColliderType = begoRootFramePos2Ref.get_colliderType();

        //-- goAltiRange --//
        //  过滤掉，在 altiRange 不会碰撞的 bego --
        if( !is_GoAltiRange_collide(dogoRef.get_currentGoAltiRange(), begoRef.get_currentGoAltiRange()) ){
            continue;
        }

        //-- 做二级碰撞检测，如果 targetPoint 确认发生碰撞，再计算出碰撞值 t --
        CollideState colliState {CollideState::Separate};
        std::pair<CollideState, glm::dvec2> capOut {};
        switch ( begoColliderType ){
            case ColliderType::Circular:

                begoCir = begoRef.calc_circular( CollideFamily::Move );
                //-- 剔除 背向而行 --
                if( is_dogoCircular_leave_begoCircular( moveVec_, dogoCir, begoCir ) ){
                    continue;
                }

                colliState = collideState_from_circular_2_circular( futureDogoCir, begoCir, 0.1 );
                if( colliState ==  CollideState::Intersect ){ //- 相交
                    double t = circularCast( moveVec_, dogoCir, begoCir ); //- 进一步计算出 t值， 并将 t值 存入 容器...
                    this->tbegoids.insert({ t, begoRef.id });

                }else if( colliState ==  CollideState::Adjacent ){ //- 相临                        
                        this->tbegoids.insert({ 1.0, begoRef.id }); //- 说明本次 位移的终点，正好和 bego相邻
                        
                }else if( colliState ==  CollideState::Separate ){ //- 相离
                    //-- do nothing ...
                }else{
                    tprAssert(0);
                }


                break;
            case ColliderType::Capsule:

                begoCap = begoRef.calc_capsule( CollideFamily::Move );
                //-- 剔除 背向而行 --
                if( is_dogoCircular_leave_begoCapsule( moveVec_, dogoCir, begoCap ) ){
                    continue;
                }
                

                capOut = collideState_from_circular_2_capsule( futureDogoCir, begoCap, 1.0 );
                if( capOut.first ==  CollideState::Intersect ){ //- 相交
                    //- 进一步计算出 t值， 并将 t值 存入 容器...
                    double t = capsuleCast( moveVec_, dogoCir, begoCap );
                    this->tbegoids.insert({ t, begoRef.id });

                }else if( capOut.first ==  CollideState::Adjacent ){ //- 相临
                        this->tbegoids.insert({ 1.0, begoRef.id }); //- 说明本次 位移的终点，正好和 bego相邻
                    
                }else if( capOut.first ==  CollideState::Separate ){ //- 相离
                    //-- do nothing ...
                }else{
                    tprAssert(0);
                }

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
    return  { true, moveVec_ * tMin }; //- 用 t值 修正 位移向量


    /*
    {
        //------------------------------//
        // 检测当前移动方向的 所有 addsEnt
        //------------------------------//
        currentGoAltiRange.set_by_addAlti( doGoRef.get_rootFramePos2Ref().get_lGoAltiRangeRef(), doGoRef.get_pos_alti() ); 
        //--- 访问 adds ---//
        for( const auto &mpos : doGoRef.get_currentSignINMapEntsRef() ){ //- each add EntOff
        //for( const auto &i : doCesRef.get_addEntOffs(nbIdx_) ){ //- each add EntOff

            if(isObstruct) break;

            //- 当发现某个 addEnt 处于非 Active 的 chunk。
            //  直接判定此次碰撞 为 阻塞，最简单的处理手段
            auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey( mpos ) );
            if( chunkState != ChunkMemState::Active ){
                isObstruct = true;
                break;
            }

            const auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( mpos );

            for( const auto &majorGoPair : mapEntRef.get_majorGos() ){ //- each major_go in target mapent
                if(isObstruct) break;
                const MajorGO_in_MapEnt &goDataRef = majorGoPair.second;
                GameObj &beGoRef = esrc::get_goRef( majorGoPair.first ); //- 目标mapent 中存储的 major_go (被动go)

                //-- 没撞到，检查下一个 --
                if( currentGoAltiRange.is_collide( beGoRef.get_currentGoAltiRange( goDataRef.lGoAltiRange ) )==false ){
                    continue;
                }
                            
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
            }//-- each major_go in target mapent
        }//-- each add EntOff
    }
    */

}


/* ===========================================================
 *                 handle_chunkKeys
 * -----------------------------------------------------------
 */ 
void Collision2::handle_chunkKeys(){

        //---------------------------//
        //  如果确认需要位移
        //   -- 检查本go 的 新chunk，如果发生变化，释放旧chunk 中的 goids, edgeGoIds
        //       登记到 新chunk 的 goids
        //   -- 重新统计 本go 的 chunkKeys，如果确认为 临界go，  
        //       登记到 主chunk 的 edgegoids 容器中
        //---------------------------//
        goid_t   goid = this->goRef.id;
        chunkKey_t newChunkKey = anyDPos_2_chunkKey( this->goRef.get_currentDPos() );
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





