/*
 * =================== GoSpecFromJson.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoSpecFromJson.h"



std::unordered_map<goSpecId_t, std::unique_ptr<GoSpecFromJson>> GoSpecFromJson::dataUPtrs {};

