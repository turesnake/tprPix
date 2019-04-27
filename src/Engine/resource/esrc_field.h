/*
 * ========================= esrc_field.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_FIELD_H_
#define _TPR_ESRC_FIELD_H_

//-------------------- CPP --------------------//
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "MapField.h"
#include "fieldKey.h"
#include "MapFieldData_In_ChunkBuild.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//


void atom_try_to_insert_and_init_the_field_ptr( const IntVec2 &_fieldMPos );
void atom_field_reflesh_altis(fieldKey_t _fieldKey, const Altitude &_alti, const IntVec2 &_pixMPos );


const std::pair<occupyWeight_t, MapFieldData_In_ChunkBuild> atom_get_mapFieldData_in_chunkBuild( fieldKey_t _fieldKey );
void atom_create_a_go_in_field( fieldKey_t _fieldKey );


}//---------------------- namespace: esrc -------------------------//
#endif

