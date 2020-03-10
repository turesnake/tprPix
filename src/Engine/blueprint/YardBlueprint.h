/*
 * ======================= YardBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_YARD_BLUE_PRINT_H
#define TPR_YARD_BLUE_PRINT_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "GameObjType.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "fieldKey.h"

#include "FloorGoType.h"
#include "PlotBlueprint.h"
#include "blueprint_oth.h"
#include "ID_Manager.h"




namespace blueprint {//------------------ namespace: blueprint start ---------------------//



using varTypeDatas_Yard_MajorGoId_t = uint32_t;

class VarTypeDatas_Yard_MajorGo{
public:
    VarTypeDatas_Yard_MajorGo()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_isPlotBlueprint( bool b_ )noexcept{ this->isPlotBlueprint = b_; }

    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_, size_t num_ )noexcept{ 
        varTypeDatas_Yard_MajorGoId_t id = VarTypeDatas_Yard_MajorGo::id_manager.apply_a_u32_id();// 盲目分配id
        auto [insertIt, insertBool] = this->goSpecPool.emplace( id, std::move( uptr_ ) ); 
        tprAssert( insertBool );
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




using varTypeDatas_Yard_FloorGoId_t = uint32_t;

class VarTypeDatas_Yard_FloorGo{
public:
    VarTypeDatas_Yard_FloorGo()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_floorGoSize( FloorGoSize size_ )noexcept{ this->floorGoSize = size_; }

    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_, size_t num_ )noexcept{ 
        varTypeDatas_Yard_FloorGoId_t id = VarTypeDatas_Yard_FloorGo::id_manager.apply_a_u32_id();// 盲目分配id
        auto [insertIt, insertBool] = this->goSpecPool.emplace( id, std::move(uptr_) ); 
        tprAssert( insertBool );
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
    using mapDataId_t = uint32_t;
    //---

    inline void insert_2_majorGo_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                                std::shared_ptr<VarTypeDatas_Yard_MajorGo> sptr_ )noexcept{
        auto [insertIt1, insertBool1] = this->majorGo_varTypeDatas.insert({ typeIdx_, std::move(sptr_) });
        tprAssert( insertBool1 );
        auto [insertIt2, insertBool2] = this->majorGo_varTypes.insert( typeIdx_ );
        tprAssert( insertBool2 );
    }

    inline void insert_2_floorGo_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                                std::shared_ptr<VarTypeDatas_Yard_FloorGo> sptr_ )noexcept{
        auto [insertIt1, insertBool1] = this->floorGo_varTypeDatas.insert({ typeIdx_, std::move(sptr_) });
        tprAssert( insertBool1 );
        auto [insertIt2, insertBool2] = this->floorGo_varTypes.insert( typeIdx_ );
        tprAssert( insertBool2 );
    }


    inline void set_isHaveMajorGos( bool b_ )noexcept{ this->isHaveMajorGos = b_; }
    inline void set_isHaveFloorGos( bool b_ )noexcept{ this->isHaveFloorGos = b_; }

    inline void set_sizeByField( IntVec2 sizeByMapEnt_ )noexcept{ 
        this->sizeByFild = sizeByMapEnt_2_yardSize( sizeByMapEnt_ );
    }


    void init_check()noexcept;

    inline YardSize get_yardSize()const noexcept{ return this->sizeByFild; }

    //- 仅用于 读取 json数据 时 -
    inline bool get_isHaveMajorGos()const noexcept{ return this->isHaveMajorGos; }
    inline bool get_isHaveFloorGos()const noexcept{ return this->isHaveFloorGos; }


    inline std::pair<mapDataId_t, MapData*> create_new_majorGo_mapData( size_t num_=1 )noexcept{
        
        mapDataId_t id = YardBlueprint::mapDataId_manager.apply_a_u32_id();
        this->majorGo_mapDataIds.insert( this->majorGo_mapDataIds.end(), num_, id );
        //---
        auto [insertIt, insertBool] = this->majorGo_mapDatas.emplace( id, MapData{} );
        tprAssert( insertBool );
        return { id, &(insertIt->second) };
    }
    inline std::pair<mapDataId_t, MapData*> create_new_floorGo_mapData( size_t num_=1 )noexcept{

        mapDataId_t id = YardBlueprint::mapDataId_manager.apply_a_u32_id();
        this->floorGo_mapDataIds.insert( this->floorGo_mapDataIds.end(), num_, id );
        //---
        auto [insertIt, insertBool] = this->floorGo_mapDatas.emplace( id, MapData{} );
        tprAssert( insertBool );
        return { id, &(insertIt->second) };
    }

    inline void bind_floorId_2_majorId( mapDataId_t majorId_, mapDataId_t floorId_ )noexcept{
        tprAssert( this->isHaveMajorGos && this->isHaveFloorGos );
        auto [insertIt, insertBool] = this->majorDataId_2_floorDataIds.emplace( majorId_, floorId_ );
        tprAssert( insertBool ); // Must
    }

    inline std::pair<mapDataId_t, const MapData*> get_binded_floorData( mapDataId_t majorId_ )const noexcept{
        tprAssert( this->majorDataId_2_floorDataIds.find(majorId_) != this->majorDataId_2_floorDataIds.end() );
        mapDataId_t id = this->majorDataId_2_floorDataIds.at( majorId_ );
        tprAssert( this->floorGo_mapDatas.find(id) != this->floorGo_mapDatas.end() );
        return { id, &this->floorGo_mapDatas.at(id) };
    }

    inline const std::set<VariableTypeIdx> &get_majorGo_varTypes()const noexcept{ return this->majorGo_varTypes; }
    inline const std::set<VariableTypeIdx> &get_floorGo_varTypes()const noexcept{ return this->floorGo_varTypes; }

    inline std::pair<mapDataId_t, const MapData*> apply_a_random_majorGo_mapData( size_t uWeight_ )const noexcept{
        mapDataId_t id = this->majorGo_mapDataIds.at( (uWeight_+86887311) % this->majorGo_mapDataIds.size() );
        return { id, &this->majorGo_mapDatas.at(id) };
    }
    inline std::pair<mapDataId_t, const MapData*> apply_a_random_floorGo_mapData( size_t uWeight_ )const noexcept{
        mapDataId_t id = this->floorGo_mapDataIds.at( (uWeight_ + 906117317) % this->floorGo_mapDataIds.size() );
        return { id, &this->floorGo_mapDatas.at(id) };
    }

    inline const VarTypeDatas_Yard_MajorGo *get_majorGo_varTypeDataPtr_Yard( VariableTypeIdx type_ )const noexcept{
        tprAssert( this->majorGo_varTypeDatas.find(type_) != this->majorGo_varTypeDatas.end() );
        return this->majorGo_varTypeDatas.at(type_).get();
    }

    inline const VarTypeDatas_Yard_FloorGo *get_floorGo_varTypeDataPtr_Yard( VariableTypeIdx type_ )const noexcept{
        tprAssert( this->floorGo_varTypeDatas.find(type_) != this->floorGo_varTypeDatas.end() );
        return this->floorGo_varTypeDatas.at(type_).get();
    }


    static inline std::unique_ptr<YardBlueprint> create_new_uptr()noexcept{
        std::unique_ptr<YardBlueprint> uptr ( new YardBlueprint() ); // can't use std::make_unique
        return uptr;
    }

private:
    YardBlueprint()=default;

    YardSize sizeByFild {}; // yard 尺寸，以 field 为单位

    // major-gos
    std::vector<mapDataId_t> majorGo_mapDataIds {}; // 实际分配池。元素个数可能大于 帧数
    std::unordered_map<mapDataId_t, MapData>majorGo_mapDatas {}; // 若干帧，每一帧数据 就是一份 分配方案 
    std::set<VariableTypeIdx> majorGo_varTypes {};
    std::unordered_map<VariableTypeIdx, std::shared_ptr<VarTypeDatas_Yard_MajorGo>> majorGo_varTypeDatas {};

    // floor-gos
    std::vector<mapDataId_t> floorGo_mapDataIds {}; // 实际分配池。元素个数可能大于 帧数
    std::unordered_map<mapDataId_t, MapData>floorGo_mapDatas {}; // 若干帧，每一帧数据 就是一份 分配方案
    std::set<VariableTypeIdx> floorGo_varTypes {};
    std::unordered_map<VariableTypeIdx, std::shared_ptr<VarTypeDatas_Yard_FloorGo>> floorGo_varTypeDatas {};

    //- 当同时拥有 majorGo/floorGo 数据时，两两元素是强关联的
    //  此时就可以用本容器，根据 majorDataId 来寻找 floorDataId
    std::unordered_map<mapDataId_t, mapDataId_t> majorDataId_2_floorDataIds {}; // {majorId, floorId}

    //- 至少有一个为 true
    bool isHaveMajorGos {};
    bool isHaveFloorGos {};

    //======= static =======//
    static ID_Manager  mapDataId_manager; // majorGo_mapDatas / floorGo_mapDatas 共用
};






/* "yardName" 索引到一个 set 实例
 * 再根据 label/labelId 索引到一个 yardId
 * ---
 * "yardLabel" 一种简陋的 type数据
 * 可以用 "" / "default" / "Default" / "DEFAULT" 来表示一种 默认类型 （它们指向同一份数据）
 */
