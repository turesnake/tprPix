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


//--------------- Script ------------------//
#include "Script/json/json_multiGoMesh.h"


//-- 仅用于 ecoObj -> create a go 

        //  这个名字太糟糕了，应该被修改

class GoSpecData{
public:
    GoSpecData( goSpecId_t rootGoSpecId_,
                bool       isMultiGoMesh_,
                std::vector<AnimLabel> &animLabels_):
        rootGoSpecId(rootGoSpecId_),
        isMultiGoMesh(isMultiGoMesh_)
        {
            tprAssert( !isMultiGoMesh_ );
            this->animLabels.swap( animLabels_ );
        }

    GoSpecData( goSpecId_t rootGoSpecId_,
                bool       isMultiGoMesh_,
                json::MultiGoMeshType multiGoMeshType_):
        rootGoSpecId(rootGoSpecId_),
        isMultiGoMesh(isMultiGoMesh_),
        multiGoMeshType(multiGoMeshType_)
        {
            tprAssert( isMultiGoMesh_ );
        }

    //----- get -----//
    inline goSpecId_t   get_rootGoSpecId()const noexcept{ return this->rootGoSpecId; }
    inline bool         get_isMultiGoMesh()const noexcept{ return this->isMultiGoMesh; }

    inline const std::vector<AnimLabel> &get_animLabels()const noexcept{ 
        tprAssert( !this->isMultiGoMesh );
        return this->animLabels; 
    }

    inline json::MultiGoMeshType get_multiGoMeshType()const noexcept{
        tprAssert( this->isMultiGoMesh );
        return this->multiGoMeshType;
    }


private:
    goSpecId_t              rootGoSpecId {};

    bool                    isMultiGoMesh;
    std::vector<AnimLabel>  animLabels {};
    json::MultiGoMeshType   multiGoMeshType {};
};


#endif 

