/*
 * ========================= sections_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <mutex>

//-------------------- Engine --------------------//
#include "esrc_section.h"
#include "config.h"
#include "sectionKey.h"

//#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

namespace{//------------ namespace --------------//

    std::unordered_map<sectionKey_t, Section> sections {};


    std::mutex   sectionsMutex;


}//---------------- namespace end --------------//



/* ===========================================================
 *            atom_insert_and_init_new_section
 * -----------------------------------------------------------
 * -- 创建 Section，将其彻底初始化，并 insert 进全局容器
 * -- 仅在 check_and_build_sections_3.cpp 文件中被使用
 */
Section *atom_insert_and_init_new_section( sectionKey_t _sectionKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    Section *sectionPtr;
    Section  section {};
    section.set_by_sectionKey( _sectionKey );
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(sectionsMutex);
            assert( esrc::sections.find(_sectionKey) == esrc::sections.end() );//- must not exist
        esrc::sections.insert({ _sectionKey, section }); //- copy
        sectionPtr = &(esrc::sections.at(_sectionKey));
    }
    sectionPtr->init();
    return sectionPtr;
}


/* ===========================================================
 *                   atom_find_from_sections
 * -----------------------------------------------------------
 */
bool atom_find_from_sections( sectionKey_t _sectionKey ){
    bool ret;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(sectionsMutex);
        ret = esrc::sections.find(_sectionKey) != esrc::sections.end();
    }
    return ret;
}


/* ===========================================================
 *                     atom_get_sectionPtr
 * -----------------------------------------------------------
 */
Section *atom_get_sectionPtr( sectionKey_t _sectionkey ){

    Section *sectionPtr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(sectionsMutex);
            assert( esrc::sections.find(_sectionkey) != esrc::sections.end() );//- tmp
        sectionPtr = &(esrc::sections.at(_sectionkey));
    }
    return sectionPtr;
}


}//---------------------- namespace: esrc -------------------------//

