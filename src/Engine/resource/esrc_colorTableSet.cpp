/*
 * ====================== esrc_colorTableSet.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_colorTableSet.h"
#include <memory>


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace cts_inn {//------------------ namespace: cts_inn -------------------------//

    std::unique_ptr<ColorTableSet> colorTableSetUPtr {nullptr};

}//--------------------- namespace: cts_inn end -------------------------//


void init_colorTableSet()noexcept{
    cts_inn::colorTableSetUPtr = std::make_unique<ColorTableSet>();
}

ColorTableSet &get_colorTabelSet()noexcept{
        //tprAssert( cts_inn::colorTableSetUPtr );// tmp
    return *(cts_inn::colorTableSetUPtr.get());
}


}//---------------------- namespace: esrc -------------------------//

