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

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "GoAltiRange.h"
#include "MapCoord.h"
#include "chunkKey.h"
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
        auto [insertIt1, insertBool1] = this->majorGos_circular.insert( goid_ );
        tprAssert( insertBool1 );
        //-- 
        auto [insertIt2, insertBool2] = this->majorGos.insert( goid_ );
        tprAssert( insertBool2 );
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
        tprAssert( this->majorGo_square == 0 ); // 一定要先合法地释放掉旧元素后，再登记新元素。禁止直接覆盖！
        this->majorGo_square = goid_;
        //--
        auto [insertIt, insertBool] = this->majorGos.insert( goid_ );
        tprAssert( insertBool );
    }

    //- 当一个 squGo 被杀死时，才会被调用
    //  自动释放一个 chunk 时，并不需要调用此函数
    inline void erase_square_goid( goid_t goid_, ColliderType colliType_ )noexcept{

        tprAssert( colliType_ == ColliderType::Square );
        tprAssert( goid_ == this->majorGo_square );
        this->majorGo_square = 0; // null
        //--
        size_t eraseNum = this->majorGos.erase(goid_);
        tprAssert( eraseNum == 1 );
    }

    inline void set_bioSoup_goid( goid_t goid_, GameObjFamily family_ )noexcept{ 

        tprAssert( family_ == GameObjFamily::BioSoup );
        tprAssert( this->bioSoupGo == 0 ); // 一定要先合法地释放掉旧元素后，再登记新元素。禁止直接覆盖！
        this->bioSoupGo = goid_;
        //--
        auto [insertIt, insertBool] = this->majorGos.insert( goid_ );
        tprAssert( insertBool );
    }

    //- 当一个 bioSoupGo 被杀死时，才会被调用
    //  自动释放一个 chunk 时，并不需要调用此函数
    inline void erase_bioSoup_goid( goid_t goid_, GameObjFamily family_ )noexcept{

        tprAssert( family_ == GameObjFamily::BioSoup );
        tprAssert( goid_ == this->bioSoupGo );
        this->bioSoupGo = 0; // null
        //--
        size_t eraseNum = this->majorGos.erase(goid_);
        tprAssert( eraseNum == 1 );
    }

    
    inline void set_mcpos( const MapCoord &mcpos_ )noexcept{ this->mcpos = mcpos_; }
    inline void set_chunkKey(chunkKey_t key_)noexcept{ this->chunkKey = key_; }
    inline void set_ecoObjKey(sectionKey_t key_)noexcept{ this->ecoObjKey = key_; }
    inline void set_colorTableId(colorTableId_t id_)noexcept{ this->colorTableId = id_; }
    inline void set_density(Density d_)noexcept{ this->density = d_; }
    inline void set_mapAlti( MapAltitude alti_ )noexcept{ this->mapAlti = alti_; }
    inline void set_isBorder( bool b_ )noexcept{ this->isBorder = b_; }

    inline void set_uWeight( double v_ )noexcept{ this->uWeight = v_; }


    inline IntVec2          get_mpos()const noexcept{ return this->mcpos.get_mpos();}
    inline MapAltitude      get_mapAlti()const noexcept{ return this->mapAlti; }
    inline chunkKey_t       get_chunkKey() const noexcept{ return this->chunkKey; }
    inline sectionKey_t     get_ecoObjKey() const noexcept{ return this->ecoObjKey; }
    inline colorTableId_t   get_colorTableId()const noexcept{ return this->colorTableId; }
    inline size_t           get_uWeight()const noexcept{ return this->uWeight; }
    inline bool             get_isBorder()const noexcept{ return this->isBorder; }
    inline Density          get_density()const noexcept{ return this->density; }

    
    inline const std::unordered_set<goid_t>     &get_goids()const noexcept{return this->majorGos; }
    inline const std::unordered_set<goid_t>     &get_circular_goids()const noexcept{return this->majorGos_circular; }
    inline goid_t                               get_square_goid()const noexcept{ return this->majorGo_square; }
    inline goid_t                               get_bioSoup_goid()const noexcept{ return this->bioSoupGo; }
    
    
private:
    MapCoord  mcpos {}; //- 本 mapent 世界坐标值 

    MapAltitude   mapAlti     {}; //- 本 mapent 中点pix 的 alti

    chunkKey_t          chunkKey  {};
    sectionKey_t        ecoObjKey {};
    colorTableId_t      colorTableId {}; // same as ecoObj.colorTableId
    Density             density {};

    //double  originPerlin {}; // [-1.0, 1.0]
    size_t  uWeight      {}; // [0, 9999]

    bool    isBorder     {false}; // 在未来，将被拓展为 一个 具体的数字，表示自己离 border 的距离（mapents）...

    // 支持 move/skill 碰撞检测
    std::unordered_set<goid_t>  majorGos {}; // cir + squ 

    std::unordered_set<goid_t>  majorGos_circular {};
    goid_t                      majorGo_square {}; // only one
                                    // 一个 mapent 其实还允许出现 floorGo
                                    // 但是它们不是 majorGo，不参与游戏交互。
                                    // 所以不会被登记到 mapent 中

    goid_t                      bioSoupGo {}; // only one
                                    // 一个 mapent 允许同时含有 一个 mp-go, 一个 bioSoup


    
    // 在未来，可能要准备 BioSoup-mapent 的 goid 信息
    // 也许会和 majorGo_square 联合存储，也许是一个独立的变量来存储
    // ...
    

};



#endif

