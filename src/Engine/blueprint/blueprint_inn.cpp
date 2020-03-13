/*
 * ======================= blueprint_inn.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "blueprint_inn.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace blueP_inn {//----------- namespace: blueP_inn ----------------//


yardBlueprintId_t VarType_Village_Manager::VarType::apply_a_yardBlueprintId( size_t uWeight_, NineDirection yardDir_ )noexcept{
        
    auto ret = this->vtPtr->apply_rand_yardBlueprintId( uWeight_, yardDir_ );
    if( !ret.has_value() ){
        tprDebug::console( "can't find yardDir_ = ", nineDirection_2_str( yardDir_ ) );
        tprAssert(0);
    }

    yardBlueprintId_t yardId = ret.value();

    if( this->isAllInstanceUseSamePlan ){
        //-- 统一值 只生成一次，并永久保留
        if( !this->isUnifiedValsset ){
            this->isUnifiedValsset = true;
            this->unifiedYardId  = yardId;
        }
        //-- 直接获得 统一值
        return this->unifiedYardId;
    }else{
        //-- 每次都独立分配 yardId --
        return yardId;
    }
}



}//-------------- namespace: blueP_inn end ----------------//
}//--------------------- namespace: blueprint end ------------------------//

