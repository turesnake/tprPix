/*
 * ========================= Collision.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.08
 *                                        修改 -- 2019.01.08
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

            // ceh.lAltiRange //-- 高度区间值。

            cesMCPos.set_by_ppos( currentPPos + ceh.pposOff_fromRootAnchor );
            ColliEntSet &cesRef = esrc::colliEntSets.at( ceh.colliEntSetIdx ); //- get ces ref

            //--- 访问 adds ---//
            for( const auto &i : cesRef.get_addEntOffs(_nbIdx) ){

                colliEntMCPos = i + cesMCPos;
                //-- 拿着这个值，就能去 mapent／section 中查看数据了 

                    //-- 现在，我们可以先打印这组数据... 
                    /*
                    cout << "addEntMPos: " << colliEntMCPos.get_mpos().x
                        << ", " << colliEntMCPos.get_mpos().y 
                        << endl;
                    */
                    

                    //-- 用图形来显示 
                    debug::insert_new_mapEntSlice( colliEntMCPos );
                    
            }
                    //cout << endl;

        }

    }


}








