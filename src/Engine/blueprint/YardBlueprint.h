/*
 * ======================= YardBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_YARD_BLUE_PRINT_H
#define TPR_YARD_BLUE_PRINT_H


//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"

#include "PlotBlueprint.h"

#include "blueprint_oth.h"

#include "ID_Manager.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/mapSurfaces/FloorGoSize.h" // tmp


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


class VarTypeDatas_Yard_MajorGo{
public:
    VarTypeDatas_Yard_MajorGo()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_isPlotBlueprint( bool b_ )noexcept{ this->isPlotBlueprint = b_; }
    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_ )noexcept{ this->goSpecPool.push_back( std::move( uptr_ ) ); }
    inline void insert_2_plotIds( plotBlueprintId_t id_ )noexcept{ this->plotIds.push_back(id_); }

    inline void init_check()const noexcept{
        if( this->isPlotBlueprint ){
            tprAssert( !this->plotIds.empty() );
        }else{
            tprAssert( !this->goSpecPool.empty() );
        }
    }

    //----- get -----//
    inline bool get_isPlotBlueprint()const noexcept{ return this->isPlotBlueprint; }
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }
    inline const std::vector<std::unique_ptr<GoSpec>> &get_goSpecPool()const noexcept{ return this->goSpecPool; } 
    inline const std::vector<plotBlueprintId_t> &get_plotIds()const noexcept{ return this->plotIds; }
private:
    //-- 以下 2 容器，只有一个被使用
    std::vector<std::unique_ptr<GoSpec>> goSpecPool {};
    std::vector<plotBlueprintId_t>       plotIds {};
    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    bool isPlotBlueprint {}; // 本变量是否为一个 plot 
};




class VarTypeDatas_Yard_FloorGo{
public:
    VarTypeDatas_Yard_FloorGo()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_floorGoSize( FloorGoSize size_ )noexcept{ this->floorGoSize = size_; };
    inline void insert_2_goSpecPool( std::unique_ptr<GoSpec> uptr_ )noexcept{ this->goSpecPool.push_back( std::move( uptr_ ) ); }

    inline void init_check()const noexcept{
        tprAssert( !this->goSpecPool.empty() );
    }

    //----- get -----//
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }
    inline FloorGoSize get_floorGoSize()const noexcept{ return this->floorGoSize; }
    inline const std::vector<std::unique_ptr<GoSpec>> &get_goSpecPool()const noexcept{ return this->goSpecPool; } 
private:
    //-- 以下 2 容器，只有一个被使用
    std::vector<std::unique_ptr<GoSpec>> goSpecPool {};
    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    FloorGoSize floorGoSize {};
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


    //===== static =====//
    static void init_for_static()noexcept;
    static yardBlueprintId_t init_new_yard( const std::string &name_ );

    static YardBlueprint &get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept;

    inline static yardBlueprintId_t str_2_yardBlueprintId( const std::string &name_ )noexcept{
        tprAssert( YardBlueprint::name_2_ids.find(name_) != YardBlueprint::name_2_ids.end() );
        return YardBlueprint::name_2_ids.at(name_);
    }

private:
    YardSize sizeByFild {}; // yard 尺寸，以 field 为单位

    // major-gos
    std::vector<MapData> majorGo_mapDatas {}; 
    std::set<VariableTypeIdx> majorGo_varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Yard_MajorGo>> majorGo_varTypeDatas {};

    // floor-gos
    std::vector<MapData> floorGo_mapDatas {};
    std::set<VariableTypeIdx> floorGo_varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Yard_FloorGo>> floorGo_varTypeDatas {};



    //- 至少有一个为 true
    bool isHaveMajorGos {}; // 是否有 常规go 数据
    bool isHaveFloorGos {};   // 是否有 地板 数据

    //===== static =====//
    static ID_Manager  id_manager;
    static std::unordered_map<std::string, yardBlueprintId_t> name_2_ids; // 索引表
    static std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>> yardUPtrs; // 真实资源

};


void parse_from_yardJsonFiles();



}//--------------------- namespace: blueprint end ------------------------//
#endif 

