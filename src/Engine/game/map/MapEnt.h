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


//-- 投影地图单位的信息 64*64 pixes --
class MemMapEnt{
public:
    //-- 临时测试用 
    MemMapEnt()
        {
            this->majorGos.reserve(10);
        }


    inline void set_mapAlti( MapAltitude alti_ )noexcept{ this->mapAlti = alti_; }
    inline void set_mcpos( const MapCoord &mcpos_ )noexcept{ this->mcpos = mcpos; }

    inline const IntVec2 &get_mpos() const noexcept{ return this->mcpos.get_mpos();}
    inline MapAltitude get_mapAlti() const noexcept{ return this->mapAlti; };

    inline void insert_2_majorGos( goid_t goid_ )noexcept{
        tprAssert( this->majorGos.find(goid_) == this->majorGos.end() );
        this->majorGos.insert({ goid_ });
    }
    inline const std::unordered_set<goid_t> &get_majorGos() const noexcept{
        return this->majorGos;
    }
    inline void erase_the_onlyOne_from_majorGos( goid_t goid_ )noexcept{
        size_t eraseNum = this->majorGos.erase(goid_);
        tprAssert( eraseNum == 1 );
    }
    
private:
    MapCoord  mcpos {}; //- 本 mapent 世界坐标值 

    MapAltitude   mapAlti     {}; //- 本 mapent 中点pix 的 alti

    //EcoSysPlanType  ecoSysPlanType  {EcoSysPlanType::Forest};

            //-- 这个值暂时未被配置，尽管在理想态，每个 mapent，应该知道自己的 eco 类型
            //   mapent 的 ecotype 信息是否有用 ???
            //   ....

    std::unordered_set<goid_t> majorGos {};

};



#endif

