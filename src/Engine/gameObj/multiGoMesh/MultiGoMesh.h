/*
 * ====================== MultiGoMesh.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MULTI_GO_MESH_H
#define TPR_MULTI_GO_MESH_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "ID_Manager.h" 



class GoMeshEnt{
public:
    std::string             animFrameSetName {};
    glm::dvec2              dposOff {}; // gomesh-dposoff based on go-dpos
    double                  zOff    {};
    AnimLabel               animLabel {};
};


using jsonGoMeshSetId_t = u32_t;

class GoMeshSet{
public:
    GoMeshSet()=default;
    std::vector<GoMeshEnt> gomeshs {};
    //======== static ========//
    static ID_Manager  id_manager;
};



// 从1 开始分配，如果传入 0，表示不属于任何 type
using multiGoMeshTypeId_t = size_t;
#define DEFAULT_MULTI_GO_MESH_TYPE 0 // Default
            // 这个数据，不能写在 json 文件内
            // 只能在 主程序中，作为函数参数


// 一种简化的策略
// 一个 go-action，可以由数个 gomesh-action 组合而成
//
class MultiGoMesh{
public:
    MultiGoMesh()=default;


    inline GoMeshSet &create_new_goMeshSet( multiGoMeshTypeId_t type_ )noexcept{
        //----- id -----//
        jsonGoMeshSetId_t id = GoMeshSet::id_manager.apply_a_u32_id();
        if( type_ != DEFAULT_MULTI_GO_MESH_TYPE ){
            auto outPair1 = this->ids.insert({ type_, std::vector<jsonGoMeshSetId_t>{} }); // insert or find
            outPair1.first->second.push_back(id);
        }else{
            // do nothing, MultiGoMeshType::Default not belong to any type
        }
        this->allIds.push_back(id); // always
        //----- instance -----//
        auto outPair = this->goMeshSetUPtrs.insert({ id, std::make_unique<GoMeshSet>() });
        tprAssert( outPair.second );
        //----- ret -----//
        return *(outPair.first->second);
    }



    inline const GoMeshSet &apply_a_goMeshSet( multiGoMeshTypeId_t type_, size_t randUVal_ )const noexcept{
        //----- id -----//
        size_t              idx {};
        jsonGoMeshSetId_t   id {};  
        if( type_ == DEFAULT_MULTI_GO_MESH_TYPE ){
            idx = (randUVal_ + 152375) % this->allIds.size(); // apply a id directly from all ids
            id = this->allIds.at( idx );
        }else{
            tprAssert( this->ids.find(type_) != this->ids.end() );
            idx = (randUVal_ + 152375) % this->ids.at(type_).size(); // from target idPool
            id = this->ids.at(type_).at( idx );
        }
        //--- ret ---//
        tprAssert( this->goMeshSetUPtrs.find(id) != this->goMeshSetUPtrs.end() );
        return *(this->goMeshSetUPtrs.at(id));
    }



    //======= static =======//


    // multiGoMesh-json 数据中，每个新类型，都被登记为一个 id 号
    inline static void insert_2_multiGoMeshTypeIds( const std::string &str_ )noexcept{
        tprAssert( str_ != "Default" ); // 保留值
        if( MultiGoMesh::str_2_multiGoMeshTypeIds.find(str_) == MultiGoMesh::str_2_multiGoMeshTypeIds.end() ){
            multiGoMeshTypeId_t id = MultiGoMesh::id_manager.apply_a_u32_id();
            auto outPair = MultiGoMesh::str_2_multiGoMeshTypeIds.insert({ str_, id });
            tprAssert( outPair.second );
        }
    }


    // 在程序体内，一律用 id 号来传递 
    static multiGoMeshTypeId_t str_2_multiGoMeshTypeId( const std::string &str_ )noexcept;


private:
    std::unordered_map<multiGoMeshTypeId_t, std::vector<jsonGoMeshSetId_t>> ids {};
    std::vector<jsonGoMeshSetId_t> allIds {};
    //---
    std::unordered_map<jsonGoMeshSetId_t, std::unique_ptr<GoMeshSet>> goMeshSetUPtrs {};


    //======= static =======//
    static ID_Manager  id_manager;
    static std::unordered_map<std::string, multiGoMeshTypeId_t> str_2_multiGoMeshTypeIds;
};


namespace json {//-------- namespace: json --------------//
    void parse_single_multiGoMeshJsonFile( const std::string &path_file_ );
}//------------- namespace: json end --------------//


#endif 

