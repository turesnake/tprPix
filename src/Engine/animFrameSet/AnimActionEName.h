/*
 * ======================== AnimActionEName.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ANIM_ACTION_E_NAME_H
#define TPR_ANIM_ACTION_E_NAME_H

//------------------- CPP --------------------//
#include <string>


// animaction name
// ---
// 代替 std::string, 提高运行时效率
// 缺点: 随着游戏内容的膨胀，会有越来越多的 元素加入进来。
// 每一次都会影响本 enum h文件
// 从而需要重编译
enum class AnimActionEName{

    Idle,

    Walk,
    Run,
    Fly,

    Wind,

    Burn, // fire

    Rise, // bioSoup.particle

    //-- playerGoCircle --
    SelfRotate,

    //--- UI-go 临时使用的 几个值 --
    New,
    Data,
    Pointer,

    //...


};


AnimActionEName str_2_animActionEName( const std::string &str_ )noexcept;
std::string animActionEName_2_str( AnimActionEName a_ )noexcept;


#endif 

