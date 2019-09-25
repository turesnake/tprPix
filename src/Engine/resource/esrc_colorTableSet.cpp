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
    std::unique_ptr<CurrentColorTable> currentColorTableUPtr {nullptr};

}//--------------------- namespace: cts_inn end -------------------------//


void init_colorTableSet()noexcept{
    cts_inn::colorTableSetUPtr = std::make_unique<ColorTableSet>();
    cts_inn::currentColorTableUPtr = std::make_unique<CurrentColorTable>();
}

ColorTableSet &get_colorTabelSet()noexcept{
    return *(cts_inn::colorTableSetUPtr.get());
}

CurrentColorTable &get_currentColorTabel()noexcept{
    return *(cts_inn::currentColorTableUPtr.get());
}


void rebind_currentColorTabel_target( colorTableId_t id_ )noexcept{
    const auto *ptr = cts_inn::colorTableSetUPtr->get_colorTablePtr(id_);
    cts_inn::currentColorTableUPtr->rebind_target( ptr );
};







}//---------------------- namespace: esrc -------------------------//

