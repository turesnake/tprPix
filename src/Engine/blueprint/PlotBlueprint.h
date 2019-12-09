/*
 * ======================= PlotBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_PLOT_BLUE_PRINT_H
#define TPR_PLOT_BLUE_PRINT_H

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set>

//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "blueprintId.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "IntVec.h"
#include "ID_Manager.h"
#include "tprAssert.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


// 记录每个 mapent 上的数据，并不全，只有一部分，
// 剩余的信息，要通过 varType 获得
class MapDataEnt{
public:
    VariableTypeIdx varTypeIdx {};
    //--
    IntVec2         mposOff {}; // based on left-bottom 
    BrokenLvl       brokenLvl {BrokenLvl::Lvl_0};
    NineDirection   direction {NineDirection::Mid};  //- 角色 动画朝向
};


//- 一个实例，就是 1-frame png 数据，也就是一份 蓝图实例
class MapData{
public:
    std::vector<std::unique_ptr<MapDataEnt>> data {};
};



//- 确定一个 go 物种 的最基本数据
class GoSpec{
public:
    goSpecId_t              goSpecId    {};
    std::string             afsName     {};
    std::vector<AnimLabel>  animLabels  {}; //- 允许是空的
};



// 蓝图中，每种变量，对应的一组数据
// 数据不是单一的，可以是一个随机池，从中选取一种
class VarTypeDatas_Plot{
public:
    VarTypeDatas_Plot()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_ )noexcept{ this->goSpecPool.push_back( std::move(uptr_) ); }

    inline void init_check()const noexcept{
        tprAssert( !this->goSpecPool.empty() );
    }

    //----- get -----//
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }
    inline const std::vector<std::unique_ptr<GoSpec>> &get_goSpecPool()const noexcept{ return this->goSpecPool; }
private:
    std::vector<std::unique_ptr<GoSpec>> goSpecPool {}; // 可有 1～n 种，禁止为 0 种
    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
};



// 地块级蓝图。最小级别的蓝图，只有 数 mapents 大
// 
class PlotBlueprint{
public:
    PlotBlueprint()=default; // DO NOT CALL IT DIRECTLY!!!

    inline void insert_2_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                        std::unique_ptr<VarTypeDatas_Plot> uptr_ )noexcept{
        auto outPair1 = this->varTypeDatas.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( outPair1.second );
        auto outPair2 = this->varTypes.insert( typeIdx_ );
        tprAssert( outPair2.second );
    }

    inline void set_sizeByMapEnt( IntVec2 val_ )noexcept{ this->sizeByMapEnt = val_; }

    inline void init_check()const noexcept{
        tprAssert( !this->mapDatas.empty() );
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
    static void init_for_static()noexcept;
    static plotBlueprintId_t init_new_plot( const std::string &name_ );

    inline static PlotBlueprint &get_plotBlueprintRef( plotBlueprintId_t id_ )noexcept{
        tprAssert( PlotBlueprint::plotUPtrs.find(id_) != PlotBlueprint::plotUPtrs.end() );
        return *(PlotBlueprint::plotUPtrs.at(id_));
    }

    inline static plotBlueprintId_t str_2_plotBlueprintId( const std::string &name_ )noexcept{
        tprAssert( PlotBlueprint::name_2_ids.find(name_) != PlotBlueprint::name_2_ids.end() );
        return PlotBlueprint::name_2_ids.at(name_);
    }

private:
    IntVec2  sizeByMapEnt {}; // plot 尺寸，以 mapent 为单位

    std::vector<MapData> mapDatas {}; // 存在 png 中的 mp-go 数据，有若干帧，可随机挑选
    std::set<VariableTypeIdx> varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Plot>> varTypeDatas {}; // 类型数据

    //======== static ========//
    static ID_Manager  id_manager;
    static std::unordered_map<std::string, plotBlueprintId_t> name_2_ids; // 索引表
    static std::unordered_map<plotBlueprintId_t, std::unique_ptr<PlotBlueprint>> plotUPtrs; // 真实资源
};


void parse_from_plotJsonFiles();




}//--------------------- namespace: blueprint end ------------------------//
#endif 

