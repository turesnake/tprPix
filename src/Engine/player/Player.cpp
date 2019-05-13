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
#include "input.h" 
#include "GODirection.h"
#include "GameKey.h"
#include "esrc_camera.h" 
#include "esrc_gameObj.h" 


using namespace std::placeholders;


#include "debug.h" //- tmp


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Player::init(){

    //-- nothing...
}


/* ===========================================================
 *                   bind_go
 * -----------------------------------------------------------
 * -- 这个函数有漏洞
 */
void Player::bind_go( goid_t _goid ){

    //-- 解绑旧go --//
    if( this->goPtr!=nullptr ){
        this->goPtr->isControlByPlayer = false;
    }


    assert( _goid != NULLID );
    this->goid = _goid;

    //=== 检测 section 中的 go数据 是否被 实例化到 mem态 ===//
    //...
    this->goPtr = esrc::get_memGameObjPtr( this->goid );
    this->goPtr->isControlByPlayer = true;
}


/* ===========================================================
 *                  handle_inputINS   
 * -----------------------------------------------------------
 *  玩家输入的鼠键 -> inputIns -> 游戏指令
 *  本函数获得 inputIns，将其转换为 游戏指令，并传输给 player.go
 * -------
 *  每一渲染帧都会被调用
 */
void Player::handle_inputINS( const InputINS &_inputINS ){


    //  此处会有很多 处理 _inputINS 数据的操作
    //  在未来展开...

    //this->goPtr->inputINS = _inputINS; //- copy 
    
    this->goPtr->move.set_newCrawlDirAxes( _inputINS.get_dirAxes() );
}

