/*
 * ========================= input.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   键盘，鼠标，joystick 等输入设备 管理文件
 * ----------------------------
 */
#include "pch.h"
#include "input.h" 

//-------------------- Engine --------------------//
#include "KeyBoard.h"
#include "GameKey.h"

#include "InputOriginData.h"

#include "JoystickButton_2_GameKey_map.h"
#include "KeyBoardKey_2_GameKey_map.h"


#include "esrc_player.h"
#include "esrc_window.h"
#include "esrc_thread.h"



namespace input{//------------- namespace input --------------------


namespace input_inn {//------------------- namespace: input_inn ----------------------//


    std::unique_ptr<InputOriginData> inputDataUPtr {nullptr};
    glm::dvec2 oldMouseCursorDPos {};

    bool isFirstCall {true};


    InputINS   inputINS {}; //- 记录玩家 鼠键输入。
    
    F_InputINS_Handle inputINS_handleFunc  {nullptr}; 

    //======= funcs =======//
    void reflresh_inputData();
    void keyboard_callback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_);
    void debug_inputData();


}//------------------- namespace: input_inn ----------------------//


/*  在 scene:begin 开始的地方 调用
 */
void InputInit_for_begin(){

    //---- new -----//
     //-- callbacks:
    glfwSetKeyCallback( esrc::get_windowPtr(), input_inn::keyboard_callback );

    input_inn::inputDataUPtr = std::make_unique<InputOriginData>();

}




/* ==========================================================
 *                 bind_inputINS_handleFunc
 *-----------------------------------------------------------
 */
void bind_inputINS_handleFunc( const F_InputINS_Handle &func_ ){
    input_inn::inputINS_handleFunc = func_;
}



/* ==========================================================
 *                 processInput
 *-----------------------------------------------------------
 */
void processInput( GLFWwindow *windowPtr_ ){

    //-------------------------------//
    //     先收集全部 Input 数据
    //-------------------------------//
    input_inn::reflresh_inputData();

    //input_inn::debug_inputData(); // show keys


    //-------------------------------//
    //  解析 InputOriginData
    //  转化为 input_inn::inputINS 
    //-------------------------------//

    input_inn::inputINS.clear_allKeys(); //- 0

    const auto &joysRef = input_inn::inputDataUPtr->joysticks;
    const auto &mouseRef = input_inn::inputDataUPtr->mouse;
    const auto &kbRef = input_inn::inputDataUPtr->keyboard;

    //----------- 方向键部分 -----------//
    // 手柄优先原则
    // 遍历所有手柄，找到第一个有输入的手柄，使用其值，屏蔽其他 方向键源
    // 如果所有手柄 方向轴，全无输入，才检测 键盘

    bool isFind_joyLeftAxes {false};
    glm::dvec2 joyLeftAxes { 0.0, 0.0 };


    for( const auto &[joyIdx, joyUPtr] : joysRef ){
        const Joystick &joyRef = *joyUPtr;

        if( joyRef.get_isAnyAxisPress() ){

            glm::dvec2 lAxes = joyRef.get_LeftAxes();

            if( !is_closeEnough( lAxes, glm::dvec2{0.0,0.0},0.001  ) ){
                isFind_joyLeftAxes = true;
                joyLeftAxes.x = lAxes.x;
                joyLeftAxes.y = -lAxes.y; // 取反
                break;
            }
        }
    }

    if( isFind_joyLeftAxes ){
        input_inn::inputINS.collect_dirAxes_from_joystick( joyLeftAxes );

    }else{

        bool is_find_dirKeys {false};

        if( kbRef.get_isAnyKeyPress() ){
            // 检查键盘 WSAD
            if( kbRef.get_key(KeyBoard::Key::W) ){ joyLeftAxes.y =  1.0; is_find_dirKeys = true; }
            if( kbRef.get_key(KeyBoard::Key::S) ){ joyLeftAxes.y = -1.0; is_find_dirKeys = true; }
            if( kbRef.get_key(KeyBoard::Key::A) ){ joyLeftAxes.x = -1.0; is_find_dirKeys = true; }
            if( kbRef.get_key(KeyBoard::Key::D) ){ joyLeftAxes.x =  1.0; is_find_dirKeys = true; }
        }

        if( is_find_dirKeys ){
            input_inn::inputINS.collect_dirAxes_from_joystick( joyLeftAxes );
        }
    }

    //----------- 功能按键部分 -----------//
    // 手柄优先原则
    // 遍历所有手柄，发现一个手柄 存在按键时，立即屏蔽后续的所有 按键源
    // 当所有手柄都无输入时，才检测 键盘

    bool isFind_joyButtons {false};

    for( const auto &[joyIdx, joyUPtr] : joysRef ){
        const Joystick &joyRef = *joyUPtr;

        if( joyRef.get_isAnyButtonPress() ){

            for( const auto &[joyButton, gameKey] : JoystickButton_2_GameKey_map ){
                if( joyRef.get_button( joyButton ) ){
                    input_inn::inputINS.set_key( gameKey );
                    if( !isFind_joyButtons ){
                        isFind_joyButtons = true;
                    }
                }
            }

            //-- 屏蔽后续所有 手柄
            if( isFind_joyButtons ){
                break;
            }
        }
    }

    if( !isFind_joyButtons ){

        for( const auto &[kb, gameKey] : KeyBoardKey_2_GameKey_map ){
            if( kbRef.get_key( kb ) ){
                input_inn::inputINS.set_key( gameKey );
            }
        }
    }

    //------------------------//
    //  处理 inputINS 中的数据
    //------------------------//
    input_inn::inputINS_handleFunc( input_inn::inputINS );
}







