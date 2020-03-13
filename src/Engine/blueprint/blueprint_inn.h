/*
 * ======================= blueprint_inn.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * only included by blueprint.cpp
 */
#ifndef TPR_BLUE_PRINT_INN_H
#define TPR_BLUE_PRINT_INN_H


//-------------------- Engine --------------------//
#include "PlotBlueprint.h"
#include "YardBlueprint.h"
#include "VillageBlueprint.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace blueP_inn {//----------- namespace: blueP_inn ----------------//


// blueprint 中的数据 全部是只读的 （为了支持多线程访问）
// 为了存储额外的 变量，使用以下 class 来实现

//-----------------//
//      Plot
//-----------------//
class VarType_Plot_Manager{

    class VarType{
        friend class VarType_Plot_Manager;
    public:
        VarType(  bool isAllInstanceUseSamePlan_, 
                    const VarTypeDatas_Plot *vtPtr_ ):
            isAllInstanceUseSamePlan(isAllInstanceUseSamePlan_),
            vtPtr(vtPtr_)
            {}
    private:
        inline const GoSpec &apply_a_goSpec( size_t uWeight_ )noexcept{
            const GoSpec &goSpecRef = this->vtPtr->apply_rand_goSpec( uWeight_ );

            if( this->isAllInstanceUseSamePlan ){
                //-- 统一值 只生成一次，并永久保留
                if( !this->isUnifiedValsset ){
                    this->isUnifiedValsset = true;
                    this->unifiedGoSpecPtr = &goSpecRef;
                }
                //-- 直接获得 统一值
                return *(this->unifiedGoSpecPtr);
            }else{
                //-- 每次都独立分配 yardId --
                return goSpecRef;
            }
        }
        //===== vals =====//
        bool isAllInstanceUseSamePlan   {}; // 是否 本类型的所有个体，共用一个 实例化对象
        const VarTypeDatas_Plot *vtPtr {};
        //---
        const GoSpec  *unifiedGoSpecPtr {nullptr};
        bool isUnifiedValsset           {false}; 
    };

public:
    explicit VarType_Plot_Manager( PlotBlueprint &plot_ ){
        for( const auto &type : plot_.get_varTypes() ){
            const auto *vtPtr = plot_.get_varTypeDataPtr_Plot( type );
            this->varTypes_P.insert({ type, VarType{ vtPtr->get_isAllInstanceUseSamePlan(), vtPtr } });
        }
    }
    inline const GoSpec &apply_a_goSpec( VariableTypeIdx type_, size_t uWeight_ )noexcept{
        tprAssert( this->varTypes_P.find(type_) != this->varTypes_P.end() );
        return this->varTypes_P.at(type_).apply_a_goSpec( uWeight_ );
    }
private:
    std::unordered_map<VariableTypeIdx, VarType> varTypes_P {};
};




//-----------------//
//      Yard
//-----------------//
class VarType_Yard_Manager{

    class VarType_Y_MajorGo{
        friend class VarType_Yard_Manager;
    public:
        VarType_Y_MajorGo(  bool isAllInstanceUseSamePlan_, 
                            bool isPlotBlueprint_,
                            const VarTypeDatas_Yard_MajorGo *vtPtr_ ):
            isAllInstanceUseSamePlan(isAllInstanceUseSamePlan_),
            isPlotBlueprint(isPlotBlueprint_),
            vtPtr(vtPtr_)
            {}
    private:
        //-- 以下 2 函数，只有一个被使用
        inline const GoSpec &apply_a_goSpec( size_t uWeight_ )noexcept{
            tprAssert( !this->isPlotBlueprint );
            const GoSpec &goSpecRef = this->vtPtr->apply_rand_goSpec( uWeight_ );

            if( this->isAllInstanceUseSamePlan ){
                //-- 统一值 只生成一次，并永久保留
                if( !this->isUnifiedValsset ){
                    this->isUnifiedValsset = true;
                    this->unifiedGoSpecPtr = &goSpecRef;
                }
                //-- 直接获得 统一值
                return *(this->unifiedGoSpecPtr);
            }else{
                //-- 每次都独立分配 yardId --
                return goSpecRef;
            }
        }
        inline plotBlueprintId_t apply_a_plotBlueprintId( size_t uWeight_ )noexcept{

            tprAssert( this->isPlotBlueprint );
            const auto &plotIdsRef = this->vtPtr->get_plotIds();

                tprAssert( !plotIdsRef.empty() );

            if( this->isAllInstanceUseSamePlan ){
                //-- 统一值 只生成一次，并永久保留
                if( !this->isUnifiedValsset ){
                    this->isUnifiedValsset = true;
                    this->unifiedPlotId = plotIdsRef.at( (uWeight_ + 751446131) % plotIdsRef.size() );
                }
                //-- 直接获得 统一值
                return this->unifiedPlotId;
            }else{
                //-- 每次都独立分配 yardId --
                return plotIdsRef.at( (uWeight_ + 751446131) % plotIdsRef.size() );
            }
        }
        //===== vals =====//
        bool isAllInstanceUseSamePlan   {}; // 是否 本类型的所有个体，共用一个 实例化对象
        bool isPlotBlueprint            {}; // 本变量是否为一个 plot
        const VarTypeDatas_Yard_MajorGo *vtPtr {};
        //-- 以下 2 数据，只有一个被使用
        const GoSpec              *unifiedGoSpecPtr {nullptr};
        plotBlueprintId_t   unifiedPlotId       {};
        bool isUnifiedValsset           {false}; 
    };


