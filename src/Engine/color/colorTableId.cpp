/*
 * ===================== colorTableId.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "colorTableId.h"


//const colorTableId_t NilColorTableId { std::hash<std::string>{}("DEFAULT") };
const colorTableId_t NilColorTableId { static_cast<colorTableId_t>(-1) };
        //-- never reach
