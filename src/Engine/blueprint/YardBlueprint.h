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


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


class VarTypeDatas_Yard{
public:
    VarTypeDatas_Yard()=default;
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

    //-- 以下 2 函数，只有一个被使用
    inline const GoSpec &apply_a_goSpec( size_t uWeight_ )noexcept{
        tprAssert( !this->isPlotBlueprint );
        if( this->isAllInstanceUseSamePlan ){
            //-- 统一值 只生成一次，并永久保留
            if( !this->isUnifiedValsset ){
                this->isUnifiedValsset = true;
                this->unifiedGoSpecPtr = this->goSpecPool.at( (uWeight_ + 751446131) % this->goSpecPool.size() ).get();
            }
            //-- 直接获得 统一值
            return *(this->unifiedGoSpecPtr);
        }else{
            //-- 每次都独立分配 yardId --
            return *(this->goSpecPool.at( (uWeight_ + 751446131) % this->goSpecPool.size() ));
        }
    }
    inline plotBlueprintId_t apply_a_plotBlueprintId( size_t uWeight_ )noexcept{
        tprAssert( this->isPlotBlueprint );
        if( this->isAllInstanceUseSamePlan ){
            //-- 统一值 只生成一次，并永久保留
            if( !this->isUnifiedValsset ){
                this->isUnifiedValsset = true;
                this->unifiedPlotId = this->plotIds.at( (uWeight_ + 751446131) % this->plotIds.size() );
            }
            //-- 直接获得 统一值
            return this->unifiedPlotId;
        }else{
            //-- 每次都独立分配 yardId --
            return this->plotIds.at( (uWeight_ + 751446131) % this->plotIds.size() );
        }
    }

private:
    //-- 以下 2 容器，只有一个被使用
    std::vector<std::unique_ptr<GoSpec>> goSpecPool {};
    std::vector<plotBlueprintId_t>       plotIds {};
    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    bool isPlotBlueprint {}; // 本变量是否为一个 plot
    //-- 以下 2 数据，只有一个被使用
    GoSpec              *unifiedGoSpecPtr   {nullptr};
    plotBlueprintId_t   unifiedPlotId       {};
    bool                isUnifiedValsset    {false};    
};




// 院子级蓝图。中间级别的蓝图，有数 fields 大
class YardBlueprint{
public:
    YardBlueprint()=default; // DO NOT CALL IT DIRECTLY!!!


    inline void insert_2_varTypeUPtrs(  VariableTypeIdx typeIdx_, 
                                        std::unique_ptr<VarTypeDatas_Yard> uptr_ )noexcept{
        auto outPair = this->varTypeUPtrs.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( outPair.second );
    }

    inline void set_sizeByField( IntVec2 sizeByMapEnt_ )noexcept{ 
        this->sizeByFild = sizeByMapEnt_2_yardSize( sizeByMapEnt_ );
    }

    inline void init_check()const noexcept{
        tprAssert( !this->mapDatas.empty() );
        tprAssert( !this->varTypeUPtrs.empty() );
    }

    inline std::vector<MapData> &getnc_mapDatasRef()noexcept{ return this->mapDatas; }

    inline const MapData &apply_a_random_mapData( size_t uWeight_ )const noexcept{
        return this->mapDatas.at( (uWeight_ + 86887311) % this->mapDatas.size() );
    }

    inline VarTypeDatas_Yard &get_varTypeDatas_Yard( VariableTypeIdx type_ )noexcept{
        tprAssert( this->varTypeUPtrs.find(type_) != this->varTypeUPtrs.end() );
        return *(this->varTypeUPtrs.at(type_));
    }

    //===== static =====//
    static void init_for_static()noexcept;
    static yardBlueprintId_t init_new_yard( const std::string &name_ );


    /*
    inline static YardBlueprint &get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept{
        tprAssert( YardBlueprint::yardUPtrs.find(id_) != YardBlueprint::yardUPtrs.end() );
        return *(YardBlueprint::yardUPtrs.at(id_));
    }
    */


    static YardBlueprint &get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept;



    inline static yardBlueprintId_t str_2_yardBlueprintId( const std::string &name_ )noexcept{
        tprAssert( YardBlueprint::name_2_ids.find(name_) != YardBlueprint::name_2_ids.end() );
        return YardBlueprint::name_2_ids.at(name_);
    }

private:
    YardSize sizeByFild {}; // yard 尺寸，以 field 为单位

    std::vector<MapData> mapDatas {}; // 存在 png 中的 mp-go 数据，有若干帧，可随机挑选
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Yard>> varTypeUPtrs {}; // 类型数据

    //===== static =====//
    static ID_Manager  id_manager;
    static std::unordered_map<std::string, yardBlueprintId_t> name_2_ids; // 索引表
    static std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>> yardUPtrs; // 真实资源

};


void parse_from_yardJsonFiles();



}//--------------------- namespace: blueprint end ------------------------//
#endif 

