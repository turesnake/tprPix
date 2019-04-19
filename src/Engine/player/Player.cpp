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


namespace{//------------
}//---------------------



/* ===========================================================
 *                    d2m    
 * -----------------------------------------------------------
 * -- 通过一个 diskPlayer 来改写 自己的各字段
 */
void Player::d2m( diskPlayer *_dp ){

    /*
    this->goid = _dp->go_id;
    goptr->targetPos.x = _dp->posx;
    goptr->targetPos.y = _dp->posy;
    */
}


/* ===========================================================
 *                      m2d  
 * -----------------------------------------------------------
 */
diskPlayer Player::m2d(){

    diskPlayer dp; //- return

    /*
    dp.go_id = goptr->id;
    dp.posx = goptr->targetPos.x;
    dp.posy = goptr->targetPos.y;
    */
    return dp;
}


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Player::init(){

    //-- nothing...
    //input::bind_inputINS_callback( std::bind( &Player::onInputINS, &esrc::player, _1 ) );
}


/* ===========================================================
 *                   bind_goPtr
 * -----------------------------------------------------------
 * -- 这个函数有漏洞
 */
void Player::bind_goPtr(){

    //-- 解绑旧go --//
    if( this->goPtr!=nullptr ){
        this->goPtr->isControlByPlayer = false;
    }

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

    //-----------------//
    //      camera
    //-----------------//
    //-- 让 camera 对其上1渲染帧 --
    //- 这会造成 camera 的延迟，但不要紧
    esrc::camera.set_targetPos( this->goPtr->goPos.get_currentFPos() ); //- 不应该放在此处...

    //---------------------------//
    //  
    //---------------------------//

    //  此处会有很多 处理 _inputINS 数据的操作
    //  在未来展开...

    //this->goPtr->inputINS = _inputINS; //- copy 

    
    this->goPtr->move.set_newCrawlDir( NineBox{ (int)_inputINS.crossX, (int)_inputINS.crossY } );
}

