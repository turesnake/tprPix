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

#include "sectionKey.h"
#include "colorTableId.h"
#include "Density.h"



//-- 投影地图单位的信息 64*64 pixes --
class MemMapEnt{
public:
    //-- 临时测试用 
    MemMapEnt()
        {
            this->majorGos.reserve(10);
        }

    inline void insert_2_majorGos( goid_t goid_ )noexcept{
        auto outPair = this->majorGos.insert( goid_ );
        tprAssert( outPair.second );
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
    


    inline const std::unordered_set<goid_t> &get_majorGos() const noexcept{return this->majorGos; }
    inline void erase_the_onlyOne_from_majorGos( goid_t goid_ )noexcept{
        size_t eraseNum = this->majorGos.erase(goid_);
        tprAssert( eraseNum == 1 );
    }
    
private:
    MapCoord  mcpos {}; //- 本 mapent 世界坐标值 

    MapAltitude   mapAlti     {}; //- 本 mapent 中点pix 的 alti

    sectionKey_t        ecoObjKey {};
    colorTableId_t      colorTableId {}; // same as ecoObj.colorTableId
    Density             density {};

    double  originPerlin {}; // [-1.0, 1.0]
    size_t  uWeight      {}; // [0, 9999]

    bool    isBorder     {false}; // 在未来，将被拓展为 一个 具体的数字，表示自己离 border 的距离（mapents）...

    std::unordered_set<goid_t> majorGos {}; // 只有启用 碰撞检测的，才会被登记于此。
                                            // 目前这道实现是不完善的

                                            // 对于那些关闭了 isMoveCollide，但是支持 技能碰撞检测的 go
                                            // 也应该提供 容器来登记
                                            // ...

};



#endif

