/*
 * ========================= esrc_eco.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_ECO_OBJ_H
#define TPR_ESRC_ECO_OBJ_H

//-------------------- CPP --------------------//
#include <unordered_map>
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "EcoObj.h"
#include "IntVec.h"
#include "sectionKey.h"
#include "EcoObj_ReadOnly.h"
#include "GoSpecData.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_ecoObjs()noexcept;

void atom_try_to_inert_and_init_a_ecoObj( sectionKey_t ecoObjKey_ )noexcept;

//-- 更加精细的 元素数据 只读访问 接口 [值传递] --
std::pair<occupyWeight_t, EcoObj_ReadOnly> atom_get_ecoObj_readOnly( sectionKey_t sectionkey_ )noexcept;

const GoSpecData &atom_ecoObj_apply_a_rand_goSpecData( sectionKey_t sectionkey_, size_t densityIdx_, double randV_ )noexcept;
double atom_ecoObj_get_applyPercent( sectionKey_t sectionkey_, Density density_ )noexcept;

colorTableId_t atom_ecoObj_get_colorTableId( sectionKey_t sectionkey_ )noexcept;

}//---------------------- namespace: esrc -------------------------//
#endif