/* ==========================================================
 *                 get_mouse_pos
 *-----------------------------------------------------------
 * -- 获得鼠标 当前 坐标值
 *  通过设置: glfwSetInputMode( esrc::get_windowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
 *  鼠标以 程序窗口 左上角为 (0,0) 点，向右下角增长（基于像素）
 *  超出窗口范围，仍能获得准确的相对值（有时候是 负数）
 * ----------
 * 这个函数 目前仅用于 randow 种子生成
 */
IntVec2 get_mouse_pos(){
    double x {};
    double y {};
    glfwGetCursorPos( esrc::get_windowPtr(), &x, &y);
    return IntVec2{ (int)x, (int)y };
}





namespace input_inn {//------------------- namespace: input_inn ----------------------//




// 每一渲染帧，都彻底刷新 全体 input 数据
// 从而支持 类似手柄的 热拔插
void reflresh_inputData(){
    
    GLFWwindow *windowPtr = esrc::get_windowPtr();

    //----------------------------//
    //        keyboards 
    //  已经交给 callback 去处理
    //----------------------------//

    //----------------------------//
    //           mouse
    //----------------------------//
    {
        Mouse &mouseRef = input_inn::inputDataUPtr->mouse;

        mouseRef.leftButton = is_GLFW_PRESS( glfwGetMouseButton( windowPtr, GLFW_MOUSE_BUTTON_LEFT ) );
        mouseRef.rightButton = is_GLFW_PRESS( glfwGetMouseButton( windowPtr, GLFW_MOUSE_BUTTON_RIGHT ) );
        mouseRef.midButton = is_GLFW_PRESS( glfwGetMouseButton( windowPtr, GLFW_MOUSE_BUTTON_MIDDLE ) );

        double cursorX {};
        double cursorY {};

        glfwGetCursorPos( windowPtr, &cursorX, &cursorY );

        mouseRef.cursorDPos.x = cursorX;
        mouseRef.cursorDPos.y = cursorY; 
    }
    

    //----------------------------//
    //          joystick
    //----------------------------//
    {
        auto &joysRef = input_inn::inputDataUPtr->joysticks;

        for( int idx=0; idx<(GLFW_JOYSTICK_LAST -GLFW_JOYSTICK_1+1); idx++ ){

            // 每一帧都重新 搜索系统，
            // 收集每一个，支持 SDL_GameControllerDB 的 手柄
            if( (glfwJoystickPresent(idx)==GL_TRUE) && // Must before GamePad check!
                (glfwJoystickIsGamepad(idx)==GLFW_TRUE) ){
                
                auto [insertIt, insertBool] = joysRef.emplace( idx, std::make_unique<Joystick>(idx) ); // maybe
                                        // if existed, do nothing
                Joystick &joyRef = *(insertIt->second);

                joyRef.refresh(); // IMPORTANT!!!
            
            }else{
                joysRef.erase(idx); // maybe
                        // if not existed, do nothing
            }
        }
    }
}


// callback
// param:
//   scancode:
//   action: 
//     “GLFW_PRESS”, “GLFW_RELEASE” or “GLFW_REPEAT”
//     REPEAT 表示这个键被持续按住了。
//
//   mods: 用来实现类似 shift+z 之类的组合键。本游戏中完全忽略这个数据
// 
void keyboard_callback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_){

    tprAssert( input_inn::inputDataUPtr );
    KeyBoard &kbRef = input_inn::inputDataUPtr->keyboard;

    //-------------//
    //    key
    //-------------//
    KeyBoard::Key kb = glfwKey_2_KeyBoardKey(key_);
    // 本游戏 不关心的按键，直接忽略
    if( (kb==KeyBoard::Key::UNKNOWN) || (kb==KeyBoard::Key::NOT_CARE) ){
        return;
    }

    if( action_ == GLFW_PRESS ){
        kbRef.insert_key( kb );

    }else if( action_ == GLFW_RELEASE ){
        kbRef.erase_key( kb );
    }
}



