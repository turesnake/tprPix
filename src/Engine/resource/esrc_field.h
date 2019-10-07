/*
 * ========================= esrc_field.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_FIELD_H
#define TPR_ESRC_FIELD_H

//-------------------- CPP --------------------//
#include <utility> //- pair
#include <memory>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "MapField.h"
#include "fieldKey.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_fields();

void move_fieldUPtrs( std::unordered_map<fieldKey_t, std::unique_ptr<MapField>> &fields_ );

void erase_all_fields_in_chunk( IntVec2 chunkMPos_ );

const MapField &get_field( fieldKey_t fieldKey_ );


}//---------------------- namespace: esrc -------------------------//
#endif

