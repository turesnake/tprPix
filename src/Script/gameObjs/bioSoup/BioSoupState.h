/*
 * ====================== BioSoupState.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BIO_SOUP_STATE_H
#define TPR_BIO_SOUP_STATE_H

//-------------------- Engine --------------------//
#include "MapAltitude.h"

namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------

enum class State{
    NotExist, //  不存在
    Active,   //  激活
    Inertia,  //  惰性
};

inline State calc_bioSoupState_by_mapAlti( MapAltitude mapAlti_ ){
    if(         mapAlti_.get_val() < -23.0 ){   return State::Inertia;
    }else if(   mapAlti_.get_val() < 0.0 ){     return State::Active;
    }else{                                      return State::NotExist;
    }
}


}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 

