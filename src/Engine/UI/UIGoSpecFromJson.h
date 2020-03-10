/*
 * =================== UIGoSpecFromJson.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UI_GO_SPEC_FROM_JSON_H
#define TPR_UI_GO_SPEC_FROM_JSON_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//-------------------- CPP --------------------//
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <functional> // hash

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObjType.h" 
#include "Move.h"
#include "PubBinary2.h"



class GameObj;



class UIGoSpecFromJson{
public:

    std::string       goSpeciesName {};
    goSpeciesId_t     speciesId {};

    //----- enum -----//
    GameObjFamily     family {};

    //----- bool -----//
    //----- numbers -----//
    MoveType    moveType {};
    SpeedLevel  moveSpeedLvl {};
    //...

    //======== static ========//
    static void assemble_2_newUIGo( goSpeciesId_t specID_, GameObj &goRef_ );


    inline static UIGoSpecFromJson &create_new_UIGoSpecFromJson( const std::string &name_ ){

        std::hash<std::string> hasher;
        goSpeciesId_t id = static_cast<goSpeciesId_t>( hasher(name_) ); // size_t -> uint64_t

        std::unique_ptr<UIGoSpecFromJson> uptr ( new UIGoSpecFromJson() ); // can't use std::make_unique
        auto [insertIt, insertBool] = UIGoSpecFromJson::dataUPtrs.emplace( id, std::move(uptr) );
        tprAssert( insertBool );
        UIGoSpecFromJson &ref = *(insertIt->second);
        //---
        ref.goSpeciesName = name_;
        ref.speciesId = id;
        UIGoSpecFromJson::insert_2_uiGoSpeciesIds_names_containers( id, name_ );
        //---
        return ref;
    }


    inline static goSpeciesId_t str_2_uiGoSpeciesId( const std::string &name_ ){
        tprAssert( UIGoSpecFromJson::names_2_ids.find(name_) != UIGoSpecFromJson::names_2_ids.end() );
        return UIGoSpecFromJson::names_2_ids.at(name_);
    }


    inline static const UIGoSpecFromJson &get_uiGoSpecFromJsonRef( goSpeciesId_t id_ ){
        tprAssert( UIGoSpecFromJson::dataUPtrs.find(id_) != UIGoSpecFromJson::dataUPtrs.end() );
        return *(UIGoSpecFromJson::dataUPtrs.at(id_));
    }


    inline static bool find_from_initFuncs( goSpeciesId_t goSpeciesId_ ){
        return (UIGoSpecFromJson::initFuncs.find(goSpeciesId_) != UIGoSpecFromJson::initFuncs.end());
    }


    inline static void call_initFunc(  goSpeciesId_t id_, GameObj &goRef_, const DyParam &dyParams_  ){
        tprAssert( UIGoSpecFromJson::find_from_initFuncs(id_) );
        UIGoSpecFromJson::initFuncs.at(id_)( goRef_, dyParams_ );
    }


    inline static void insert_2_initFuncs( const std::string &goTypeName_, const F_GO_INIT &functor_ ){
        goSpeciesId_t id = UIGoSpecFromJson::str_2_uiGoSpeciesId( goTypeName_ );
        auto [insertIt, insertBool] = UIGoSpecFromJson::initFuncs.emplace( id, functor_ );
        tprAssert( insertBool );
    }


private:
    UIGoSpecFromJson()=default;

    inline static void insert_2_uiGoSpeciesIds_names_containers( goSpeciesId_t id_, const std::string &name_ ){
        auto out1 = UIGoSpecFromJson::ids_2_names.emplace( id_, name_ );
        auto out2 = UIGoSpecFromJson::names_2_ids.emplace( name_, id_ );
        tprAssert( out1.second );
        tprAssert( out2.second );
    }

    //======== static ========//
    // 资源持续整个游戏生命期，不用释放
    static std::unordered_map<goSpeciesId_t, std::unique_ptr<UIGoSpecFromJson>> dataUPtrs;
    static std::unordered_map<goSpeciesId_t, std::string> ids_2_names;
    static std::unordered_map<std::string, goSpeciesId_t> names_2_ids;
    static std::unordered_map<goSpeciesId_t, F_GO_INIT> initFuncs;

};



namespace json{//------------- namespace json ----------------
    void parse_uiGoJsonFile();
}//------------- namespace json: end ----------------



#endif 

