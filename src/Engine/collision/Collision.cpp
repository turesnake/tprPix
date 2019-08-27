/*
 * ========================= Collision.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    碰撞模块，暂没想好怎么用
 *    关键性数据 全在 mapEnt 中
 * ----------------------------
 */
#include "Collision.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjMesh.h"
#include "GameObj.h"
#include "FramePos.h"
#include "MapCoord.h"
#include "MapEnt.h"
#include "esrc_colliEntSet.h"
#include "esrc_chunk.h"
#include "esrc_gameObj.h"

//#include "tprDebug.h"

/* ===========================================================
 *                   detect_collision    [2rd]
 * -----------------------------------------------------------
 *  移动专用 碰撞检测 
 *   第二版
 *   go 携带的碰撞盒 变成了一个类似 风筝的存在，由自由移动的 go 拖着行走
 * ------------
 * 参数 nbIdx_ 限制了 每帧移动范围，只能向 周围 8个 mapent 移动
 * ------------
 * -- 命名规则：
 *   - "do..."  主动go所属变量
 *   - "be..."  被动go所属变量
 * -- return:
 *    true  -- 检测到 “无法穿过”的碰撞，将提示 crawl 放弃本回合移动
 *    false -- 未检测到 “无法穿过”的碰撞，crawl的 本次移动将正常进行
 */  
bool Collision::detect_collision( const NineBoxIdx &nbIdx_ ){

    // 当前的 实现已经收缩了检测范围：只检测 doGo.rootGoMesh
    // 因为本函数只在 dogo 发生 节点位移时，才被触发。
    // 所以无法用于 技能向的 碰撞检测，比如：
    //    一个生物，站在原地，挥刀砍向前方。
    // 在这种情景里， 本函数就不会被调用

    //------------------------------//
    //MapCoord    cesMCPos         {};        //- 每个 ces左下角的 mcpos （世界绝对pos）
    IntVec2     cesMPos          {};
    GoAltiRange currentGoAltiRange {};      //- each ceh abs GoAltiRange.
    GameObj     &doGoRef = this->goRef; //- 碰撞检测 主动发起方

    IntVec2     tmpEntMPos {};

    bool isRootCollide = doGoRef.get_goMeshRef("root").isCollide;

    //----------------------        
    const ColliEntHead &doCehRef = doGoRef.get_rootColliEntHeadRef();
    const ColliEntSet  &doCesRef = esrc::get_colliEntSetRef( doCehRef.colliEntSetIdx ); //- get do_ces_ref

    bool isObstruct {false}; //- 是否检测到 “无法穿过” 的碰撞。
                             //- 用于 跳出多层循环 
                             //  也是本函数的 返回值
                             // - true  检测到 “无法穿过”
                             // - false 未检测到 “无法穿过”
                            
    //------------------------------//
    // 检测当前移动方向的 所有 addsEnt
    //------------------------------//
    if( isRootCollide ){

        currentGoAltiRange.set_by_addAlti( doCehRef.lGoAltiRange, doGoRef.get_pos_alti() ); 

        //cesMCPos.set_by_mpos( currentMPos - doCehRef.mposOff_from_cesLB_2_centerMPos );
        //cesMCPos.set_by_mpos( doGoRef.get_rootCES_leftBottom_MPos() );
        cesMPos = doGoRef.get_rootCES_leftBottom_MPos();

        //--- 访问 adds ---//
        for( const auto &i : doCesRef.get_addEntOffs(nbIdx_) ){ //- each add EntOff
            if(isObstruct) break;

            tmpEntMPos = i.get_mpos() + cesMPos;

            //- 当发现某个 addEnt 处于非 Active 的 chunk。
            //  直接判定此次碰撞 为 阻塞，最简单的处理手段
            auto chunkState = esrc::get_chunkMemState( anyMPos_2_chunkKey( tmpEntMPos ) );
            if( chunkState != ChunkMemState::Active ){
                isObstruct = true;
                break;
            }

            const auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( tmpEntMPos );

            for( const auto &majorGoPair : mapEntRef.get_major_gos() ){ //- each major_go in target mapent
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
                if( isPass_Check( beGoRef.get_collision_isBePass() )==false ){
                    isObstruct = true;
                    break;
                }

            }//-- each major_go in target mapent
        }//-- each add EntOff
    }

    //-- 若确认 “无法穿过”，直接退出本函数 --
    if( isObstruct==true ){
        return true;
    }

    //-- 不参与碰撞的go。不需要 注销和登记 collient
    if( isRootCollide == false ){
        return false;
    }

    //-- 若确认本回合移动 “可以穿过”，
    //   注销所有 dels
    //   登记所有 adds （只登记 isBody==true ）
    //   只有 rootGoMesh 参与登记
    //-----------------//
    //   登记所有 adds 
    //-----------------//
    for( const auto &i : doCesRef.get_addEntOffs(nbIdx_) ){ //- each add EntOff
        auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( i.get_mpos()+cesMPos );
        mapEntRef.insert_2_major_gos( doGoRef.id, doCehRef.lGoAltiRange, doCehRef.isCarryAffect );
    }//-- each add EntOff

    //-----------------//
    //   注销所有 dels 
    //-----------------//
    for( const auto &i : doCesRef.get_delEntOffs(nbIdx_) ){ //- each del EntOff
        auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( i.get_mpos()+cesMPos );
        mapEntRef.erase_the_onlyOne_from_major_gos( doGoRef.id );
                        //-- 执行正式的注销操作，并确保原初 存在唯一的 目标元素
    }//-- each del EntOff
    
    return false;
}



/* ===========================================================
 *                     isPass_Check
 * -----------------------------------------------------------
 * -- 本实例为 主动go，参数为 被动go。检测此次碰撞 是否能 穿过
 * -- return:
 *     - true:   说明本次碰撞 可以穿过
 *     - false:  说明本次碰撞 无法穿过
 */
bool Collision::isPass_Check( bool isBePass_ ){

    //--- *** 本流程可能存在逻辑问题... *** ---//
    if( this->isDoPass == true ){
        //-- 此时，主动go比较特殊：火球／箭 等，允许穿过一切物体
        return true;
    }

    if( isBePass_ == true ){
        //-- 说明 被动go 允许别人穿过自己。比如是：草，地上的液体等
        return true;
    }else{
        //-- 主动go／被动go 都不运行穿过，才判定为 “无法穿过”
        return false;
    }
}

