/*
 * ========================= esrc_field.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_FIELD_H_
#define TPR_ESRC_FIELD_H_

//-------------------- CPP --------------------//
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "MapField.h"
#include "fieldKey.h"
#include "MapFieldData_In_ChunkBuild.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


void atom_try_to_insert_and_init_the_field_ptr( const IntVec2 &fieldMPos_ );
void atom_field_reflesh_min_and_max_altis(fieldKey_t fieldKey_, const MapAltitude &alti_ );

void atom_field_set_nodeAlti_2( fieldKey_t fieldKey_, const std::vector<MemMapEnt> &chunkMapEnts_ );

const std::pair<occupyWeight_t, MapFieldData_In_ChunkBuild> atom_get_mapFieldData_in_chunkBuild( fieldKey_t fieldKey_ );
void atom_create_a_go_in_field( fieldKey_t fieldKey_ );


//-- tmp，仅用于 debug，在未来，要被删除
const MapField &atom_get_field( fieldKey_t fieldKey_ );   // tmp......


}//---------------------- namespace: esrc -------------------------//
#endif

