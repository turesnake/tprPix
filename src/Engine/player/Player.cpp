/*
 * ====================== Player.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.10
 *                                        修改 -- 2018.12.10
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
#include "srcs_engine.h" 

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
    goid = _dp->go_id;
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

    //--- bind keyboard callback ---
    input::bind_key_callback( input::KEY::SPACE,  std::bind( &Player::onKeyDown_SPACE, &esrc::player ) );

    //--- bind gameCross[move] Keys ---
    input::bind_gameCross_key( input::DIRECTION::Left,  input::KEY::A );
    input::bind_gameCross_key( input::DIRECTION::Right, input::KEY::D );
    input::bind_gameCross_key( input::DIRECTION::Up,    input::KEY::W );
    input::bind_gameCross_key( input::DIRECTION::Down,  input::KEY::S );

    //--- bind gameCross callback ---
    input::bind_gameCross_callback( std::bind( &Player::onGameCross, &esrc::player, _1 ) );

}


/* ===========================================================
 *                   bind_goPtr
 * -----------------------------------------------------------
 */
void Player::bind_goPtr(){

    //-- 解绑旧go --//
    if( goPtr!=nullptr ){
        goPtr->isControlByPlayer = false;
    }

    //=== 检测 section 中的 go数据 是否被 实例化到 mem态 ===//
    //...
    goPtr = esrc::find_memGameObjs( goid );
    goPtr->isControlByPlayer = true;
}


/* ===========================================================
 *                  onGameCross
 * -----------------------------------------------------------
 * -- 每一渲染帧都会被调用，来处理 
 */
void Player::onGameCross( NineBox _nb ){

    //-----------------//
    //      camera
    //-----------------//
    //-- 让 camera 对其上1渲染帧 --
    //- 这会造成 camera 的延迟，但不要紧
    //esrc::camera.set_targetPos( goPtr->move.get_currentFPos() ); //- 不应该放在此处。
    esrc::camera.set_targetPos( goPtr->goPos.get_currentFPos() ); //- 不应该放在此处。

    //---------------------------//
    //  just save the cs in goPtr->move 
    //---------------------------//
    goPtr->move.set_newCrawlDir( _nb );
}




/* ===========================================================
 *               Keyboard CallBack
 * -----------------------------------------------------------
 */
void Player::onKeyDown_SPACE(){
    //cout << "_" << endl;
}




