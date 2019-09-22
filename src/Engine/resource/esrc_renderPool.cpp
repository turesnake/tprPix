/*
 * ===================== esrc_renderPool.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "esrc_renderPool.h"

//-------------------- CPP --------------------//
#include <unordered_map>
#include <memory>

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//

namespace renderP_inn {//-------- namespace: renderP_inn  --------------//

    std::unordered_map<RenderPoolType, std::unique_ptr<RenderPool>> renderPoolUPtrs {};

}//------------- namespace: renderP_inn end --------------//



void init_renderPools()noexcept{
    renderP_inn::renderPoolUPtrs.insert({ RenderPoolType::Opaque,          std::make_unique<RenderPool>(true) });
    renderP_inn::renderPoolUPtrs.insert({ RenderPoolType::Translucent,     std::make_unique<RenderPool>(false) });
    renderP_inn::renderPoolUPtrs.insert({ RenderPoolType::Shadow,          std::make_unique<RenderPool>(false) });// tmp
}


RenderPool &get_renderPool( RenderPoolType type_ )noexcept{
    tprAssert( renderP_inn::renderPoolUPtrs.find(type_) != renderP_inn::renderPoolUPtrs.end() );
    return *(renderP_inn::renderPoolUPtrs.at(type_).get());
}


void clear_all_renderPool()noexcept{
    for( auto &pair : renderP_inn::renderPoolUPtrs ){
        pair.second->clear();
    }
}



}//---------------------- namespace: esrc -------------------------//

