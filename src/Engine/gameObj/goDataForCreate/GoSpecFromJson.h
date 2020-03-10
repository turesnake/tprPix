/*
 * ====================== GoSpecFromJson.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_SPEC_FROM_JSON_H
#define TPR_GO_SPEC_FROM_JSON_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "GameObjType.h" 
#include "Move.h"
#include "PubBinary2.h"
#include "GoAltiRange.h"

#include "GoAssemblePlan.h"

#include "AnimActionEName.h"


class GameObj;


// 从 go.json 文件中都取得数据，被存储为 此格式
class GoSpecFromJson{
public:
    //========== Nested ==========//
    class MoveStateTable;

    //========== Self ==========//
    inline std::unordered_set<std::string> *get_afsNamesPtr()noexcept{ return &(this->afsNames); }

    inline void insert_2_ExtraPassableDogoSpeciesIds( const std::string &name_ )noexcept{
        std::hash<std::string> hasher;
        goSpeciesId_t id = static_cast<goSpeciesId_t>( hasher(name_) ); // size_t -> uint64_t
        this->extraPassableDogoSpeciesIds.insert( id );
    }

    inline void insert_2_lAltiRanges( GoAltiRangeLabel label_, GoAltiRange val_ )noexcept{
        auto [insertIt, insertBool] = this->lAltiRanges.emplace( label_, val_ );
        tprAssert( insertBool );
    }

    inline GoAltiRange get_lAltiRange( GoAltiRangeLabel label_ )const noexcept{
        tprAssert( this->lAltiRanges.find(label_) != this->lAltiRanges.end() );
        return this->lAltiRanges.at( label_ );
    }

    void init_check()noexcept;

    //======== vals ========//
    std::string         goSpeciesName {};
    goSpeciesId_t       speciesId {};
    GameObjFamily       family {};
    GameObjState        state  {};
    GameObjMoveState    moveState {};
    MoveType            moveType {};

    //----- bool -----//
    bool    isMoveCollide {};
    bool    isDoPass {};
    bool    isBePass {};

    //----- numbers -----//
    SpeedLevel   moveSpeedLvl {};
    double       alti   {};
    double       weight {};
    //...

    PubBinary2   pubBinary {};


    std::unique_ptr<GoAssemblePlanSet> goAssemblePlanSetUPtr {nullptr}; // 数据存储地
    std::unique_ptr<MoveStateTable> moveStateTableUPtr {nullptr};

            
    //======== static ========//
    static void assemble_2_newGo( goSpeciesId_t specID_, GameObj &goRef_ );
    static void check_all_extraPassableDogoSpeciesIds()noexcept;


    inline static GoSpecFromJson &create_new_goSpecFromJson( const std::string &name_ )noexcept{

        std::hash<std::string> hasher;
        goSpeciesId_t id = static_cast<goSpeciesId_t>( hasher(name_) ); // size_t -> uint64_t

        std::unique_ptr<GoSpecFromJson> uptr ( new GoSpecFromJson() ); // can't use std::make_unique
        auto [insertIt, insertBool] = GoSpecFromJson::dataUPtrs.emplace( id, std::move(uptr) );
        tprAssert( insertBool );
        GoSpecFromJson &ref = *(insertIt->second);
        //--
        ref.goSpeciesName = name_;
        ref.speciesId = id;
        GoSpecFromJson::insert_2_goSpeciesIds_names_containers( id, name_ );
        //--
        return ref;
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
        return (c.afsNames.find(name_) != c.afsNames.end());
    }

    inline static goSpeciesId_t str_2_goSpeciesId( const std::string &name_ ){
        tprAssert( GoSpecFromJson::names_2_ids.find(name_) != GoSpecFromJson::names_2_ids.end() );
        return GoSpecFromJson::names_2_ids.at(name_);
    }
    inline static const std::string &goSpeciesId_2_name( goSpeciesId_t id_ )noexcept{
        tprAssert( GoSpecFromJson::ids_2_names.find(id_) != GoSpecFromJson::ids_2_names.end() );
        return GoSpecFromJson::ids_2_names.at(id_);
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
        auto [insertIt, insertBool] = GoSpecFromJson::initFuncs.emplace( id, functor_ );
        tprAssert( insertBool );
    }

    

private:
    GoSpecFromJson()=default;
    
    inline static void insert_2_goSpeciesIds_names_containers( goSpeciesId_t id_, const std::string &name_ ){
        auto out1 = GoSpecFromJson::ids_2_names.emplace( id_, name_ );
        auto out2 = GoSpecFromJson::names_2_ids.emplace( name_, id_ );
        tprAssert( out1.second );
        tprAssert( out2.second );
    }

    //======== vals ========//
    std::unordered_set<std::string>                     afsNames {};
    std::unordered_map<GoAltiRangeLabel, GoAltiRange>   lAltiRanges {};

    //-- 当 isBePass == false 时，允许一组 特殊的 dogo，可以穿透本 bego
    // 正式使用时，只提供 只读指针
    std::set<goSpeciesId_t>    extraPassableDogoSpeciesIds {};


    //======== static ========//
    // 资源持续整个游戏生命期，不用释放
    static std::unordered_map<goSpeciesId_t, std::unique_ptr<GoSpecFromJson>> dataUPtrs;
    static std::unordered_map<goSpeciesId_t, std::string>  ids_2_names;
    static std::unordered_map<std::string, goSpeciesId_t>  names_2_ids;
    static std::unordered_map<goSpeciesId_t, F_GO_INIT>    initFuncs; 

};


class GoSpecFromJson::MoveStateTable{
public:
    MoveStateTable()=default;

    void init_check( const GoSpecFromJson *goSpecFromJsonPtr_ )noexcept;

    //--- vals ---//
    SpeedLevel minLvl {}; // include
    SpeedLevel maxLvl {}; // include

    std::map<SpeedLevel, std::pair<AnimActionEName,int>> table {}; // speedLvl, actionEName, timeStepOff
};




namespace json{//------------- namespace json ----------------
    void parse_goJsonFile();
}//------------- namespace json: end ----------------



#endif 