void debug_inputData(){

    tprAssert( input_inn::inputDataUPtr );

    //----------------------------//
    //        keyboard
    //----------------------------//
    const auto &kbRef = input_inn::inputDataUPtr->keyboard;

    if( kbRef.get_isAnyKeyPress() ){
        for( const auto &k : kbRef.get_pressedKeysRef() ){
            tprDebug::console( "-- KeyBoard: {}", keyBoardKey_2_str(k) );
        }
    }

    //----------------------------//
    //         mouse
    //----------------------------//
    const Mouse &mouseRef = input_inn::inputDataUPtr->mouse;

    if( mouseRef.cursorDPos != input_inn::oldMouseCursorDPos ){
        input_inn::oldMouseCursorDPos = mouseRef.cursorDPos;

        tprDebug::console(
            "++ Mouse: Cursor: {0}, {1}",
            mouseRef.cursorDPos.x, mouseRef.cursorDPos.y 
        );
    }


    if( mouseRef.leftButton ){
        tprDebug::console( "++ Mouse: Left_Button: press;" );
    }
    if( mouseRef.rightButton ){
        tprDebug::console( "++ Mouse: Right_Button: press;" );
    }
    if( mouseRef.midButton ){
        tprDebug::console( "++ Mouse: Mid_Button: press;" );
    }

    //----------------------------//
    //        joystick
    //----------------------------//
    glm::dvec2 leftAxes {};
    glm::dvec2 rightAxes {};
    double LT {};
    double RT {};

    for( const auto &[joyIdx, joyUPtr] : input_inn::inputDataUPtr->joysticks ){
        const Joystick &joyRef = *joyUPtr;

        leftAxes = joyRef.get_LeftAxes();
        rightAxes = joyRef.get_RightAxes();
        LT = joyRef.get_LT();
        RT = joyRef.get_RT();

        if( !is_closeEnough<double>( leftAxes.x, 0.0, 0.001 ) ){
            tprDebug::console( "  LeftAxes.x: {}", leftAxes.x );
        }  
        if( !is_closeEnough<double>( leftAxes.y, 0.0, 0.001 ) ){
            tprDebug::console( "                LeftAxes.y: {}", leftAxes.y );
        }  

        if( !is_closeEnough<double>( rightAxes.x, 0.0, 0.001 ) ){
            tprDebug::console( "  RightAxes.x: {}", rightAxes.x );
        }
        if( !is_closeEnough<double>( rightAxes.y, 0.0, 0.001 ) ){
            tprDebug::console( "                RightAxes.y: {}", rightAxes.y );
        }

        if( !is_closeEnough<double>( LT, -1.0, 0.001 ) ){
            tprDebug::console( "  LT: {}", LT );
        }   
        if( !is_closeEnough<double>( RT, -1.0, 0.001 ) ){
            tprDebug::console( "  RT: {}", RT );
        }  

        if( joyRef.get_button(Joystick::Button::A) ){           tprDebug::console( "A, " ); }
        if( joyRef.get_button(Joystick::Button::B) ){           tprDebug::console( "B, " ); }
        if( joyRef.get_button(Joystick::Button::X) ){           tprDebug::console( "X, " ); }
        if( joyRef.get_button(Joystick::Button::Y) ){           tprDebug::console( "Y, " ); }
        if( joyRef.get_button(Joystick::Button::LB) ){          tprDebug::console( "LB, " ); }
        if( joyRef.get_button(Joystick::Button::RB) ){          tprDebug::console( "RB, " ); }
        if( joyRef.get_button(Joystick::Button::Back) ){        tprDebug::console( "Back, " ); }
        if( joyRef.get_button(Joystick::Button::Start) ){       tprDebug::console( "Start, " ); }
        if( joyRef.get_button(Joystick::Button::Guide) ){       tprDebug::console( "Guide, " ); }
        if( joyRef.get_button(Joystick::Button::LeftThumb) ){   tprDebug::console( "LeftThumb, " ); }
        if( joyRef.get_button(Joystick::Button::RightThumb) ){  tprDebug::console( "RightThumb, " ); }
        if( joyRef.get_button(Joystick::Button::Hat_Up) ){      tprDebug::console( "Hat_Up, " ); }
        if( joyRef.get_button(Joystick::Button::Hat_Down) ){    tprDebug::console( "Hat_Down, " ); }
        if( joyRef.get_button(Joystick::Button::Hat_Left) ){    tprDebug::console( "Hat_Left, " ); }
        if( joyRef.get_button(Joystick::Button::Hat_Right) ){   tprDebug::console( "Hat_Right, " ); }
    }

} 



}//------------------------ namespace: input_inn end --------------------//
}//----------------- namespace input: end -------------------

