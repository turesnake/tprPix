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

//-------------------- Engine --------------------//
#include "MapField.h"
#include "fieldKey.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_fields();

void atom_try_to_insert_and_init_the_field_ptr( IntVec2 fieldMPos_ );

void atom_write_2_field_from_jobData(   fieldKey_t fieldKey_, 
                                    sectionKey_t ecoObjkey_,
                                    colorTableId_t id_,
                                    Density d_,
                                    MapAltitude nodeAlti_,
                                    MapAltitude minAlti_,
                                    MapAltitude maxAlti_,
                                    bool        isCrossEcoObj_,
                                    bool        isCrossColorTable_ );

void atom_erase_all_fields_in_chunk( IntVec2 chunkMPos_ );

//-- tmp，仅用于 debug，在未来，要被删除
const MapField &atom_get_field( fieldKey_t fieldKey_ );   // tmp......


}//---------------------- namespace: esrc -------------------------//
#endif

