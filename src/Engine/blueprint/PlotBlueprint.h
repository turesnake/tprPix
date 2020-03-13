/*
 * ======================= PlotBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_PLOT_BLUE_PRINT_H
#define TPR_PLOT_BLUE_PRINT_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <variant>

//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "blueprintId.h"
#include "GameObjType.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "FloorGoType.h"
#include "ID_Manager.h"
#include "GoAssemblePlan.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


// 记录每个 mapent 上的数据，并不全，只有一部分，
// 剩余的信息，要通过 varType 获得
class MapDataEnt{
public:
    MapDataEnt()=default;

    //------- vals -------//
    VariableTypeIdx varTypeIdx {};
    IntVec2         mposOff {}; // based on left-bottom 
    BrokenLvl       brokenLvl {};
    NineDirection   direction {NineDirection::Center};  //- 角色 动画朝向
};


//- 一个实例，就是 1-frame png 数据，也就是一份 蓝图实例
class MapData{
public:
    std::vector<std::unique_ptr<MapDataEnt>> data {};
};



//- 确定一个 go 物种 的最基本数据
class GoSpec{
public:
    GoSpec()=default;

    //----- vals -----//
    goSpeciesId_t           goSpeciesId    {};
    std::string             afsName     {}; // 允许为 "", 此时 animLabel == ""
    std::string             animLabel   {};
    goLabelId_t             goLabelId {};
    bool                    isPlaceHolder {false}; // 单纯的占位元素，当从分配池中抽到 此类元素时，什么go也不生成
};


using varTypeDatas_PlotId_t = uint32_t;


// 蓝图中，每种变量，对应的一组数据
// 数据不是单一的，可以是一个随机池，从中选取一种
class VarTypeDatas_Plot{
public:
    VarTypeDatas_Plot()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }

    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_, size_t num_ )noexcept{
        varTypeDatas_PlotId_t id = VarTypeDatas_Plot::id_manager.apply_a_u32_id();// 盲目分配id
        auto [insertIt, insertBool] = this->goSpecPool.emplace( id, std::move(uptr_) ); 
        tprAssert( insertBool );
        //--
        this->randPool.insert( this->randPool.end(), num_, id );
    }

    void init_check()noexcept;

    //----- get -----//
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }

    inline const GoSpec &apply_rand_goSpec( size_t uWeight_ )const noexcept{
        varTypeDatas_PlotId_t id = this->randPool.at( (uWeight_ + 3556177) % this->randPool.size() );
        return *(this->goSpecPool.at(id));
    }

private:
    std::vector<varTypeDatas_PlotId_t> randPool {}; // 随机抽取池
    std::unordered_map<varTypeDatas_PlotId_t, std::unique_ptr<GoSpec>> goSpecPool {}; // 可有 1～n 种，禁止为 0 种

    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象

    //======== static ========//
    static ID_Manager  id_manager;
};



// 地块级蓝图。最小级别的蓝图，只有 数 mapents 大
// 
class PlotBlueprint{
public:

    inline void insert_2_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                        std::unique_ptr<VarTypeDatas_Plot> uptr_ )noexcept{
        auto [insertIt1, insertBool1] = this->varTypeDatas.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( insertBool1 );
        auto [insertIt2, insertBool2] = this->varTypes.insert( typeIdx_ );
        tprAssert( insertBool2 );
    }

    inline void set_sizeByMapEnt( IntVec2 val_ )noexcept{ this->sizeByMapEnt = val_; }

    inline void init_check()const noexcept{
        tprAssert( !this->mapDatas.empty() );
        tprAssert( !this->varTypes.empty() );
        tprAssert( !this->varTypeDatas.empty() );
    }

    //- 仅用于 读取 json数据 时 -
    inline std::vector<MapData> &getnc_mapDatasRef()noexcept{ return this->mapDatas; }

    inline const std::set<VariableTypeIdx> &get_varTypes()const noexcept{ return this->varTypes; }

    inline const MapData &apply_a_random_mapData( size_t uWeight_ )const noexcept{
        return this->mapDatas.at( (uWeight_ + 1844477191) % this->mapDatas.size() );
    }

    inline const VarTypeDatas_Plot *get_varTypeDataPtr_Plot( VariableTypeIdx type_ )const noexcept{
        tprAssert( this->varTypeDatas.find(type_) != this->varTypeDatas.end() );
        return this->varTypeDatas.at(type_).get();
    }

    //===== static =====//
    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 
    static plotBlueprintId_t init_new_plot( const std::string &plotName_, const std::string &plotLabel_ );

    inline static PlotBlueprint &get_plotBlueprintRef( plotBlueprintId_t id_ )noexcept{
        tprAssert( PlotBlueprint::plotUPtrs.find(id_) != PlotBlueprint::plotUPtrs.end() );
        return *(PlotBlueprint::plotUPtrs.at(id_));
    }

    // "_PLACE_HOLDER_", "_PLACE_HOLDER_" 为占位符 专用 
    inline static plotBlueprintId_t str_2_plotBlueprintId( const std::string &plotName_, const std::string &plotLabel_ )noexcept{
        if( plotName_ == "_PLACE_HOLDER_" ){
            tprAssert( plotLabel_ == "_PLACE_HOLDER_" );
            return PlotBlueprint::placeHolderId;
        }
        tprAssert( PlotBlueprint::name_2_ids.find(plotName_) != PlotBlueprint::name_2_ids.end() );
        const auto &innUMap = PlotBlueprint::name_2_ids.at( plotName_ );
        tprAssert( innUMap.find(plotLabel_) != innUMap.end() );
        return innUMap.at( plotLabel_ );
    }


    inline static bool is_placeHolderId( plotBlueprintId_t id_ )noexcept{
        return (id_ == PlotBlueprint::placeHolderId);
    }


private:
    PlotBlueprint()=default;

    IntVec2  sizeByMapEnt {}; // plot 尺寸，以 mapent 为单位 不一定必须是 正方形

    std::vector<MapData> mapDatas {}; // 若干帧，每一帧数据 就是一份 分配方案
    std::set<VariableTypeIdx> varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Plot>> varTypeDatas {}; // 类型数据

    //======== static ========//
    static ID_Manager  id_manager;
    static constexpr plotBlueprintId_t placeHolderId { static_cast<plotBlueprintId_t>(-1) }; // 占位符专用

    // 两层索引：plotName, plotLabel
    static std::unordered_map<std::string, std::unordered_map<std::string, plotBlueprintId_t>> name_2_ids; // 索引表
    static std::unordered_map<plotBlueprintId_t, std::unique_ptr<PlotBlueprint>> plotUPtrs; // 真实资源
};


void parse_plotJsonFiles();




}//--------------------- namespace: blueprint end ------------------------//
#endif 

