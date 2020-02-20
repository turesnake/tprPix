/*
 * ====================== GoAssembleData.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoAssembleData.h"

//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


//======== static ========//
ID_Manager  GoMeshSet::id_manager { ID_TYPE::U32, 0};


std::unordered_set<goLabelId_t>     GoAssembleData::goLabelIds {};
std::hash<std::string>              GoAssembleData::hasher {};
const size_t                         GoAssembleData::DEFAULT_MULTI_GO_MESH_TYPE { GoAssembleData::hasher("DEFAULT") };


