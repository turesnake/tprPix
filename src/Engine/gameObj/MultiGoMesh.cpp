/*
 * ====================== MultiGoMesh.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "MultiGoMesh.h"

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

ID_Manager                                              MultiGoMesh::id_manager { ID_TYPE::U32, 1}; // start from 1
std::unordered_map<std::string, multiGoMeshTypeId_t>    MultiGoMesh::str_2_multiGoMeshTypeIds {};




multiGoMeshTypeId_t MultiGoMesh::str_2_multiGoMeshTypeId( const std::string &str_ )noexcept{
    if( (str_=="Default") || (str_=="") ){ // 表示自己，不为任何类型
        return DEFAULT_MULTI_GO_MESH_TYPE;
    }
    //--
    if( MultiGoMesh::str_2_multiGoMeshTypeIds.find(str_) == MultiGoMesh::str_2_multiGoMeshTypeIds.end() ){
        cout << "str_ = " << str_ << endl;
    }
    tprAssert( MultiGoMesh::str_2_multiGoMeshTypeIds.find(str_) != MultiGoMesh::str_2_multiGoMeshTypeIds.end() );
    return MultiGoMesh::str_2_multiGoMeshTypeIds.at( str_ );
}


