/*
 * ========================= esrc_section.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_SECTION_H_
#define _TPR_ESRC_SECTION_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "Section.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

inline std::unordered_map<sectionKey_t, Section> sections {};

Section *insert_new_section( const IntVec2 &_anyMPos );
Section *insert_new_section( sectionKey_t _sectionkey );


inline bool find_from_sections( sectionKey_t _sectionKey ){
    return (esrc::sections.find(_sectionKey) != esrc::sections.end());
}

inline Section *get_sectionPtr( sectionKey_t _sectionkey ){
        assert( sections.find(_sectionkey) != sections.end() );//- tmp
    return static_cast<Section*>( &(sections.at(_sectionkey)) );
}

}//---------------------- namespace: esrc -------------------------//
#endif

