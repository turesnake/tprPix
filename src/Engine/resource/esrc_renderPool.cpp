/*
 * ===================== esrc_renderPool.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_renderPool.h"

//-------------------- CPP --------------------//
#include "esrc_state.h"


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


void debug_for_renderPools()noexcept{

    tprDebug::console( 
        "renderPools:" \
        "\n  Opaque size: {0}" \
        "\n  Translucent size: {1}" \
        "\n  Shadow size: {2}",
        get_renderPool(RenderPoolType::Opaque).get_poolSize(),
        get_renderPool(RenderPoolType::Translucent).get_poolSize(),
        get_renderPool(RenderPoolType::Shadow).get_poolSize()
    );

}


RenderPool &get_renderPool( RenderPoolType type_ )noexcept{
    tprAssert( renderP_inn::renderPoolUPtrs.find(type_) != renderP_inn::renderPoolUPtrs.end() );
    return *(renderP_inn::renderPoolUPtrs.at(type_));
}

GroundRenderPool &get_groundRenderPool()noexcept{
    return *(renderP_inn::groundRenderPoolUPtr);
}


void clear_all_renderPool()noexcept{
    for( auto &pair : renderP_inn::renderPoolUPtrs ){
        pair.second->clear();
    }
    renderP_inn::groundRenderPoolUPtr->clear();
}


}//---------------------- namespace: esrc -------------------------//

