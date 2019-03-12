/*
 * =================== fieldBorderEntPixMaskSet_mem.cpp =======
 *                          -- tpr --
 *                                        CREATE -- 2019.03.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  fieldBorderEntPixMaskSet
 * ----------------------------
 */

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源

//#include "debug.h" //- tmp


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *            init_fieldBorderEntPixMaskSet
 * -----------------------------------------------------------
 */
void init_fieldBorderEntPixMaskSet(){

    esrc::fieldBorderEntPixMaskSet.init( "/fieldBorderEntPixMaskSet.png",
                                            IntVec2{16,7},
                                            16*7 );

}


}//---------------------- namespace: esrc -------------------------//