class YardBlueprintSet{
    using yardLabelId_t = size_t; // std::hash
    using yardBlueprintSetId_t = size_t; // std::hash
public:
    using F_getYardId = std::function< std::optional<yardBlueprintId_t>(NineDirection)>;

    //===== static =====//
    static void init_for_static()noexcept;// MUST CALL IN MAIN !!! 
    static yardBlueprintId_t init_new_yard( const std::string &yardName_, 
                                            const std::string &yardLabel_,
                                            NineDirection      yardDir_ );

    static YardBlueprint &get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept;

    // 仅用于 json_ecoSysPlan.cpp
    inline static yardBlueprintId_t get_yardBlueprintId(const std::string &yardName_, 
                                                        const std::string &yardLabel_,
                                                        NineDirection      yardDir_ )noexcept{
        
        auto &innUMap = YardBlueprintSet::get_dir_2_yardId_umap( yardName_, yardLabel_ );
        tprAssert( innUMap.find(yardDir_) != innUMap.end() );
        return innUMap.at(yardDir_);
    }


    // 返回一个 函数指针，存储在 village 实例中，为了可以用它，配合 具体的 dir，来获取 yardId
    static F_getYardId getFunctor_getYardId(const std::string &yardName_, 
                                            const std::string &yardLabel_ )noexcept{

        return [ yardName_l=yardName_, yardLabel_l=yardLabel_ ]( NineDirection dir_ )->std::optional<yardBlueprintId_t>{
            auto &innUMap = YardBlueprintSet::get_dir_2_yardId_umap( yardName_l, yardLabel_l );
            return ( innUMap.find(dir_) == innUMap.end() ) ?
                        std::nullopt  :
                        std::optional<yardBlueprintId_t>{ innUMap.at(dir_) };
        };

    }

    
    inline static bool is_find_name( const std::string &yardName_, const std::string &yardLabel_ )noexcept{
        //--- yardName ---//
        yardBlueprintSetId_t setId = std::hash<std::string>{}( yardName_ );
        if( YardBlueprintSet::setUPtrs.find(setId) == YardBlueprintSet::setUPtrs.end() ){
            return false;
        }
        YardBlueprintSet &setRef = *(YardBlueprintSet::setUPtrs.at(setId));
        //--- yardLabel ---//
        std::string yardLabel = check_and_unify_default_labels(yardLabel_); // "_DEFAULT_"
        yardLabelId_t yardLabelId = std::hash<std::string>{}( yardLabel );
        return (setRef.yardIDs.find(yardLabelId) != setRef.yardIDs.end());
    }


private:
    YardBlueprintSet()=default;

