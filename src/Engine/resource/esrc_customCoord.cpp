/*
 * ========================= esrc_customCoord.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.15
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_customCoord.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <memory>

//-------------------- Engine --------------------//
#include "esrc_state.h"



namespace esrc {//------------------ namespace: esrc -------------------------//

namespace customCoord_inn {//-------- namespace: customCoord_inn --------------//
    
    std::unique_ptr<CustomCoord> artifactCoordUPtr {nullptr};

}//------------- namespace: customCoord_inn end --------------//


void init_customCoord(){

    //-- 目前唯一的 客制坐标系，用来 摆放 人造物go
    //   一种 45度斜坐标系
    double radian = atan( 649.0 / 848.0 ); //- 外部测量得值
    double xVal = 1.0 * cos( radian );
    double yVal = 1.0 * sin( radian );
    glm::dvec2 xP { xVal, -yVal };
    glm::dvec2 yP { xVal,  yVal };
    //---
    customCoord_inn::artifactCoordUPtr = std::make_unique<CustomCoord>( xP, yP );
    //---
    esrc::insertState("customCoord");
}


const CustomCoord &get_artifactCoordRef()noexcept{
    return *(customCoord_inn::artifactCoordUPtr);
}





}//---------------------- namespace: esrc -------------------------//

