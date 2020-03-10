/*
 * ====================== GoAssemblePlan.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GoAssemblePlan.h"

//--------------- CPP ------------------//
#include <utility>

//======== static ========//
ID_Manager  GoAssemblePlanSet::Plan::id_manager { ID_TYPE::U32, 0};


std::unordered_set<goLabelId_t>     GoAssemblePlanSet::goLabelIds {};
std::hash<std::string>              GoAssemblePlanSet::hasher {};
const size_t                         GoAssemblePlanSet::DEFAULT_GO_LABEL_ID { GoAssemblePlanSet::hasher("DEFAULT") };