    inline static std::unordered_map<NineDirection, yardBlueprintId_t> &
    get_dir_2_yardId_umap(  const std::string &yardName_, const std::string &yardLabel_ )noexcept{
        //--- yardName ---//
        yardBlueprintSetId_t setId = std::hash<std::string>{}( yardName_ );
        tprAssert( YardBlueprintSet::setUPtrs.find(setId) != YardBlueprintSet::setUPtrs.end() );
        YardBlueprintSet &setRef = *(YardBlueprintSet::setUPtrs.at(setId));
        //--- yardLabel ---//
        std::string yardLabel = check_and_unify_default_labels(yardLabel_); // "_DEFAULT_"
        yardLabelId_t yardLabelId = std::hash<std::string>{}( yardLabel );
        tprAssert( setRef.yardIDs.find(yardLabelId) != setRef.yardIDs.end() ); // Must existed
        return setRef.yardIDs.at( yardLabelId );
    }


    // 两层索引: yardLabel, dir 
    std::unordered_map<yardLabelId_t, std::unordered_map<NineDirection, yardBlueprintId_t>> yardIDs;


    //===== static =====//
    static ID_Manager  yardId_manager; // apply yardBlueprint id
    static std::unordered_map<yardBlueprintSetId_t, std::unique_ptr<YardBlueprintSet>> setUPtrs; // 真实资源
    static std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>> yardUPtrs; // 真实资源
};



void parse_yardJsonFiles();

void calc_yard_fieldKeys( std::unordered_set<fieldKey_t> &outContainer_,
                            IntVec2 yardMPos_,
                            YardSize sizeByFild_ )noexcept;



}//--------------------- namespace: blueprint end ------------------------//
#endif 

