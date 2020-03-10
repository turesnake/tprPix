/*
 * ========================== Job_MapEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_MAP_ENT_H
#define TPR_JOB_MAP_ENT_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "chunkKey.h"
#include "fieldKey.h"
#include "colorTableId.h"
#include "Density.h"
#include "MapAltitude.h"
#include "GoSpecData.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupState.h"


// need:
class MemMapEnt;

// tmp data. created in job threads
class Job_MapEnt{
public:
    explicit Job_MapEnt( IntVec2 mpos_ )
        {
            this->init( mpos_ );// 并不完整，还有一部分在 calc_job_chunk() 中完成
        }
    
    //---------- init ----------//
    inline void init_ecoObjKey( sectionKey_t key_ )noexcept{ tprAssert( !this->ecoObjKey.has_value() ); this->ecoObjKey = {key_}; }
    inline void init_colorTableId( colorTableId_t id_ )noexcept{ tprAssert( !this->colorTableId.has_value() ); this->colorTableId = {id_}; }
    inline void init_density( Density density_ )noexcept{ tprAssert( !this->density.has_value() ); this->density = {density_}; }
    inline void init_isEcoBorder( bool b_ )noexcept{ tprAssert( !this->isEcoBorder.has_value() ); this->isEcoBorder = {b_}; }

    //---------- get ----------//
    inline IntVec2          get_mpos()const noexcept{ tprAssert( this->mpos.has_value() ); return this->mpos.value(); }
    inline chunkKey_t       get_chunkKey()const noexcept{ tprAssert( this->chunkKey.has_value() ); return this->chunkKey.value(); }
    inline sectionKey_t     get_ecoObjKey()const noexcept{ tprAssert( this->ecoObjKey.has_value() ); return this->ecoObjKey.value(); }
    inline colorTableId_t   get_colorTableId()const noexcept{ tprAssert( this->colorTableId.has_value() ); return this->colorTableId.value(); }
    inline Density          get_density()const noexcept{ tprAssert( this->density.has_value() ); return this->density.value(); }
    inline MapAltitude      get_alti()const noexcept{ tprAssert( this->alti.has_value() ); return this->alti.value(); }
    inline gameObjs::bioSoup::State     get_bioSoupState()const noexcept{ tprAssert( this->bioSoupState.has_value() ); return this->bioSoupState.value(); }
    inline size_t           get_uWeight()const noexcept{ tprAssert( this->uWeight.has_value() ); return this->uWeight.value(); }
    inline bool             get_isEcoBorder()const noexcept{ tprAssert( this->isEcoBorder.has_value() ); return this->isEcoBorder.value(); }

private:
    void init( IntVec2 mpos_ )noexcept;

    //====== vals ======//
    // 确保每个数据，只被 init一次
    std::optional<IntVec2>                  mpos            {std::nullopt};
    std::optional<chunkKey_t>               chunkKey        {std::nullopt};
    std::optional<sectionKey_t>             ecoObjKey       {std::nullopt};
    std::optional<colorTableId_t>           colorTableId    {std::nullopt}; // same as ecoObj.colorTableId
    std::optional<Density>                  density         {std::nullopt};
    std::optional<MapAltitude>              alti            {std::nullopt};
    std::optional<gameObjs::bioSoup::State> bioSoupState    {std::nullopt};
    std::optional<size_t>                   uWeight         {std::nullopt}; // [0, 9999]
    std::optional<bool>                     isEcoBorder     {std::nullopt}; //- 是否为 eco边缘go default:false
                             // 在未来，将被拓展为 一个 具体的数字，表示自己离 border 的距离（mapents）...
}; 



#endif 

