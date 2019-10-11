/*
 * ==================== json_multiGoMesh.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JSON_MULTI_GO_MESH_H
#define TPR_JSON_MULTI_GO_MESH_H
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


namespace json{//------------- namespace json ----------------

enum class MultiGoMeshType {
    Default, // not belong to any type

    Mid  // tmp
    //...
};


inline MultiGoMeshType str_2_multiGoMeshType( const std::string &str_ )noexcept{
    if( (str_=="") || (str_=="Default") ){  return MultiGoMeshType::Default;
    }else if( str_ == "Mid" ){              return MultiGoMeshType::Mid;
    }
    //...
    else{
        tprAssert(0);
        return MultiGoMeshType::Default; // never reach
    }
}



class Json_GoMesh{
public:
    std::string             animFrameSetName {};
    glm::dvec2              dposOff {}; // gomesh-dposoff based on go-dpos
    std::vector<AnimLabel>  animLabels {};
};




using jsonGoMeshSetId_t = u32_t;

class Json_GoMeshSet{
public:
    Json_GoMeshSet()=default;
    std::vector<Json_GoMesh> gomeshs {};
    //======== static ========//
    static ID_Manager  id_manager;
};




class Json_MultiGoMesh{
public:
    Json_MultiGoMesh()=default;


    inline Json_GoMeshSet &create_new_json_goMeshSet( MultiGoMeshType type_ )noexcept{
        //----- id -----//
        jsonGoMeshSetId_t id = Json_GoMeshSet::id_manager.apply_a_u32_id();
        if( type_ != MultiGoMeshType::Default ){
            if( this->ids.find(type_) == this->ids.end() ){
                this->ids.insert({ type_, std::vector<jsonGoMeshSetId_t>{} });
            }
            this->ids.at(type_).push_back(id);
        }else{
            // do nothing, MultiGoMeshType::Default not belong to any type
        }
        this->allIds.push_back(id); // always
        //----- instance -----//
        auto outPair = this->goMeshSetUPtrs.insert({ id, std::make_unique<Json_GoMeshSet>() });
        tprAssert( outPair.second );
        //----- ret -----//
        return *(outPair.first->second);
    }


    inline const Json_GoMeshSet &apply_a_json_goMeshSet( MultiGoMeshType type_, size_t randUVal_ )const noexcept{
        //----- id -----//
        size_t              idx {};
        jsonGoMeshSetId_t   id {};  
        if( type_ == MultiGoMeshType::Default ){
            idx = (randUVal_ * 9 + 52375) % this->allIds.size(); // apply a id directly from all ids
            id = this->allIds.at( idx );
        }else{
            tprAssert( this->ids.find(type_) != this->ids.end() );
            idx = (randUVal_ * 9 + 52375) % this->ids.at(type_).size(); // from target idPool
            id = this->ids.at(type_).at( idx );
        }
        //--- ret ---//
        tprAssert( this->goMeshSetUPtrs.find(id) != this->goMeshSetUPtrs.end() );
        return *(this->goMeshSetUPtrs.at(id));
    }


private:
    std::unordered_map<MultiGoMeshType, std::vector<jsonGoMeshSetId_t>> ids {};
    std::vector<jsonGoMeshSetId_t> allIds {};
    //---
    std::unordered_map<jsonGoMeshSetId_t, std::unique_ptr<Json_GoMeshSet>> goMeshSetUPtrs {};
};


}//------------- namespace json: end ----------------
#endif 

