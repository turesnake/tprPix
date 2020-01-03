/*
 * ======================= YardBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_YARD_BLUE_PRINT_H
#define TPR_YARD_BLUE_PRINT_H

//-------------------- CPP --------------------//
#include <unordered_set>

//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "fieldKey.h"

#include "PlotBlueprint.h"

#include "blueprint_oth.h"

#include "ID_Manager.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/floorGos/FloorGoSize.h" // tmp


namespace blueprint {//------------------ namespace: blueprint start ---------------------//



using varTypeDatas_Yard_MajorGoId_t = u32_t;

class VarTypeDatas_Yard_MajorGo{
public:
    VarTypeDatas_Yard_MajorGo()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_isPlotBlueprint( bool b_ )noexcept{ this->isPlotBlueprint = b_; }

    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_, size_t num_ )noexcept{ 
        varTypeDatas_Yard_MajorGoId_t id = VarTypeDatas_Yard_MajorGo::id_manager.apply_a_u32_id();// 盲目分配id
        auto outPair = this->goSpecPool.insert({ id, std::move( uptr_ ) }); 
        tprAssert( outPair.second );
        //---
        this->goSpecRandPool.insert( this->goSpecRandPool.end(), num_, id );
    }

    inline void insert_2_plotIds( plotBlueprintId_t id_, size_t num_ )noexcept{ 
        this->plotIds.insert( this->plotIds.end(), num_, id_ ); 
    }

    void init_check()noexcept;

    //----- get -----//
    inline bool get_isPlotBlueprint()const noexcept{ return this->isPlotBlueprint; }
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }

    inline const GoSpec &apply_rand_goSpec( size_t uWeight_ )const noexcept{
        varTypeDatas_Yard_MajorGoId_t id = this->goSpecRandPool.at( (uWeight_ + 7106177) % this->goSpecRandPool.size() );
        return *(this->goSpecPool.at(id));
    }

    inline const std::vector<plotBlueprintId_t> &get_plotIds()const noexcept{ return this->plotIds; }
private:
    //-- 以下 2组容器，只有一个被使用，要么是 goSpecs，要么是 plots
    std::vector<varTypeDatas_Yard_MajorGoId_t> goSpecRandPool {};
    std::unordered_map<varTypeDatas_Yard_MajorGoId_t, std::unique_ptr<GoSpec>> goSpecPool {};
    //---
    std::vector<plotBlueprintId_t>       plotIds {}; // 随机抽取池
    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    bool isPlotBlueprint {}; // 本变量是否为一个 plot 
    //======== static ========//
    static ID_Manager  id_manager;
};




using varTypeDatas_Yard_FloorGoId_t = u32_t;

class VarTypeDatas_Yard_FloorGo{
public:
    VarTypeDatas_Yard_FloorGo()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_floorGoSize( FloorGoSize size_ )noexcept{ this->floorGoSize = size_; }

    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_, size_t num_ )noexcept{ 
        varTypeDatas_Yard_FloorGoId_t id = VarTypeDatas_Yard_FloorGo::id_manager.apply_a_u32_id();// 盲目分配id
        auto outPair = this->goSpecPool.insert({ id, std::move(uptr_) }); 
        tprAssert( outPair.second );
        //---
        this->goSpecRandPool.insert( this->goSpecRandPool.end(), num_, id );
    }

    void init_check()noexcept;

    //----- get -----//
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }
    inline FloorGoSize get_floorGoSize()const noexcept{ return this->floorGoSize; }

    inline const GoSpec &apply_rand_goSpec( size_t uWeight_ )const noexcept{
        varTypeDatas_Yard_FloorGoId_t id = this->goSpecRandPool.at( (uWeight_ + 1076177) % this->goSpecRandPool.size() );
        return *(this->goSpecPool.at(id));
    }

private:
    std::vector<varTypeDatas_Yard_FloorGoId_t> goSpecRandPool {};
    std::unordered_map<varTypeDatas_Yard_FloorGoId_t, std::unique_ptr<GoSpec>> goSpecPool {};

    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    FloorGoSize floorGoSize {};
    //======== static ========//
    static ID_Manager  id_manager;
};








// 院子级蓝图。中间级别的蓝图，有数 fields 大
class YardBlueprint{
public:
    YardBlueprint()=default; // DO NOT CALL IT DIRECTLY!!!


    inline void insert_2_majorGo_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                                std::unique_ptr<VarTypeDatas_Yard_MajorGo> uptr_ )noexcept{
        auto outPair1 = this->majorGo_varTypeDatas.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( outPair1.second );
        auto outPair2 = this->majorGo_varTypes.insert( typeIdx_ );
        tprAssert( outPair2.second );
    }

    inline void insert_2_floorGo_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                                std::unique_ptr<VarTypeDatas_Yard_FloorGo> uptr_ )noexcept{
        auto outPair1 = this->floorGo_varTypeDatas.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( outPair1.second );
        auto outPair2 = this->floorGo_varTypes.insert( typeIdx_ );
        tprAssert( outPair2.second );
    }


    inline void set_isHaveMajorGos( bool b_ )noexcept{ this->isHaveMajorGos = b_; }
    inline void set_isHaveFloorGos( bool b_ )noexcept{ this->isHaveFloorGos = b_; }

    inline void set_sizeByField( IntVec2 sizeByMapEnt_ )noexcept{ 
        this->sizeByFild = sizeByMapEnt_2_yardSize( sizeByMapEnt_ );
    }

    inline void init_check()const noexcept{
        if( this->isHaveMajorGos ){
            tprAssert(  !this->majorGo_mapDatas.empty() &&
                        !this->majorGo_varTypes.empty() &&
                        !this->majorGo_varTypeDatas.empty() );
        }
        if( this->isHaveFloorGos ){
            tprAssert(  !this->floorGo_mapDatas.empty() &&
                        !this->floorGo_varTypes.empty() &&
                        !this->floorGo_varTypeDatas.empty() );
        }
    }

    inline YardSize get_yardSize()const noexcept{ return this->sizeByFild; }

    //- 仅用于 读取 json数据 时 -
    inline bool get_isHaveMajorGos()const noexcept{ return this->isHaveMajorGos; }
    inline bool get_isHaveFloorGos()const noexcept{ return this->isHaveFloorGos; }

    inline MapData &create_new_majorGo_mapData()noexcept{
        this->majorGo_mapDatas.push_back( MapData{} );
        return this->majorGo_mapDatas.back();
    }
    inline MapData &create_new_floorGo_mapData()noexcept{
        this->floorGo_mapDatas.push_back( MapData{} );
        return this->floorGo_mapDatas.back();
    }

    inline const std::set<VariableTypeIdx> &get_majorGo_varTypes()const noexcept{ return this->majorGo_varTypes; }
    inline const std::set<VariableTypeIdx> &get_floorGo_varTypes()const noexcept{ return this->floorGo_varTypes; }

    inline const MapData &apply_a_random_majorGo_mapData( size_t uWeight_ )const noexcept{
        return this->majorGo_mapDatas.at( (uWeight_ + 86887311) % this->majorGo_mapDatas.size() );
    }
    inline const MapData &apply_a_random_floorGo_mapData( size_t uWeight_ )const noexcept{
        return this->floorGo_mapDatas.at( (uWeight_ + 906117317) % this->floorGo_mapDatas.size() );
    }

    inline const VarTypeDatas_Yard_MajorGo *get_majorGo_varTypeDataPtr_Yard( VariableTypeIdx type_ )const noexcept{
        tprAssert( this->majorGo_varTypeDatas.find(type_) != this->majorGo_varTypeDatas.end() );
        return this->majorGo_varTypeDatas.at(type_).get();
    }

    inline const VarTypeDatas_Yard_FloorGo *get_floorGo_varTypeDataPtr_Yard( VariableTypeIdx type_ )const noexcept{
        tprAssert( this->floorGo_varTypeDatas.find(type_) != this->floorGo_varTypeDatas.end() );
        return this->floorGo_varTypeDatas.at(type_).get();
    }

private:
    YardSize sizeByFild {}; // yard 尺寸，以 field 为单位

    // major-gos
    std::vector<MapData> majorGo_mapDatas {};  // 若干帧，每一帧数据 就是一份 分配方案
    std::set<VariableTypeIdx> majorGo_varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Yard_MajorGo>> majorGo_varTypeDatas {};

    // floor-gos
    std::vector<MapData> floorGo_mapDatas {}; // 若干帧，每一帧数据 就是一份 分配方案
    std::set<VariableTypeIdx> floorGo_varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Yard_FloorGo>> floorGo_varTypeDatas {};

    //- 至少有一个为 true
    bool isHaveMajorGos {};
    bool isHaveFloorGos {};
};







/* "yardName" 索引到一个 set 实例
 * 再根据 label/labelId 索引到一个 yardId
 * ---
 * "yardLabel" 一种简陋的 type数据
 * 可以用 "" / "default" / "Default" / "DEFAULT" 来表示一种 默认类型 （它们指向同一份数据）
 */
