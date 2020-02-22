/*
 * ===================== GoDataForCreate.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_DATA_FOR_CREATE_H
#define TPR_GO_DATA_FOR_CREATE_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//------------------- CPP --------------------//
#include <vector>
#include <memory>
#include <variant>
#include <optional>

//------------------- Engine --------------------//
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "FloorGoType.h"
#include "animSubspeciesId.h"
#include "IntVec.h"
#include "AnimActionEName.h"
#include "GoAltiRange.h"
#include "GoAssemblePlan.h"



// 生成一个go实例，需要的基本数据
// - 在 ecoobj 生成阶段，此数据被创建，并永久存储在 ecoobj 中
// - 以 const 指针 的形式，被传递到 job chunk/filed, 以及 具象go类中
// 所以，不用担心此数据的 容量
class GoDataForCreate{
public:

    class GoMesh;
    
    //========== Self Vals ==========//
    GoDataForCreate()=default;

    inline std::optional<BrokenLvl> get_brokenLvl()const noexcept{
        if( auto vPtr = std::get_if<BrokenLvl>( &this->brokenLvl_or_floorGoLayer ) ){
            return { *vPtr };
        }else{
            return std::nullopt;
        }
    }

    inline std::optional<FloorGoLayer> get_floorGoLayer()const noexcept{
        if( auto vPtr = std::get_if<FloorGoLayer>( &this->brokenLvl_or_floorGoLayer ) ){
            return { *vPtr };
        }else{
            return std::nullopt;
        }
    }

    static void assemble_new_goDataForCreate(   GoDataForCreate     &goDRef_,
                                                const glm::dvec2    &dpos_, // 让外部计算好
                                                goSpeciesId_t       goSpeciesId_,
                                                goLabelId_t         goLabelId_,
                                                NineDirection       direction_,
                                                std::variant<std::monostate, BrokenLvl, FloorGoLayer> brokenLvl_or_floorGoLayer_,
                                                size_t mapEntUWeight_ );

    //---
    goSpeciesId_t       goSpeciesId {};
    glm::dvec2          dpos      {}; // go 绝对 dpos

    NineDirection       direction {NineDirection::Center};  //- 角色 动画朝向
    GoAltiRangeLabel    goAltiRangeLabel {};

    std::variant<   std::monostate,
                    BrokenLvl, 
                    FloorGoLayer> brokenLvl_or_floorGoLayer {};
    
    //---




    //bool            isNeedWind    {}; // 是否需要生成 风吹值,暂时 始终为 true


    std::vector<std::unique_ptr<GoMesh>> goMeshEntUPtrs {};
};




class GoDataForCreate::GoMesh{
public:
    GoMesh( const GoAssemblePlanSet::GoMeshEnt *goMeshEntPtr_ ):
        goMeshEntPtr(goMeshEntPtr_)
        {
            tprAssert( this->goMeshEntPtr );
        }

    //------- set -------//
    inline void set_subspeciesId( animSubspeciesId_t id_ )noexcept{ this->subspeciesId = id_; }
    inline void set_windDelayIdx( size_t windDelayIdx_ )noexcept{ this->windDelayIdx = windDelayIdx_; }
    
    //------- get -------//
    inline const std::string    &get_goMeshName()const noexcept{ return this->goMeshEntPtr->goMeshName; }
    inline const std::string    &get_animFrameSetName()const noexcept{ return this->goMeshEntPtr->animFrameSetName; }
    inline glm::dvec2           get_dposOff()const noexcept{ return this->goMeshEntPtr->dposOff; }
    inline double               get_zOff()const noexcept{ return this->goMeshEntPtr->zOff; }
    inline AnimLabel            get_animLabel()const noexcept{ return this->goMeshEntPtr->animLabel; }
    inline AnimActionEName      get_animActionEName()const noexcept{ return this->goMeshEntPtr->animActionEName; }
    inline RenderLayerType      get_renderLayerType()const noexcept{ return this->goMeshEntPtr->renderLayerType; }
    inline ShaderType           get_shaderType()const noexcept{ return this->goMeshEntPtr->shaderType; }

    inline bool                 get_isVisible()const noexcept{ return this->goMeshEntPtr->isVisible; }

    //---
    inline animSubspeciesId_t   get_subspeciesId()const noexcept{ return this->subspeciesId; }
    inline size_t               get_windDelayIdx()const noexcept{ return this->windDelayIdx; }

private:
    const GoAssemblePlanSet::GoMeshEnt *goMeshEntPtr {nullptr};

    animSubspeciesId_t  subspeciesId {};
    size_t              windDelayIdx {}; // only used in windClock

};




#endif 

