/*
 * ======================= VillageBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_VILLAGE_BLUE_PRINT_H
#define TPR_VILLAGE_BLUE_PRINT_H


//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"

#include "YardBlueprint.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//




class VarTypeDatas_Village{
public:
    VarTypeDatas_Village()=default;
    //----- set -----//
    inline void set_isAllInstanceUseSamePlan( bool b_ )noexcept{ this->isAllInstanceUseSamePlan = b_; }
    inline void set_isRoad( bool b_ )noexcept{ this->isRoad = b_; }
    inline void insert_2_yardIds( yardBlueprintId_t id_ )noexcept{ this->yardIds.push_back(id_); }

    inline void init_check()const noexcept{
        tprAssert( !this->yardIds.empty() );
    }
    //----- get -----//
    inline bool get_isRoad()const noexcept{ return this->isRoad; }
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }


    inline yardBlueprintId_t apply_a_yardBlueprintId( size_t uWeight_ )noexcept{
        if( this->isAllInstanceUseSamePlan ){
            //-- 统一值 只生成一次，并永久保留
            if( !this->isUnifiedValsset ){
                this->isUnifiedValsset = true;
                this->unifiedYardId = this->yardIds.at( (uWeight_ + 7876117) % this->yardIds.size() );
            }
            //-- 直接获得 统一值
            return this->unifiedYardId;
        }else{
            //-- 每次都独立分配 yardId --
            return this->yardIds.at( (uWeight_ + 7876117) % this->yardIds.size() );
        }
    }
    

private:
    std::vector<yardBlueprintId_t> yardIds {};
    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    bool isRoad {}; // 本变量是否为一个 道路单位 
    //---
    yardBlueprintId_t unifiedYardId {99};
    bool isUnifiedValsset       {false};    
};





// 村级蓝图。最大级别的蓝图。 一个 section/ecoobj，分配一个 村级蓝图。
class VillageBlueprint{
public:
    VillageBlueprint()=default; // DO NOT CALL IT DIRECTLY!!!


    inline void insert_2_varTypeUPtrs(  VariableTypeIdx typeIdx_, 
                                        std::unique_ptr<VarTypeDatas_Village> uptr_ )noexcept{
        auto outPair = this->varTypeUPtrs.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( outPair.second );
    }

    inline void init_check()const noexcept{
        tprAssert( !this->mapDatas.empty() );
        tprAssert( !this->varTypeUPtrs.empty() );
    }


    inline std::vector<MapData> &getnc_mapDatasRef()noexcept{ return this->mapDatas; }


    inline const MapData &apply_a_random_mapData( size_t uWeight_ )const noexcept{
        return this->mapDatas.at( (uWeight_ + 3731577) % this->mapDatas.size() );
    }

    inline VarTypeDatas_Village &get_varTypeDatas_Village( VariableTypeIdx type_ )noexcept{
        tprAssert( this->varTypeUPtrs.find(type_) != this->varTypeUPtrs.end() );
        return *(this->varTypeUPtrs.at(type_));
    }


    //===== static =====//
    static void init_for_static()noexcept;
    static villageBlueprintId_t init_new_village( const std::string &name_ );

    inline static VillageBlueprint &get_villageBlueprintRef( villageBlueprintId_t id_ )noexcept{
        tprAssert( VillageBlueprint::villageUPtrs.find(id_) != VillageBlueprint::villageUPtrs.end() );
        return *(VillageBlueprint::villageUPtrs.at(id_));
    }

    inline static villageBlueprintId_t str_2_villageBlueprintId( const std::string &name_ )noexcept{
        tprAssert( VillageBlueprint::name_2_ids.find(name_) != VillageBlueprint::name_2_ids.end() );
        return VillageBlueprint::name_2_ids.at(name_);
    }


private:

    std::vector<MapData> mapDatas {}; // 存在 png 中的 mp-go 数据，有若干帧，可随机挑选
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Village>> varTypeUPtrs {}; // 类型数据

    //===== static =====//
    static ID_Manager  id_manager;
    static std::unordered_map<std::string, villageBlueprintId_t> name_2_ids; // 索引表
    static std::unordered_map<villageBlueprintId_t, std::unique_ptr<VillageBlueprint>> villageUPtrs; // 真实资源
};


void parse_from_villageJsonFiles();



}//--------------------- namespace: blueprint end ------------------------//
#endif 

