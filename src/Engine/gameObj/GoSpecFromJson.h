/*
 * ====================== GoSpecFromJson.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_SPEC_FROM_JSON_H
#define TPR_GO_SPEC_FROM_JSON_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <unordered_set>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjType.h" 
#include "Move.h"
#include "PubBinary2.h"


//--------------- Script ------------------//
#include "Script/json/json_multiGoMesh.h"


// 从 go.json 文件中都取得数据，被存储为 此格式
class GoSpecFromJson{
public:
    GoSpecFromJson()=default; // DO NOT CALL IT DIRECTLY!!!

    inline void insert_2_afsNames( const std::string &name_ )noexcept{
        auto outPair = this->afsNames.insert(name_);
        tprAssert( outPair.second );
    }

    //======== vals ========//
    std::string       goSpecName {};
    goSpecId_t        specID {};
    goid_t            parentID {};

    //----- enum -----//
    GameObjFamily       family {};
    GameObjState        state  {};
    GameObjMoveState    moveState {};
    MoveType            moveType {};

    //----- bool -----//
    bool    isTopGo  {};
    bool    isMoveCollide {};
    bool    isDoPass {};
    bool    isBePass {};

    //----- numbers -----//
    SpeedLevel  speedLvl {};
    double       alti   {};
    double       weight {};
    //...

    PubBinary2   pubBinary {};

    //----- afs / gomeshs -----//
    std::unique_ptr<json::Json_MultiGoMesh> multiGoMeshUPtr {nullptr}; // 将被废弃 ...


    //======== static ========//
    inline static GoSpecFromJson &create_new_goSpecFromJson( goSpecId_t id_ )noexcept{
        auto outPair = GoSpecFromJson::dataUPtrs.insert({ id_, std::make_unique<GoSpecFromJson>() });
        tprAssert( outPair.second );
        return *(outPair.first->second);
    }
    
    inline static GoSpecFromJson &getnc_goSpecFromJsonRef( goSpecId_t id_ )noexcept{
        tprAssert( GoSpecFromJson::dataUPtrs.find(id_) != GoSpecFromJson::dataUPtrs.end() );
        return *(GoSpecFromJson::dataUPtrs.at(id_));
    }

    // support multi-thread
    inline static const GoSpecFromJson &get_goSpecFromJsonRef( goSpecId_t id_ )noexcept{
        tprAssert( GoSpecFromJson::dataUPtrs.find(id_) != GoSpecFromJson::dataUPtrs.end() );
        return *(GoSpecFromJson::dataUPtrs.at(id_));
    }

    inline static bool is_find_in_afsNames( goSpecId_t id_, const std::string &name_ )noexcept{
        const auto &c = GoSpecFromJson::get_goSpecFromJsonRef(id_);
        return c.is_find_in_afsNames(name_);
    }


private:
    inline bool is_find_in_afsNames( const std::string &name_ )const noexcept{
        return (this->afsNames.find(name_) != this->afsNames.end());
    }

    //======== vals ========//
    std::unordered_set<std::string> afsNames {};

    //======== static ========//
    // 资源持续整个游戏生命期，不用释放
    static std::unordered_map<goSpecId_t, std::unique_ptr<GoSpecFromJson>> dataUPtrs;
};





#endif 

