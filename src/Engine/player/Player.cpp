/*
 * ====================== Player.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   hold data about player
 * ----------------------------
 */
#include "Player.h"

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "input.h" 
//#include "GODirection.h"
#include "GameKey.h"
#include "esrc_camera.h" 
#include "esrc_gameObj.h" 
#include "ParamBinary.h"


//-------------------- Script --------------------//
#include "Script/resource/ssrc.h"
#include "Script/gameObjs/create_goes.h"


using namespace std::placeholders;

#include "tprDebug.h" //- tmp

/* ===========================================================
 *                   get_goRef
 * -----------------------------------------------------------
 */
GameObj &Player::get_goRef() const {
    return esrc::get_goRef( this->goid );
}


/* ===========================================================
 *                   bind_go
 * -----------------------------------------------------------
 * -- 这个函数有漏洞
 */
void Player::bind_go( goid_t goid_ ){


    //--- first bind ---//
    if( this->goid == NULLID ){

        //-- bind new go --//
        tprAssert( goid_ != NULLID );
        auto &newGoRef = esrc::get_goRef( goid_ );
        newGoRef.isControlByPlayer = true;
        this->goid = goid_;

        //-- create playerGoCircle --//
        // 和 常规go 一样， playerGoCircle 也会被登记到 chunk 上，但不参与 碰撞检测
        this->playerGoCircle_goid = gameObjs::create_a_Go(  ssrc::get_goSpecId("playerGoCircle"),
                                                            newGoRef.get_dpos(),
                                                            emptyParamBinary );
        
        //-- playerGoCircle 的数据同步 --
        GameObj &playerGoCircleRef = esrc::get_goRef( this->playerGoCircle_goid );
        playerGoCircleRef.move.set_speedLvl( newGoRef.move.get_speedLvl() ); //- 同步 speedLv
                                        //-- 当 go 自行加速，这个同步就失效了 ...
                                        //   未修改 ...
        

    }else{
        //-- 解绑旧go --//
        GameObj &oldGoRef = esrc::get_goRef( this->goid );
        oldGoRef.isControlByPlayer = false;

        //=== 检测 chunk 中的 go数据 是否被 实例化到 mem态 ===//
        //...

        //-- bind new go --//
        tprAssert( goid_ != NULLID );
        GameObj &newGoRef = esrc::get_goRef( goid_ );
        newGoRef.isControlByPlayer = true;
        this->goid = goid_;

        //-- reset playerGoCircle mpos --//




    }




                    //------


    //-- 若是第一次调用，生成 playerGoCircle 实例 --//
    /*
    if( this->goid == NULLID ){
        this->playerGoCircle_goid = gameObjs::create_a_Go(  ssrc::get_goSpecId("playerGoCircle"),
                                                            IntVec2{0,0},
                                                            emptyParamBinary );
    }
    */

    //-- 解绑旧go --//
    /*
    if( this->goid != NULLID ){
        GameObj &oldGoRef = esrc::get_goRef( this->goid );
        oldGoRef.isControlByPlayer = false;
    }
    */

    //=== 检测 chunk 中的 go数据 是否被 实例化到 mem态 ===//
    //...

    /*
    tprAssert( goid_ != NULLID );
    GameObj &newGoRef = esrc::get_goRef( goid_ );
    newGoRef.isControlByPlayer = true;
    this->goid = goid_;
    */

    //-- 同步 playerGoCircle 的 pos --//
    //...

}


/* ===========================================================
 *                  handle_inputINS   
 * -----------------------------------------------------------
 *  玩家输入的鼠键 -> inputIns -> 游戏指令
 *  本函数获得 inputIns，将其转换为 游戏指令，并传输给 player.go
 * -------
 *  每一渲染帧都会被调用
 */
void Player::handle_inputINS( const InputINS &inputINS_ ){


    //  此处会有很多 处理 _inputINS 数据的操作
    //  在未来展开...

    //this->goPtr->inputINS = _inputINS; //- copy 

    GameObj &goRef = esrc::get_goRef( this->goid );
    goRef.move.set_newCrawlDirAxes( inputINS_.get_dirAxes() );
}