    class VarType_Y_FloorGo{
        friend class VarType_Yard_Manager;
    public:
        VarType_Y_FloorGo(  bool isAllInstanceUseSamePlan_,
                            const VarTypeDatas_Yard_FloorGo *vtPtr_ ):
            isAllInstanceUseSamePlan(isAllInstanceUseSamePlan_),
            vtPtr(vtPtr_)
            {}

    private:
        inline const GoSpec &apply_a_goSpec( size_t uWeight_ )noexcept{
            const GoSpec &goSpecRef = this->vtPtr->apply_rand_goSpec( uWeight_ );

            if( this->isAllInstanceUseSamePlan ){
                //-- 统一值 只生成一次，并永久保留
                if( !this->isUnifiedValsset ){
                    this->isUnifiedValsset = true;
                    this->unifiedGoSpecPtr = &goSpecRef;
                }
                //-- 直接获得 统一值
                return *(this->unifiedGoSpecPtr);
            }else{
                //-- 每次都独立分配 yardId --
                return goSpecRef;
            }
        }
        //===== vals =====//
        bool isAllInstanceUseSamePlan   {}; // 是否 本类型的所有个体，共用一个 实例化对象
        const VarTypeDatas_Yard_FloorGo *vtPtr {};
        //---
        const GoSpec *unifiedGoSpecPtr   {nullptr};
        bool isUnifiedValsset           {false}; 
    };

public:
    explicit VarType_Yard_Manager( YardBlueprint &yard_ ){
        for( const auto &type : yard_.get_majorGo_varTypes() ){
            const auto *vtPtr = yard_.get_majorGo_varTypeDataPtr_Yard( type );
            this->varTypes_Y_majorGo.insert({ type, VarType_Y_MajorGo{vtPtr->get_isAllInstanceUseSamePlan(), 
                                                                                    vtPtr->get_isPlotBlueprint(),
                                                                                    vtPtr } });
        }
        for( const auto &type : yard_.get_floorGo_varTypes() ){
            const auto *vtPtr = yard_.get_floorGo_varTypeDataPtr_Yard( type );
            this->varTypes_Y_floorGo.insert({ type, VarType_Y_FloorGo{vtPtr->get_isAllInstanceUseSamePlan(), 
                                                                                    vtPtr } });
        }
    }
    inline const GoSpec &apply_a_majorGoSpec( VariableTypeIdx type_, size_t uWeight_ )noexcept{
        tprAssert( this->varTypes_Y_majorGo.find(type_) != this->varTypes_Y_majorGo.end() );
        return this->varTypes_Y_majorGo.at(type_).apply_a_goSpec( uWeight_ );
    }
    inline const GoSpec &apply_a_floorGoSpec( VariableTypeIdx type_, size_t uWeight_ )noexcept{
        tprAssert( this->varTypes_Y_floorGo.find(type_) != this->varTypes_Y_floorGo.end() );
        return this->varTypes_Y_floorGo.at(type_).apply_a_goSpec( uWeight_ );
    }
    inline plotBlueprintId_t apply_a_plotBlueprintId( VariableTypeIdx type_, size_t uWeight_ )noexcept{
        tprAssert( this->varTypes_Y_majorGo.find(type_) != this->varTypes_Y_majorGo.end() );
        return this->varTypes_Y_majorGo.at(type_).apply_a_plotBlueprintId( uWeight_ );
    }
private:
    std::unordered_map<VariableTypeIdx, VarType_Y_MajorGo> varTypes_Y_majorGo {};
    std::unordered_map<VariableTypeIdx, VarType_Y_FloorGo> varTypes_Y_floorGo {};
};







//-----------------//
//    Village
//-----------------//
class VarType_Village_Manager{

    class VarType{
        friend class VarType_Village_Manager;
    public:
        VarType(  bool isAllInstanceUseSamePlan_, 
                    const VarTypeDatas_Village *vtPtr_ ):
            isAllInstanceUseSamePlan(isAllInstanceUseSamePlan_),
            vtPtr(vtPtr_)
            {}
    private:
        yardBlueprintId_t apply_a_yardBlueprintId( size_t uWeight_, NineDirection yardDir_ )noexcept;
        //----- vals -----//
        bool isAllInstanceUseSamePlan   {}; // 是否 本类型的所有个体，共用一个 实例化对象
        const VarTypeDatas_Village *vtPtr {};
        yardBlueprintId_t unifiedYardId {};
        bool isUnifiedValsset           {false}; 
    };


public:
    explicit VarType_Village_Manager( VillageBlueprint &village_ ){
        for( const auto &type : village_.get_varTypes() ){
            const auto *vtPtr = village_.get_varTypeDataPtr_Village( type );
            this->varTypes.insert({ type, VarType{ vtPtr->get_isAllInstanceUseSamePlan(), vtPtr } });
        }
    }
    inline yardBlueprintId_t apply_a_yardBlueprintId( VariableTypeIdx type_, size_t uWeight_, NineDirection yardDir_ )noexcept{
        tprAssert( this->varTypes.find(type_) != this->varTypes.end() );
        return this->varTypes.at(type_).apply_a_yardBlueprintId( uWeight_, yardDir_ );
    }
private:
    std::unordered_map<VariableTypeIdx, VarType> varTypes {};
};











}//-------------- namespace: blueP_inn end ----------------//
}//--------------------- namespace: blueprint end ------------------------//
#endif 

