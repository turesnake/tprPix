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
#include "GameObjMesh.h"
#include "GameObj.h"
#include "FramePos.h"
#include "srcs_engine.h"
#include "MapCoord.h"
#include "MapEnt.h"

//#include "debug.h"

/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 
 */
void Collision::init( GameObj *_goPtr ){
    this->goPtr = _goPtr;
    //...
}


/* ===========================================================
 *                   collide_for_crawl
 * -----------------------------------------------------------
 * -- 碰撞检测主流程 
 *    目前版本在 性能上有一定问题。因为它做了一次 四层遍历，以及一次三层遍历...
 *    在真的爆发 “性能问题” 时，再来优化此模块。
 * ------------
 * -- 命名规则：
 *   - "do..."  主动go所属变量
 *   - "be..."  被动go所属变量
 * -- return:
 *    true  -- 检测到 “无法穿过”的碰撞，将提示 crawl 放弃本回合移动
 *    false -- 未检测到 “无法穿过”的碰撞，crawl的 本次移动将正常进行
 */  
bool Collision::collide_for_crawl( const NineBoxIdx &_nbIdx ){

            //-- 清空 debug: renderPool --
            debug::clear_mapEntSlices();

    //------------------------------//
    MapCoord  cesMCPos;             //- 每个 ces左下角的 mcpos （世界绝对pos）
    AltiRange currentAltiRange {};  //- each ceh abs AltiRange.
    MemMapEnt *mapEntPtr;           //- 目标 mapent
    GameObj   *beGoPtr;         //- 目标mapent 中存储的 major_go (被动go)
    GameObj   *doGoPtr = this->goPtr; //- 碰撞检测 主动发起方

    // 当前处于 crawl 节点帧，
    // rootAnchor 所在的 collient 一定对齐于 mapent
    IntVec2  currentPPos = this->goPtr->goPos.get_currentPPos();  //-- 直指 current rootAnchor 检测用

    bool isObstruct {false}; //- 是否检测到 “无法穿过” 的碰撞。
                             //- 用于 跳出多层循环 
                             //  也是本函数的 返回值
                             // - true  检测到 “无法穿过”
                             // - false 未检测到 “无法穿过”
                            
    //------------------------------//
    // 检测当前移动方向的 所有 addsEnt
    //------------------------------//
    for( const auto &goMeshPair : doGoPtr->goMeshs  ){  //- each goMesh
        if(isObstruct) break;
        const GameObjMesh &meshRef = goMeshPair.second;
        if( meshRef.isCollide == false ) continue; //- 不参与碰撞检测的 gomesh 直接跳过

        for( const auto &doCehRef : meshRef.get_currentFramePos().get_colliEntHeads() ){ //-- each do_colliEntHead
            if(isObstruct) break;

            currentAltiRange.set_by_addAlti( doCehRef.lAltiRange, doGoPtr->goPos.get_alti() ); 
            cesMCPos.set_by_ppos( currentPPos + doCehRef.pposOff_fromRootAnchor );
            const ColliEntSet &doCesRef = esrc::colliEntSets.at( doCehRef.colliEntSetIdx ); //- get do_ces_ref

            //--- 访问 adds ---//
            for( const auto &i : doCesRef.get_addEntOffs(_nbIdx) ){ //- each add EntOff
                if(isObstruct) break;

                        //-- 图形debug --
                        debug::insert_new_mapEntSlice( i+cesMCPos );

                mapEntPtr = esrc::get_memMapEntPtr( i+cesMCPos );

                for( const auto &majorGoPair : mapEntPtr->major_gos ){ //- each major_go in target mapent
                    if(isObstruct) break;
                    const MajorGO_in_MapEnt &goDataRef = majorGoPair.second;
                    beGoPtr = esrc::get_memGameObjPtr( majorGoPair.first ); //- 获取目标go指针

                    //-- 没撞到，检查下一个 --
                    if( currentAltiRange.is_collide( beGoPtr->get_currentAltiRange( goDataRef.lAltiRange ) )==false ){
                        continue;
                    }
                    
                    //--------------//
                    //   若确认碰撞 
                    //--------------//
                    // 现在，beGoPtr 就是 确认碰撞的 被动go

                    //-- 优先执行 doAffect;
                    if( doCehRef.isBody==true ){
                        if( (doCehRef.isCarryAffect==true) && (doGoPtr->DoAffect_body!=nullptr) ){
                            doGoPtr->DoAffect_body( doGoPtr, beGoPtr );
                        }
                    }else{
                        if( (doCehRef.isCarryAffect==true) && (doGoPtr->DoAffect_virtual!=nullptr) ){
                            doGoPtr->DoAffect_virtual( doGoPtr, beGoPtr );
                        }
                    }
                    //-- 再执行 beAffect;
                    if( (goDataRef.isCarryAffect==true) && (beGoPtr->BeAffect_body!=nullptr) ){
                        beGoPtr->BeAffect_body( doGoPtr, beGoPtr );
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
                    if( isPass_Check( beGoPtr->get_collision_isBePass() )==false ){
                        isObstruct = true;
                        //break;
                    }

                }//-- each major_go in target mapent
            }//-- each add EntOff
        }//-- each do_colliEntHead
    }//- each goMesh


    //-- 若确认 “无法穿过”，直接退出本函数 --
    if( isObstruct==true ){
        return true;
    }

    //-- 若确认本回合移动 “可以穿过”，
    //   注销所有 dels
    //   登记所有 adds （只登记 isBody==true ）
    for( const auto &goMeshPair : doGoPtr->goMeshs  ){  //- each goMesh
        const GameObjMesh &meshRef = goMeshPair.second;
        if( meshRef.isCollide == false ) continue; //- 不参与碰撞检测的 gomesh 直接跳过

        for( const auto &doCehRef : meshRef.get_currentFramePos().get_colliEntHeads() ){ //-- each do_colliEntHead
            cesMCPos.set_by_ppos( currentPPos + doCehRef.pposOff_fromRootAnchor );
            const ColliEntSet &doCesRef = esrc::colliEntSets.at( doCehRef.colliEntSetIdx ); //- get do_ces_ref

            //-----------------//
            //   登记所有 adds 
            //-----------------//
            for( const auto &i : doCesRef.get_addEntOffs(_nbIdx) ){ //- each add EntOff
                mapEntPtr = esrc::get_memMapEntPtr( i+cesMCPos ); //- 目标 mapent
                mapEntPtr->major_gos.insert({ doGoPtr->id,
                                MajorGO_in_MapEnt{ doCehRef.lAltiRange, doCehRef.isCarryAffect } });
            }//-- each add EntOff

            //-----------------//
            //   注销所有 dels 
            //-----------------//
            for( const auto &i : doCesRef.get_delEntOffs(_nbIdx) ){ //- each del EntOff
                mapEntPtr = esrc::get_memMapEntPtr( i+cesMCPos ); //- 目标 mapent
                assert( mapEntPtr->major_gos.erase(doGoPtr->id) == 1 );
                            //-- 执行正式的注销操作，并确保原初 存在唯一的 目标元素
            }//-- each del EntOff

        }//-- each do_colliEntHead
    }//- each goMesh

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
bool Collision::isPass_Check( bool _isBePass ){

    //--- *** 本流程可能存在逻辑问题... *** ---//
    if( this->isDoPass == true ){
        //-- 此时，主动go比较特殊：火球／箭 等，允许穿过一切物体
        return true;
    }

    if( _isBePass == true ){
        //-- 说明 被动go 允许别人穿过自己。比如是：草，地上的液体等
        return true;
    }else{
        //-- 主动go／被动go 都不运行穿过，才判定为 “无法穿过”
        return false;
    }
}




