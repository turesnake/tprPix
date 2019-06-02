/*
 * ========================= AI_Crab.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   螃蟹式 AI 
 * -----------------------
 */
#ifndef _TPR_AI_CRAB_H_
#define _TPR_AI_CRAB_H_


//-------------------- CPP --------------------//
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "GameObj.h"


class AI_Crab{
public:
    using F_1 = std::function<int()>;


    AI_Crab() = default;


    inline void init( GameObj *_goPtr ){
        this->goPtr = _goPtr;
    }


    //void logicUpdate();


    inline void bind_get_tmpVal_functor( const F_1 &_functor ){
        this->get_tmpVal_functor = _functor;
    }



private:

    GameObj *goPtr {nullptr};
    F_1 get_tmpVal_functor {nullptr};
    //int tmpCount {0}; 未被使用...


};










#endif 

