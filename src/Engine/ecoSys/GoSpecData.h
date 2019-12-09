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

        //  在未来，它可能跟随 整个旧版 mp-go 分配系统 被取消


class GoSpecData{
public:
    GoSpecData( goSpecId_t rootGoSpecId_,
                bool       isMultiGoMesh_,
                const std::string &afsName_,
                std::vector<AnimLabel> &animLabels_):
        rootGoSpecId(rootGoSpecId_),
        isMultiGoMesh(isMultiGoMesh_),
        afsName(afsName_)
        {
            tprAssert( !isMultiGoMesh_ );
            this->animLabels.swap( animLabels_ );
        }

    GoSpecData( goSpecId_t rootGoSpecId_,
                bool       isMultiGoMesh_,
                const std::string &afsName_,
                json::MultiGoMeshType multiGoMeshType_):
        rootGoSpecId(rootGoSpecId_),
        isMultiGoMesh(isMultiGoMesh_),
        afsName(afsName_),
        multiGoMeshType(multiGoMeshType_)
        {
            tprAssert( isMultiGoMesh_ );
        }

    //----- get -----//
    inline goSpecId_t   get_rootGoSpecId()const noexcept{ return this->rootGoSpecId; }
    inline bool         get_isMultiGoMesh()const noexcept{ return this->isMultiGoMesh; }
    inline const std::string &get_afsName()const noexcept{ return this->afsName; }
    

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

    std::string             afsName    {};
    std::vector<AnimLabel>  animLabels {};
    json::MultiGoMeshType   multiGoMeshType {};
};


#endif 

