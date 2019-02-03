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

#include "debug.h"




/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 
 */
void Collision::init( GameObj *_goPtr ){
    goPtr = _goPtr;
    //...
}


/* ===========================================================
 *                   collide_for_crawl
 * -----------------------------------------------------------
 * -- 碰撞检测主流程 
 */
void Collision::collide_for_crawl( const NineBoxIdx &_nbIdx ){

    //-- 最简状态：什么也不优化，先走一遍流程... --//

    //-- 清空 renderPool --
    debug::clear_mapEntSlices();

    //------------------------------//
    //  获得  rootAnchor 当前 ppos
    //------------------------------//
    // 当前处于 crawl 节点帧，
    // rootAnchor 所在的 collient 一定对齐于 mapent
    IntVec2  currentPPos = goPtr->goPos.get_currentPPos();  //-- 直指 current rootAnchor 检测用

    MapCoord  cesMCPos;      //- 每个 ces左下角的 mcpos （世界绝对pos）
    MapCoord  colliEntMCPos; //- adds/dels 中，每个ent 的 mcpos （世界绝对pos）
    AltiRange currentAltiRange {};  //- each ceh abs AltiRange.
    MemMapEnt *mapEntPtr;  //- 目标 mapent
    GameObj   *beGoPtr;     //- 目标mapent 中存储的 major_go (被动go)
    
    //------------------------------//
    //  遍历每个  go.goMesh
    //  遍历每个  ces
    //------------------------------//
    auto ipair = goPtr->goMeshs.begin();
    for( ; ipair!=goPtr->goMeshs.end(); ipair++ ){
        GameObjMesh &meshRef = ipair->second;

        //-- 检测这个 mesh 是否可以跳过
        if( meshRef.isCollide == false ){
            continue;
        }

        for( auto &ceh : meshRef.get_currentFramePos().get_colliEntHeads() ){ //-- each colliEntHead

            currentAltiRange.set_by_addAlti( ceh.lAltiRange, goPtr->goPos.get_alti() ); 

            cesMCPos.set_by_ppos( currentPPos + ceh.pposOff_fromRootAnchor );
            ColliEntSet &cesRef = esrc::colliEntSets.at( ceh.colliEntSetIdx ); //- get ces ref

            //--- 访问 adds ---//
            for( const auto &i : cesRef.get_addEntOffs(_nbIdx) ){ //- each addEntOff
                colliEntMCPos = i + cesMCPos;
                //-- 拿着这个值，就能去 mapent／section 中查看数据了 

                        //-- 用图形debug 来显示 这组数据
                        debug::insert_new_mapEntSlice( colliEntMCPos );

                mapEntPtr = esrc::get_memMapEnt( colliEntMCPos );

                auto ipr = mapEntPtr->major_gos.begin();
                for( ; ipr!=mapEntPtr->major_gos.end(); ipr++ ){ //- each major_go in target mapent

                    beGoPtr = esrc::find_memGameObjs( ipr->first ); //- 获取目标go指针

                    //-- 没撞到，检查下一个 --
                    if( currentAltiRange.is_collide( beGoPtr->get_currentAltiRange( ipr->second ) )==false ){
                        continue;
                    }
                    
                    //-- 若确认碰撞 --
                    // 现在，beGoPtr 就是 确认碰撞的 被动go
                    //  其实这里有很多工作需要做。但暂时只实现最简单的一步：“放弃本次移动”...
                            cout << "collide; goid = " << ipr->first 
                                << endl;
                    
                    //-- 被动go.BeAffect() call --
                    if( beGoPtr->BeAffect != nullptr ){
                        beGoPtr->BeAffect( goPtr );
                    }

                    
                
                    //-- 如果一个 被动go 已经被碰撞了。很可能在 后续帧中，被 同一个主动go 的其他 collient 再次碰撞
                    //   应该确立一种方法，来避免这类重复碰撞。
                    //   比如，一旦某个go被碰撞，它自己应该进入到一种状态。比如播放一段动画。此时外界再次施加的 碰撞技能
                    //   都将对它失效.直到它从这个状态中结束。才再次允许被 碰撞并施加技能
                    //...


                    //-- 如果确认的碰撞为 "可以穿过"，将继续后续检测
                    //-- 只要出现一次 “无法穿过”式的碰撞，后续检测就被终止 --
                    //...


                }

            }
                    

            //-- 还缺一部分：当占据新 mapent后，要释放原有 mapent，注册新 mapent。
            //-- 这部分是否该放在此处完成 ？
            //  -1- 执行完整套碰撞检测，确认是否 “可以穿过” 
            //      如果 “无法穿过”，那将向 crawl层 发送信号，终止后续一切
            //      如果 “可以穿过”
            //  -2- 再次遍历 adds，针对每一个mapent，注册本go
            //  -3- 遍历 dels，针对每一个mapent，注销本go
            //...


        }

    }


}



/* ===========================================================
 *                     isPass_Check
 * -----------------------------------------------------------
 * -- 本实例为 主动go，参数为 被动go。检测此次碰撞 是否能 穿过
 * -- return:
 *     - true:   说明本次碰撞 可以穿过
 *     - false:  说明本次碰撞 无法穿过
 */
bool Collision::isPass_Check( bool _beGo_isSelfBePass ){

    //--- *** 本流程可能存在逻辑问题... *** ---//
    if( isPass == true ){
        //-- 此时，主动go比较特殊：火球／箭 等，允许穿过一切物体
        return true;
    }

    if( _beGo_isSelfBePass == true ){
        //-- 说明 被动go 允许别人穿过自己。比如是：草，地上的液体等
        return true;
    }else{
        //-- 主动go／被动go 都不运行穿过，才判定为 “无法穿过”
        return false;
    }
}




