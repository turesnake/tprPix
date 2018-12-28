/*
 * ========================= Behaviour.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    GameObj 是游戏中的 一等公民。
 *    可以作为一个 独立的单位，存在于 游戏中
 * ----------------------------
 */
#ifndef _TPR_BEHAVIOUR_H_
#define _TPR_BEHAVIOUR_H_


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional> 




//-- 设计思路类似 u3d 中的 MonoBehaviour/Behaviour, 但更为简单 --
//  本类只有一个 全局变量 
class Behaviour{

    using F_V_V = std::function<void()>;
public:
    Behaviour() = default;

    //- 将一个函数 注册到 awake函数集中
    inline void signUp_AwakeFuncs( F_V_V _func ){ 
        AwakeFuncs.push_back(_func);
    }

    //- 依次调用 awake函数集中的所有函数
    inline void call_AwakeFuncs(){
        for( const auto &i : AwakeFuncs ){
            if( i != nullptr ){
                i();
            }
        }
    }


private:
    //----------------------
    std::vector<F_V_V> AwakeFuncs {};  //- 在 引擎运作之前被call 的函数们。
    

};






#endif
