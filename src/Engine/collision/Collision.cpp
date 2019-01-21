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
#include "GameMesh.h"
#include "GameObj.h"
#include "FramePos.h"
#include "srcs_engine.h"


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 
 */
void Collision::init( GameObj *_goPtr, GameMesh *_gameMeshPtr ){
    goPtr = _goPtr;
    gameMeshPtr = _gameMeshPtr;
    //...
}



/* ===========================================================
 *                     refresh
 * -----------------------------------------------------------
 * -- 也许会被 内置到 collide 函数内 
 */
void Collision::refresh(){

    //-- 若无动画帧切换，不执行后续 refresh --
    if( (gameMeshPtr->get_actionName()==currentaActionName) &&
        (gameMeshPtr->get_currentActionFrameIdx()==currentActionFrameIdx) ){
        return;
    }
    currentaActionName = gameMeshPtr->get_actionName();
    currentActionFrameIdx = gameMeshPtr->get_currentActionFrameIdx();

    //...
    

}


/* ===========================================================
 *                     collide
 * -----------------------------------------------------------
 * -- 碰撞检测主流程 
 */
void Collision::collide(){

    //-- 最简状态：什么也不优化，先走一遍流程 --//

    for( auto &m : goPtr->gameMeshs ){ //-- each go.gameMesh

        const FramePos &framePos = m.get_currentFramePos();

        for( auto &ceh : framePos.colliEntHeads ){ //-- each colliEntHead

        

            ColliEntSet &cesRef = esrc::colliEntSets.at( ceh.colliEntSetIdx ); //- get ces ref

            //--- 这就是我们要找的数据 ---//
            //cesRef.colliEnt_adds
            //cesRef.colliEnt_dels




        }

    }









}








