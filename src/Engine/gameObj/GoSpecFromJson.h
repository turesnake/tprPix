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
#include <set>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjType.h" 
#include "Move.h"
#include "PubBinary2.h"

#include "MultiGoMesh.h"


class GameObj;


// 从 go.json 文件中都取得数据，被存储为 此格式
class GoSpecFromJson{
public:
    GoSpecFromJson()=default; // DO NOT CALL IT DIRECTLY!!!

    inline std::unordered_set<std::string> *get_afsNamesPtr()noexcept{ return &(this->afsNames); }

    inline void insert_2_ExtraPassableDogoSpeciesNames( const std::string &name_ )noexcept{
        this->extraPassableDogoSpeciesNames.push_back(name_);
    }


    //======== vals ========//
    std::string       goSpeciesName {};
    goSpeciesId_t     speciesId {};

    //----- enum -----//
    GameObjFamily       family {};
    GameObjState        state  {};
    GameObjMoveState    moveState {};
    MoveType            moveType {};

    //----- bool -----//
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
    std::unique_ptr<MultiGoMesh> multiGoMeshUPtr {nullptr}; // 数据存储地
    
            

    //======== static ========//
    static void assemble_2_newGo( goSpeciesId_t specID_, GameObj &goRef_ );
    static void convert_all_extraPassableDogoSpeciesNames_2_goSpeciesIds()noexcept;


    inline static GoSpecFromJson &create_new_goSpecFromJson( goSpeciesId_t id_ )noexcept{
        auto outPair = GoSpecFromJson::dataUPtrs.insert({ id_, std::make_unique<GoSpecFromJson>() });
        tprAssert( outPair.second );
        return *(outPair.first->second);
    }
    
    inline static GoSpecFromJson &getnc_goSpecFromJsonRef( goSpeciesId_t id_ )noexcept{
        tprAssert( GoSpecFromJson::dataUPtrs.find(id_) != GoSpecFromJson::dataUPtrs.end() );
        return *(GoSpecFromJson::dataUPtrs.at(id_));
    }

    // support multi-thread
    inline static const GoSpecFromJson &get_goSpecFromJsonRef( goSpeciesId_t id_ )noexcept{
        tprAssert( GoSpecFromJson::dataUPtrs.find(id_) != GoSpecFromJson::dataUPtrs.end() );
        return *(GoSpecFromJson::dataUPtrs.at(id_));
    }

    inline static bool is_find_in_afsNames( goSpeciesId_t id_, const std::string &name_ )noexcept{
        const auto &c = GoSpecFromJson::get_goSpecFromJsonRef(id_);
        return c.is_find_in_afsNames(name_);
    }

    inline static void insert_2_goSpeciesIds_names_containers( goSpeciesId_t id_, const std::string &name_ ){
        auto out1 = GoSpecFromJson::ids_2_names.insert({ id_, name_ });
        auto out2 = GoSpecFromJson::names_2_ids.insert({ name_, id_ });
        tprAssert( out1.second );
        tprAssert( out2.second );
    }

    inline static goSpeciesId_t str_2_goSpeciesId( const std::string &name_ ){
        tprAssert( GoSpecFromJson::names_2_ids.find(name_) != GoSpecFromJson::names_2_ids.end() );
        return GoSpecFromJson::names_2_ids.at(name_);
    }

    inline static bool find_from_initFuncs( goSpeciesId_t goSpeciesId_ ){
        return ( GoSpecFromJson::initFuncs.find(goSpeciesId_) != GoSpecFromJson::initFuncs.end());
    }

    inline static void call_initFunc(  goSpeciesId_t id_, GameObj &goRef_, const DyParam &dyParams_  ){
        tprAssert( GoSpecFromJson::find_from_initFuncs(id_) );
        GoSpecFromJson::initFuncs.at(id_)( goRef_, dyParams_ );
    }

    inline static void insert_2_initFuncs( const std::string &goTypeName_, const F_GO_INIT &functor_ ){
        goSpeciesId_t id = GoSpecFromJson::str_2_goSpeciesId( goTypeName_ );
        auto outPair = GoSpecFromJson::initFuncs.insert({ id, functor_ });
        tprAssert( outPair.second );
    }

private:
    inline bool is_find_in_afsNames( const std::string &name_ )const noexcept{
        return (this->afsNames.find(name_) != this->afsNames.end());
    }


    //======== vals ========//
    std::unordered_set<std::string> afsNames {};


    //-- 当 isBePass == false 时，允许一组 特殊的 dogo，可以穿透本 bego
    // 从 json文件中，读取时，暂时纯粹为 string 形态
    // 最后 统计转换为 goSpeciesId 数据 （然后 string 容器被清空）
    // 正式使用时，只提供 只读指针
    std::vector<std::string> extraPassableDogoSpeciesNames {};
    std::set<goSpeciesId_t>    extraPassableDogoSpeciesIds {};


    //======== static ========//
    // 资源持续整个游戏生命期，不用释放
    static std::unordered_map<goSpeciesId_t, std::unique_ptr<GoSpecFromJson>> dataUPtrs;
    static std::unordered_map<goSpeciesId_t, std::string>  ids_2_names;
    static std::unordered_map<std::string, goSpeciesId_t>  names_2_ids;
    static std::unordered_map<goSpeciesId_t, F_GO_INIT>    initFuncs; 

};



namespace json{//------------- namespace json ----------------
    void parse_goJsonFile();
}//------------- namespace json: end ----------------



#endif 

