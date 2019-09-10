/*
 * =================== AnimSubspecies.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 */
#ifndef TPR_ANIM_SUB_SPECIES_H
#define TPR_ANIM_SUB_SPECIES_H


//-------------------- CPP --------------------//
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>


//-------------------- Engine --------------------//
#include "ID_Manager.h" 
#include "AnimAction.h"
#include "animSubspeciesId.h"


class AnimSubspecies{
public:
    AnimSubspecies()=default;

    inline AnimAction &insert_new_animAction( const std::string &actionName_ ){
            tprAssert( this->animActions.find(actionName_) == this->animActions.end() );
        this->animActions.insert({ actionName_, std::make_unique<AnimAction>() });
        return *(this->animActions.at(actionName_).get());
    }
    inline AnimAction *get_animActionPtr( const std::string &actionName_ ){
            tprAssert( this->animActions.find(actionName_) != this->animActions.end() );
        return this->animActions.at(actionName_).get();
    }
    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id
private:
    std::unordered_map<std::string, std::unique_ptr<AnimAction>> animActions {};
};
//============== static ===============//
inline ID_Manager  AnimSubspecies::id_manager { ID_TYPE::U32, 1};




class AnimSubspeciesWrap{
public:
    AnimSubspeciesWrap()=default;

    inline animSubspeciesId_t apply_a_random_animSubspeciesId( double randVal_ ){
        size_t i = static_cast<size_t>(floor(randVal_ * 3.1 + 17.7)) % this->subIdxs.size();
        return this->subIds.at( this->subIdxs.at(i) );
    }

    std::vector<size_t> subIdxs {};
    std::unordered_map<size_t,animSubspeciesId_t> subIds {}; 

};





#endif 

