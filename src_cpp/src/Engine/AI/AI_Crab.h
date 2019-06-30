/*
 * ========================= AI_Crab.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   螃蟹式 AI 
 * -----------------------
 */
#ifndef TPR_AI_CRAB_H
#define TPR_AI_CRAB_H


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


    inline void init(  ){
        //this->goid = goid_;
    }


    //void logicUpdate();


    inline void bind_get_tmpVal_functor( const F_1 &functor_ ){
        this->get_tmpVal_functor = functor_;
    }



private:

    //GameObj *goPtr {nullptr};
    //std::weak_ptr<GameObj> goWPtr {};
    //goid_t  goid {};
                //-- tmp 


    F_1 get_tmpVal_functor {nullptr};
    //int tmpCount {0}; 未被使用...


};










#endif 

