



#include "Script/gameObjs/Norman.h" 
#include "Script/gameObjs/BigMan.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 
#include "GameObj.h" 
#include "MapCoord.h"
#include "IntVec.h"

using std::string;

//#include "debug.h" //- tmp


void create_a_Norman( const IntVec2 &_ppos );
void create_a_BigMan( const IntVec2 &_ppos );

/* ===========================================================
 *                       go_byPass
 * -----------------------------------------------------------
 * -- 跳过 section，硬生成一个／组 go实例（Dog_A）
 */
void go_byPass(){

    //-------------------------//
    //  手动CREATE 若干个 go/Dog_A 实例
    //--------------------------//
    

    create_a_Norman( IntVec2{ 50, 50 } );

    /*
    create_a_BigMan( IntVec2{ 130, 145 } );
    create_a_BigMan( IntVec2{ 145, 155 } );
    create_a_BigMan( IntVec2{ 150, 130 } );
    */

    /*
    create_a_Norman( IntVec2{ 130, 145 } );
    create_a_Norman( IntVec2{ 145, 155 } );
    create_a_Norman( IntVec2{ 150, 130 } );
    */
    

    
    //-------------------------//
    //  手动将所有 现存的 go实例，
    //  添加入  goids_active 容器
    //--------------------------//
    auto it = esrc::memGameObjs.begin();
    for( ; it!=esrc::memGameObjs.end(); it++ ){
        esrc::goids_active.insert( it->first );
    }

}


/* ===========================================================
 *                      create_a_Dog_A
 * -----------------------------------------------------------
 * -- 
 */

void create_a_Norman( const IntVec2 &_ppos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::find_memGameObjs( goid ); //- 获取目标go指针
    gameObjs::norman.init( goPtr );
    goPtr->goPos.init_by_currentMCPos( MapCoord{ ppos_2_mpos(_ppos) } );
            //-- 这是一个严格检查，_ppos 必须对齐于 mapent

            goPtr->debug();


    //-- 将自己 注册到 当前 mapent 上 --
    //  临时性的，很简陋的实现...
    //  遍历 go的每一个 collient，将它们对应的 每一个 mapent 都注册

    //------------------------------//
    //  遍历每个  go.goMesh
    //  遍历每个  ces
    //------------------------------//
    IntVec2   currentPPos = goPtr->goPos.get_currentPPos();  //-- 直指 current rootAnchor 检测用
    MapCoord  cesMCPos;      //- 每个 ces左下角的 mcpos （世界绝对pos）
    MapCoord  colliEntMCPos; //- adds/dels 中，每个ent 的 mcpos （世界绝对pos）
    MemMapEnt *mapEntPtr;    //- 目标 mapent

    auto ipair = goPtr->goMeshs.begin();
    for( ; ipair!=goPtr->goMeshs.end(); ipair++ ){ //- each gomesh
        GameObjMesh &meshRef = ipair->second;

        //-- 未开启碰撞检测的 mesh 可以直接跳过 --
        if( meshRef.isCollide == false ){
            continue;
        }

        for( auto &ceh : meshRef.get_currentFramePos().get_colliEntHeads() ){ //-- each ceh
 
            cesMCPos.set_by_ppos( currentPPos + ceh.pposOff_fromRootAnchor );
            ColliEntSet &cesRef = esrc::colliEntSets.at( ceh.colliEntSetIdx ); //- get ces ref

            for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in target_ces

                colliEntMCPos = i + cesMCPos;
                //-- 这就是 每一个 ces.collient 的 mcpos

                mapEntPtr = esrc::get_memMapEnt( colliEntMCPos ); //- 目标 mapent 访问权 --

                //-- 并不检测 当前 mapent 中是否有 重合的 go。而是直接 将数据 存入 mapent
                mapEntPtr->major_gos.insert({ goid, ceh.lAltiRange });
            }
        }
    }
}


void create_a_BigMan( const IntVec2 &_ppos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::find_memGameObjs( goid ); //- 获取目标go指针
    gameObjs::big_man.init( goPtr );
    goPtr->goPos.init_by_currentMCPos( MapCoord{ ppos_2_mpos(_ppos) } );
            //-- 这是一个严格检查，_ppos 必须对齐于 mapent
}

