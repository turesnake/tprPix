/*
 * ========================= GoSpecData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_SPEC_DATA_H
#define TPR_GO_SPEC_DATA_H

//-------------------- CPP --------------------//
#include <vector>

//------------------- Engine --------------------//
#include "AnimLabel.h"
#include "GameObjType.h"


//-- 仅用于 ecoObj -> create a go 
class GoSpecData{
public:
    GoSpecData()=default;
    GoSpecData( goSpecId_t specId_,
                const std::vector<AnimLabel> &labels_ ):
        specId(specId_)
        {
            this->animLabels.insert( this->animLabels.end(), labels_.begin(), labels_.end() );
        }
    inline goSpecId_t get_goSpecId()const noexcept{ return this->specId; }
    inline const std::vector<AnimLabel> &get_animLabels()const noexcept{ return this->animLabels; }
private:
    goSpecId_t specId {};
    std::vector<AnimLabel> animLabels {};
};


#endif 

