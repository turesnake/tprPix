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


enum class BioSoupState{
    NotExist, //  不存在
    Active,   //  激活
    Inertia,  //  惰性
};



inline BioSoupState calc_bioSoupState_by_mapAlti( MapAltitude mapAlti_ ){
    if(         mapAlti_.val < -13.0 ){ return BioSoupState::Inertia;
    }else if(   mapAlti_.val < 13.0 ){  return BioSoupState::Active;
    }else{                              return BioSoupState::NotExist;
    }
}



#endif 

