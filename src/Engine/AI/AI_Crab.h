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

#include "pch.h"

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "functorTypes.h"


class AI_Crab{
public:

    AI_Crab() = default;


    inline void init(  )noexcept{
        //this->goid = goid_;
    }


    //void logicUpdate();


    inline void bind_get_tmpVal_functor( const F_R_int &functor_ )noexcept{
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

