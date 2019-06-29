/*
 * ====================== Player.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    存储所有 玩家 信息。
 *    ----------
 *    
 * ----------------------------
 */
#include "Player.h"

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "input.h" 
#include "GODirection.h"
#include "GameKey.h"
#include "esrc_camera.h" 
#include "esrc_gameObj.h" 


using namespace std::placeholders;


#include "tprDebug.h" //- tmp


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Player::init(){

    //-- nothing...
    this->goid = NULLID; 
}


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

    //-- 解绑旧go --//
    if( this->goid != NULLID ){
        GameObj &oldGoRef = esrc::get_goRef( this->goid );
        oldGoRef.isControlByPlayer = false;
    }

    //=== 检测 section 中的 go数据 是否被 实例化到 mem态 ===//
    //...

    tprAssert( goid_ != NULLID );
    GameObj &newGoRef = esrc::get_goRef( goid_ );
    newGoRef.isControlByPlayer = true;
    this->goid = goid_;
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

