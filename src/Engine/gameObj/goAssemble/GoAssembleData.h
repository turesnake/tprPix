/*
 * ===================== GoAssembleData.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_ASSEMBLE_DATA_H
#define TPR_GO_ASSEMBLE_DATA_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional> // hash

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "ID_Manager.h" 
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "AnimActionEName.h"
#include "GoAltiRange.h"



class GoMeshEnt{
public:
    std::string             goMeshName {}; // 具象go类中 登记的名字
    std::string             animFrameSetName {};
    glm::dvec2              dposOff {}; // gomesh-dposoff based on go-dpos
    double                  zOff    {};
    AnimLabel               animLabel {};
    AnimActionEName         animActionEName {};
    
    //NineDirection           dir {};
    //BrokenLvl               brokenLvl {};
                                // dir / broken 数据，是作为 go的数据，从蓝图中被读取的
                                // 在完善的设计中，每一个 gomesh，都可以选择，直接使用 go的这份数据
                                // 或者 在 json 文件中，自定义这部分数据
                                // ...
};


using jsonGoMeshSetId_t = uint32_t;



// 一个 go 实例，由数个 gomesh 以及其他数据 组合而成
class GoMeshSet{
public:
    GoMeshSet()=default;
    std::vector<GoMeshEnt> gomeshs {};

    GoAltiRangeLabel goAltiRangeLabel {};

    //======== static ========//
    static ID_Manager  id_manager;
};



// 从1 开始分配，如果传入 0，表示不属于任何 type
using goLabelId_t = size_t; // std::hash

// 一个 go-action，可以由数个 gomesh-action 组合而成
//
class GoAssembleData{
public:
    GoAssembleData()=default;


    inline GoMeshSet &create_new_goMeshSet( goLabelId_t labelId_ )noexcept{
        //----- id -----//
        jsonGoMeshSetId_t id = GoMeshSet::id_manager.apply_a_u32_id();
        if( labelId_ != DEFAULT_MULTI_GO_MESH_TYPE ){
            auto outPair1 = this->ids.insert({ labelId_, std::vector<jsonGoMeshSetId_t>{} }); // insert or find
            outPair1.first->second.push_back(id);
        }else{
            // do nothing, goLabelId::Default not belong to any type
        }
        this->allIds.push_back(id); // always
        //----- instance -----//
        auto outPair = this->goMeshSetUPtrs.insert({ id, std::make_unique<GoMeshSet>() });
        tprAssert( outPair.second );
        //----- ret -----//
        return *(outPair.first->second);
    }



    inline const GoMeshSet &apply_a_goMeshSet( goLabelId_t labelId_, size_t randUVal_ )const noexcept{
        //----- id -----//
        size_t              idx {};
        jsonGoMeshSetId_t   id {};  
        if( labelId_ == DEFAULT_MULTI_GO_MESH_TYPE ){
            idx = (randUVal_ + 152375) % this->allIds.size(); // apply a id directly from all ids
            id = this->allIds.at( idx );
        }else{
            tprAssert( this->ids.find(labelId_) != this->ids.end() );
            idx = (randUVal_ + 152375) % this->ids.at(labelId_).size(); // from target idPool
            id = this->ids.at(labelId_).at( idx );
        }
        //--- ret ---//
        tprAssert( this->goMeshSetUPtrs.find(id) != this->goMeshSetUPtrs.end() );
        return *(this->goMeshSetUPtrs.at(id));
    }



    //======= static =======//


    // json 数据中，每个新类型，都被登记为一个 id 号
    inline static void insert_2_goLabelIds( const std::string &str_ )noexcept{
        tprAssert( (str_!="DEFAULT") && (str_!="Default") ); // 保留值
        GoAssembleData::goLabelIds.insert( GoAssembleData::hasher(str_) ); // maybe
    }


    // 在程序体内，一律用 id 号来传递 
    static goLabelId_t str_2_goLabelId( const std::string &str_ )noexcept{
        if( (str_=="DEFAULT") || (str_=="Default") || (str_=="") ){ // 表示自己，不为任何类型
            return GoAssembleData::DEFAULT_MULTI_GO_MESH_TYPE;
        }
        size_t id = GoAssembleData::hasher(str_);
        tprAssert( GoAssembleData::goLabelIds.find(id) != GoAssembleData::goLabelIds.end() );
        return id;
    }


private:
    std::unordered_map<goLabelId_t, std::vector<jsonGoMeshSetId_t>> ids {};
    std::vector<jsonGoMeshSetId_t> allIds {};
    //---
    std::unordered_map<jsonGoMeshSetId_t, std::unique_ptr<GoMeshSet>> goMeshSetUPtrs {};


    //======= static =======//
    static std::unordered_set<goLabelId_t> goLabelIds;
                        // 制作成全局容器不够精确，假设某种 go，都含有 goLabel: "Sml"
                        // 另一种go，也想要检测 "Sml" 时，只能获知，总数据中含有，而不是，自身的数据中含有
                        // 先不做修改
                        // ...


    static std::hash<std::string> hasher;
    static const size_t DEFAULT_MULTI_GO_MESH_TYPE;

};


namespace json {//-------- namespace: json --------------//
    void parse_single_goAssembleDataJsonFile( const std::string &path_file_ );
}//------------- namespace: json end --------------//


#endif 

