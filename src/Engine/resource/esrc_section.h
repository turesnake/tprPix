/*
 * ========================= esrc_section.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_SECTION_H_
#define _TPR_ESRC_SECTION_H_

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "Section.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//



Section *atom_insert_and_init_new_section( sectionKey_t _sectionKey );
bool atom_find_from_sections( sectionKey_t _sectionKey );
Section *atom_get_sectionPtr( sectionKey_t _sectionkey );


}//---------------------- namespace: esrc -------------------------//
#endif