class YardBlueprintSet{
public:
    YardBlueprintSet()=default;// DO NOT CALL IT DIRECTLY!!!

    //===== static =====//
    static void init_for_static()noexcept;
    static yardBlueprintId_t init_new_yard( const std::string &yardName_, const std::string &yardLabel_ );

    static YardBlueprint &get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept;

    inline static yardBlueprintId_t get_yardBlueprintId( const std::string &yardName_, const std::string &yardLabel_ )noexcept{
        //--- yardName ---//
        tprAssert( YardBlueprintSet::name_2_yardSetIds.find(yardName_) != YardBlueprintSet::name_2_yardSetIds.end() );
        yardBlueprintSetId_t setId = YardBlueprintSet::name_2_yardSetIds.at(yardName_);
        YardBlueprintSet &setRef = *(YardBlueprintSet::setUPtrs.at(setId));
        //--- yardLabel ---//
        std::string yardLabel = YardBlueprintSet::check_and_unify_default_labels(yardLabel_); // "_DEFAULT_"
        tprAssert( setRef.name_2_labelIds.find(yardLabel) != setRef.name_2_labelIds.end() );
        return setRef.yardIDs.at( setRef.name_2_labelIds.at(yardLabel) );
    }
    
    inline static bool is_find_name( const std::string &yardName_, const std::string &yardLabel_ )noexcept{
        //--- yardName ---//
        if( YardBlueprintSet::name_2_yardSetIds.find(yardName_) == YardBlueprintSet::name_2_yardSetIds.end() ){
            return false;
        }
        yardBlueprintSetId_t setId = YardBlueprintSet::name_2_yardSetIds.at(yardName_);
        YardBlueprintSet &setRef = *(YardBlueprintSet::setUPtrs.at(setId));
        //--- yardLabel ---//
        std::string yardLabel = YardBlueprintSet::check_and_unify_default_labels(yardLabel_); // "_DEFAULT_"
        return (setRef.name_2_labelIds.find(yardLabel) != setRef.name_2_labelIds.end());
    }


private:

    // "" / "Default", 将被统一替换为 "DEFAULT" 
    // 以此来表示，一种 默认 label 
    inline static std::string check_and_unify_default_labels( const std::string &yardLabel_ )noexcept{
        if( (yardLabel_=="") || 
            (yardLabel_=="default") ||
            (yardLabel_=="Default") ||
            (yardLabel_=="DEFAULT") ){
            return "_DEFAULT_";
        }else{
            return yardLabel_; // copy，无需考虑性能
        }
    }

    std::unordered_map<std::string, yardLabelId_t> name_2_labelIds {}; // 索引表
    std::unordered_map<yardLabelId_t, yardBlueprintId_t> yardIDs;

    //===== static =====//
    static ID_Manager  yardSetId_manager; // apply yardBlueprintSet id
    static ID_Manager  labelId_manager; // apply label id
    static ID_Manager  yardId_manager; // apply yardBlueprint id
    static std::unordered_map<std::string, yardBlueprintSetId_t> name_2_yardSetIds; // 索引表
    static std::unordered_map<yardBlueprintSetId_t, std::unique_ptr<YardBlueprintSet>> setUPtrs; // 真实资源
    static std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>> yardUPtrs; // 真实资源
};



void parse_yardJsonFiles();

void calc_yard_fieldKeys( std::unordered_set<fieldKey_t> &outContainer_,
                            IntVec2 yardMPos_,
                            YardSize sizeByFild_ )noexcept;



}//--------------------- namespace: blueprint end ------------------------//
#endif 

