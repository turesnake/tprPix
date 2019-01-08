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

    //--- tmp ---
    CrossState  cs        {};      //- 每一回合的 cs值
    //bool    is_on_moving  {false}; //- 是否正处于回合内，只有在 节点帧（非回合内），才能改写 cs值
    int     count         {0};     //- 回合内计数器 
    int     maxCount      {4};     //- 计数器z最大值，与speed联动
    float   speed         {0.75f};  //- 单帧位移距离， 与maxCount联动

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
    input::bind_gameCross_key( DIRECTION::Left,  input::KEY::A );
    input::bind_gameCross_key( DIRECTION::Right, input::KEY::D );
    input::bind_gameCross_key( DIRECTION::Up,    input::KEY::W );
    input::bind_gameCross_key( DIRECTION::Down,  input::KEY::S );

    //--- bind gameCross callback ---
    input::bind_gameCross_callback( std::bind( &Player::onGameCross, &esrc::player, _1 ) );

}


/* ===========================================================
 *                   bind_goPtr
 * -----------------------------------------------------------
 */
void Player::bind_goPtr(){

    //=== 检测 section 中的 go数据 是否被 实例化到 mem态 ===//
    //...
    goPtr = esrc::find_memGameObjs( goid );
}


/* ===========================================================
 *                  onGameCross
 * -----------------------------------------------------------
 * -- 每一渲染帧都会被调用，来处理 
 */
void Player::onGameCross( CrossState _cs ){

    //-----------------//
    //      camera
    //-----------------//
    //-- 让 camera 对其上1渲染帧 --
    //- 这会造成 camera 的延迟，但不要紧
    esrc::camera.set_targetPos( goPtr->currentPos ); //- 不应该放在此处。


    //---------------------------//
    //  试验1: 
    //---------------------------//
    //tmp_move( _cs );
    goPtr->move.RenderUpdate( _cs );

}


/* ===========================================================
 *                      tmp_move
 * -----------------------------------------------------------
 */
void Player::tmp_move( CrossState _cs ){

    if( count == maxCount ){
        count = 0;
    }

    if( count == 0 ){
        cs = _cs;
        //------
        /*
        if( (cs.x!=0) && (cs.y!=0) ){ //- 斜向
            speed    = 1.0f;
            maxCount = 3;
        }else{ //- 十字移动
            speed    = 1.5f;
            maxCount = 2;
        }
        */
    }

    count++;
    basic_translate();
}


/* ===========================================================
 *                     basic_translate
 * -----------------------------------------------------------
 * -- 用于 tmp_move() 
 */
void Player::basic_translate(){

    if( cs.x == -1 ){
        goPtr->currentPos += glm::vec2{ -speed, 0.0f };  //- A -
    }else if( cs.x == 1 ){
        goPtr->currentPos += glm::vec2{ speed, 0.0f };   //- D -
    }
    if( cs.y == 1 ){
        goPtr->currentPos += glm::vec2{ 0.0f, speed };   //- W -
    }else if( cs.y == -1 ){
        goPtr->currentPos += glm::vec2{ 0.0f, -speed };   //- S -
    }
}



/* ===========================================================
 *               Keyboard CallBack
 * -----------------------------------------------------------
 */
void Player::onKeyDown_SPACE(){
    //cout << "_" << endl;
}










