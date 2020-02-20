/*
 * ====================== WeakMapEntSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_WEAK_MAP_ENT_SET_H
#define TPR_WEAK_MAP_ENT_SET_H

//--------------- CPP ------------------//
#include <unordered_map>
#include <vector>

//--------------- Engine ------------------//
#include "WeakMapEntsType.h"
#include "IntVec.h"
#include "tprAssert.h"



class WeakMapEntSet{
public:

    static inline const std::vector<IntVec2> &get_WeakMapEntOffs( WeakMapEntsType type_ )noexcept{
        if( !WeakMapEntSet::isStaticInit ){
            WeakMapEntSet::isStaticInit = true;
            WeakMapEntSet::init_for_static();
        }
        tprAssert( WeakMapEntSet::datas.find(type_) != WeakMapEntSet::datas.end() );
        return WeakMapEntSet::datas.at(type_);
    }


private:
    static void init_for_static()noexcept;
    //----- vals -----//
    static std::unordered_map<WeakMapEntsType, std::vector<IntVec2>> datas;
    static bool isStaticInit;
};





#endif 

