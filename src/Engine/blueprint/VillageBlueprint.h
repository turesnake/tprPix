/*
 * ======================= VillageBlueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_VILLAGE_BLUE_PRINT_H
#define TPR_VILLAGE_BLUE_PRINT_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "BlueprintVarType.h"
#include "GameObjType.h"
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

    inline void insert_2_getYardId_functors( YardBlueprintSet::F_getYardId f_, size_t num_ )noexcept{ 
        this->getYardId_functors.insert( this->getYardId_functors.end(), num_, f_ ); // copy
    }


    void init_check()noexcept;

    //----- get -----//
    inline bool get_isRoad()const noexcept{ return this->isRoad; }
    inline bool get_isAllInstanceUseSamePlan()const noexcept{ return this->isAllInstanceUseSamePlan; }

    inline std::optional<yardBlueprintId_t> apply_rand_yardBlueprintId( size_t uWeight_, NineDirection yardDir_ )const noexcept{
        size_t idx = (uWeight_ + 1076173) % this->getYardId_functors.size();
        return this->getYardId_functors.at(idx)( yardDir_ );
    }


private:
    // 不再直接存储 yardId, 而是存储一组 函数指针，在运行时，通过参数传入 yardDir, 再找出 yardId
    std::vector<YardBlueprintSet::F_getYardId> getYardId_functors {}; 

    bool isAllInstanceUseSamePlan {}; // 是否 本类型的所有个体，共用一个 实例化对象
    bool isRoad {}; // 本变量是否为一个 道路单位  
};





// 村级蓝图。最大级别的蓝图。 一个 section/ecoobj，分配一个 村级蓝图。
class VillageBlueprint{
public:

    inline void insert_2_varTypeDatas(  VariableTypeIdx typeIdx_, 
                                        std::unique_ptr<VarTypeDatas_Village> uptr_ )noexcept{
        auto [insertIt1, insertBool1] = this->varTypeDatas.insert({ typeIdx_, std::move(uptr_) });
        tprAssert( insertBool1 );
        auto [insertIt2, insertBool2] = this->varTypes.insert( typeIdx_ );
        tprAssert( insertBool2 );
    }

    inline void init_check()const noexcept{
        tprAssert( !this->mapDatas.empty() );
        tprAssert( !this->varTypeDatas.empty() );
    }

    //- 仅用于 读取 json数据 时 -
    inline std::vector<MapData> &getnc_mapDatasRef()noexcept{ return this->mapDatas; }

    inline const std::set<VariableTypeIdx> &get_varTypes()const noexcept{ return this->varTypes; }

    inline const MapData &apply_a_random_mapData( size_t uWeight_ )const noexcept{
        return this->mapDatas.at( (uWeight_ + 3731577) % this->mapDatas.size() );
    }

    inline const VarTypeDatas_Village *get_varTypeDataPtr_Village( VariableTypeIdx type_ )const noexcept{
        tprAssert( this->varTypeDatas.find(type_) != this->varTypeDatas.end() );
        return this->varTypeDatas.at(type_).get();
    }

    //===== static =====//
    static void init_for_static()noexcept;// MUST CALL IN MAIN !!! 
    static villageBlueprintId_t init_new_village( const std::string &name_ );

    inline static VillageBlueprint &get_villageBlueprintRef( villageBlueprintId_t id_ )noexcept{
        tprAssert( VillageBlueprint::villageUPtrs.find(id_) != VillageBlueprint::villageUPtrs.end() );
        return *(VillageBlueprint::villageUPtrs.at(id_));
    }

    inline static villageBlueprintId_t str_2_villageBlueprintId( const std::string &name_ )noexcept{

        villageBlueprintId_t id = VillageBlueprint::hasher(name_);
        tprAssert( VillageBlueprint::villageIds.find(id) != VillageBlueprint::villageIds.end() );
        return id;
    }


private:
    VillageBlueprint()=default;

    std::vector<MapData> mapDatas {}; // 若干帧，每一帧数据 就是一份 分配方案
    std::set<VariableTypeIdx> varTypes {};
    std::unordered_map<VariableTypeIdx, std::unique_ptr<VarTypeDatas_Village>> varTypeDatas {}; // 类型数据

    //===== static =====//
    static std::unordered_set<villageBlueprintId_t> villageIds; // 记录已被存储的 ids
    static std::hash<std::string> hasher;

    static std::unordered_map<villageBlueprintId_t, std::unique_ptr<VillageBlueprint>> villageUPtrs; // 真实资源
};


void parse_villageJsonFiles();



}//--------------------- namespace: blueprint end ------------------------//
#endif 

