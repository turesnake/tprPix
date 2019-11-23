/*
 * ====================== MapEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_ENT_H
#define TPR_MAP_ENT_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_set> 

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "GoAltiRange.h"
#include "MapCoord.h"
#include "EcoSysPlanType.h"
#include "MapAltitude.h"
#include "GameObjType.h"
#include "ColliderType.h"

#include "sectionKey.h"
#include "colorTableId.h"
#include "Density.h"



//-- 投影地图单位的信息 64*64 pixes --
class MemMapEnt{
public:
    //-- 临时测试用 
    MemMapEnt()=default;

     
    inline void insert_2_circular_goids( goid_t goid_, ColliderType colliType_ )noexcept{

        tprAssert( colliType_ == ColliderType::Circular );
        auto outPair1 = this->majorGos_circular.insert( goid_ );
        tprAssert( outPair1.second );
        //-- 
        auto outPair3 = this->majorGos.insert( goid_ );
        tprAssert( outPair3.second );
    }


    inline void erase_from_circular_goids( goid_t goid_, ColliderType colliType_ )noexcept{

        tprAssert( colliType_ == ColliderType::Circular );        
        size_t eraseNum = this->majorGos_circular.erase(goid_);
        tprAssert( eraseNum == 1 );
        //--
        eraseNum = this->majorGos.erase(goid_);
        tprAssert( eraseNum == 1 );
    }

    inline void set_square_goid( goid_t goid_, ColliderType colliType_ )noexcept{ 

        tprAssert( colliType_ == ColliderType::Square );
        this->majorGo_square = goid_; //- 可能会覆盖原有数据
        //--
        auto outPair = this->majorGos.insert( goid_ );
        tprAssert( outPair.second );
    }

    //- 当一个 squGo 被杀死时，才会被调用
    //  自动释放一个 chunk 时，并不会调用此函数
    inline void erase_square_goid( goid_t goid_, ColliderType colliType_ )noexcept{

        tprAssert( colliType_ == ColliderType::Square );
        tprAssert( goid_ == this->majorGo_square );
        this->majorGo_square = 0; // null
        //--
        size_t eraseNum = this->majorGos.erase(goid_);
        tprAssert( eraseNum == 1 );
    }

    
    inline void set_mcpos( const MapCoord &mcpos_ )noexcept{ this->mcpos = mcpos_; }
    inline void set_ecoObjKey(sectionKey_t key_)noexcept{ this->ecoObjKey = key_; }
    inline void set_colorTableId(colorTableId_t id_)noexcept{ this->colorTableId = id_; }
    inline void set_density(Density d_)noexcept{ this->density = d_; }
    inline void set_mapAlti( MapAltitude alti_ )noexcept{ this->mapAlti = alti_; }
    inline void set_isBorder( bool b_ )noexcept{ this->isBorder = b_; }


    inline void set_perlin( double originPerlin_, size_t uWeight_ ){
        this->originPerlin = originPerlin_;
        this->uWeight = uWeight_;
    }


    inline IntVec2          get_mpos()const noexcept{ return this->mcpos.get_mpos();}
    inline MapAltitude      get_mapAlti()const noexcept{ return this->mapAlti; }
    inline sectionKey_t     get_ecoObjKey() const noexcept{ return this->ecoObjKey; }
    inline colorTableId_t   get_colorTableId()const noexcept{ return this->colorTableId; }
    inline size_t           get_uWeight()const noexcept{ return this->uWeight; }
    inline bool             get_isBorder()const noexcept{ return this->isBorder; }
    inline Density          get_density()const noexcept{ return this->density; }

    
    inline const std::unordered_set<goid_t>     &get_goids()const noexcept{return this->majorGos; }
    inline const std::unordered_set<goid_t>     &get_circular_goids()const noexcept{return this->majorGos_circular; }
    inline goid_t                               get_square_goid()const noexcept{ return this->majorGo_square; }

    
    
private:
    MapCoord  mcpos {}; //- 本 mapent 世界坐标值 

    MapAltitude   mapAlti     {}; //- 本 mapent 中点pix 的 alti

    sectionKey_t        ecoObjKey {};
    colorTableId_t      colorTableId {}; // same as ecoObj.colorTableId
    Density             density {};

    double  originPerlin {}; // [-1.0, 1.0]
    size_t  uWeight      {}; // [0, 9999]

    bool    isBorder     {false}; // 在未来，将被拓展为 一个 具体的数字，表示自己离 border 的距离（mapents）...

    // 支持 move/skill 碰撞检测
    std::unordered_set<goid_t>  majorGos {}; // cir + squ 
    std::unordered_set<goid_t>  majorGos_circular {};
    goid_t                      majorGo_square {}; // only one
                                    // 一个 mapent 其实还允许出现类似 mapsurface 的 地板go
                                    // 但是它们不是 majorGo，不参与游戏交互。
                                    // 所以不会被登记到 mapent 中
    

};



#endif

