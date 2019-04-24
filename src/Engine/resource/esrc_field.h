/*
 * ========================= esrc_field.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_FIELD_H_
#define _TPR_ESRC_FIELD_H_

//-------------------- Engine --------------------//
#include "MapField.h"
#include "fieldKey.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//


void atom_try_to_insert_and_init_the_field_ptr( const IntVec2 &_fieldMPos );

MapField *atom_get_fieldPtr( fieldKey_t _fieldKey );



}//---------------------- namespace: esrc -------------------------//
#endif

