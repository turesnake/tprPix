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

#include "esrc_state.h"

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//

namespace renderP_inn {//-------- namespace: renderP_inn  --------------//

    std::unordered_map<RenderPoolType, std::unique_ptr<RenderPool>> renderPoolUPtrs {};
    std::unique_ptr<GroundRenderPool> groundRenderPoolUPtr {nullptr};

}//------------- namespace: renderP_inn end --------------//


//-- Must after esrc::init_colorTableSet !!!
void init_renderPools()noexcept{

    tprAssert( esrc::is_setState("colorTableSet") );
    tprAssert( esrc::is_setState("json_colorTableSet") );

    renderP_inn::renderPoolUPtrs.insert({ RenderPoolType::Opaque,          std::make_unique<RenderPool>(true) });
    renderP_inn::renderPoolUPtrs.insert({ RenderPoolType::Translucent,     std::make_unique<RenderPool>(false) });
    renderP_inn::renderPoolUPtrs.insert({ RenderPoolType::Shadow,          std::make_unique<RenderPool>(false) });// tmp
    //---
    renderP_inn::groundRenderPoolUPtr = std::make_unique<GroundRenderPool>();

    esrc::insertState("renderPool");
}


RenderPool &get_renderPool( RenderPoolType type_ )noexcept{
    tprAssert( renderP_inn::renderPoolUPtrs.find(type_) != renderP_inn::renderPoolUPtrs.end() );
    return *(renderP_inn::renderPoolUPtrs.at(type_).get());
}

GroundRenderPool &get_groundRenderPool()noexcept{
    return *(renderP_inn::groundRenderPoolUPtr.get());
}


void clear_all_renderPool()noexcept{
    for( auto &pair : renderP_inn::renderPoolUPtrs ){
        pair.second->clear();
    }
    renderP_inn::groundRenderPoolUPtr->clear();
}


}//---------------------- namespace: esrc -------------------------//

