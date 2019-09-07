/*
 * ========================= Behaviour.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    GameObj 是游戏中的 一等公民。
 *    可以作为一个 独立的单位，存在于 游戏中
 * ----------------------------
 */
#ifndef TPR_BEHAVIOUR_H
#define TPR_BEHAVIOUR_H

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

    //------------- sign up -------------//
    inline void signUp_Awakes( F_V_V func_ )noexcept{ 
        Awakes.push_back(func_);
    }

    inline void signUp_Starts( F_V_V func_ )noexcept{ 
        Starts.push_back(func_);
    }

    inline void signUp_Updates( F_V_V func_ )noexcept{ 
        Updates.push_back(func_);
    }

    //------------- call -------------//
    inline void call_Awakes()noexcept{
        call_funcs( Awakes );
    }

    inline void call_Starts()noexcept{
        call_funcs( Starts );
    }

    inline void call_Updates()noexcept{
        call_funcs( Updates );
    }

private:
    inline void call_funcs( const std::vector<F_V_V> &funcs_ )noexcept{
        if( funcs_.size() == 0 ){
            return;
        }
        for( const auto &i : funcs_ ){
            if( i != nullptr ){
                i();
            }
        }
    }

    //======== vals ========//
    std::vector<F_V_V> Awakes  {}; //- 在引擎运作之前
    std::vector<F_V_V> Starts  {}; //- 在主循环开启之前
    std::vector<F_V_V> Updates {}; //- 在主循环的每一回合中
                                   //  这个 Updates 是每帧执行一次，还是 每n帧 执行一次
                                   //  目前还没有想好 
};


#endif
