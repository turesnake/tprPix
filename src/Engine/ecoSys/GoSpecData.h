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
#include "GameObjType.h"

#include "GoAssemblePlan.h"


//--------------- Script ------------------//


//-- 仅用于 ecoObj -> create a go 

        //  这个名字太糟糕了，应该被修改

        //  在未来，它可能跟随 整个旧版 mp-go 分配系统 被取消


class GoSpecData{
public:
    GoSpecData( goSpeciesId_t rootGoSpeciesId_,
                const std::string &afsName_,
                goLabelId_t     goLabelId_):
        rootGoSpeciesId(rootGoSpeciesId_),
        afsName(afsName_),
        goLabelId(goLabelId_)
        {}

    //----- get -----//
    inline goSpeciesId_t        get_rootGoSpeciesId()const noexcept{ return this->rootGoSpeciesId; }
    inline const std::string    &get_afsName()const noexcept{ return this->afsName; }
    inline const std::string    &get_animLabel()const noexcept{ return this->animLabel; }

    inline goLabelId_t get_goLabelId()const noexcept{
        return this->goLabelId;
    }


private:
    goSpeciesId_t              rootGoSpeciesId {};

    std::string             afsName    {};
    std::string             animLabel {};
    goLabelId_t             goLabelId {};
};


#endif 

