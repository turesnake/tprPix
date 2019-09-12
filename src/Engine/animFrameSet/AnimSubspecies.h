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
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm> 


//-------------------- Engine --------------------//
#include "ID_Manager.h" 
#include "AnimAction.h"
#include "animSubspeciesId.h"
#include "AnimLabel.h"


//-- 亚种 --
//   保存并管理一组 animAction 实例
class AnimSubspecies{
public:
    AnimSubspecies()
        {
            this->animActions.reserve(4);
        }

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




//-- 一组亚种ids，相互间，拥有相同的 animLabels，不同的序号idx --
class AnimSubspeciesSquad{
public:
    AnimSubspeciesSquad()
        {
            this->subIdxs.reserve(4);
            this->subspeciesIds.reserve(4);
        }

    inline animSubspeciesId_t find_or_insert( size_t subIdx_ )noexcept{
        if( this->subspeciesIds.find(subIdx_) == this->subspeciesIds.end() ){
            animSubspeciesId_t id = AnimSubspecies::id_manager.apply_a_u32_id();
            this->subspeciesIds.insert({ subIdx_, id });
            this->subIdxs.push_back( subIdx_ );
            return id;
        }else{
            return this->subspeciesIds.at(subIdx_);
        }
    }

    inline animSubspeciesId_t apply_a_random_animSubspeciesId( double randVal_ )const noexcept{
        if( this->subspeciesIds.size() == 1 ){
            return this->subspeciesIds.begin()->second; //- only one
        }
        size_t i = static_cast<size_t>(floor(randVal_ * 3.1 + 17.7)) % this->subIdxs.size();
        return this->subspeciesIds.at( this->subIdxs.at(i) );
    }

    inline void check()const noexcept{
        tprAssert( !this->subIdxs.empty() );
        tprAssert( !this->subspeciesIds.empty() );
    }

private:
    std::vector<size_t> subIdxs {};
    std::unordered_map<size_t,animSubspeciesId_t> subspeciesIds {}; 
};





//-- 通过 animLabel 来管理 所有亚种 --
class AnimSubspeciesGroup{
public:
    AnimSubspeciesGroup()
        {
            this->labels.reserve(4);
            this->labelKeys.reserve(4);
            this->subSquads.reserve(4);
        }

    inline animSubspeciesId_t find_or_insert_a_animSubspeciesId(const std::vector<AnimLabel> &labels_, 
                                                                size_t            subIdx_ ){
        size_t labelSz = labels_.size();
        tprAssert( labelSz <= 2 );
        if( labelSz == 0 ){         return this->find_or_insert_inn( AnimLabel::Default, AnimLabel::Default, subIdx_ );
        }else if( labelSz == 1 ){   return this->find_or_insert_inn( labels_.at(0), AnimLabel::Default, subIdx_ );
        }else{                      return this->find_or_insert_inn( labels_.at(0), labels_.at(1), subIdx_ );
        }
    }

    inline animSubspeciesId_t apply_a_random_animSubspeciesId(  const std::vector<AnimLabel> &labels_, 
                                                                double             randVal_ ){
        animLabelKey_t key {};
        size_t labelSz = labels_.size();
        tprAssert( labelSz <= 2 );
        if( labelSz == 0 ){        
            //-- 2个 label 都为随机值 --
            size_t fstI = static_cast<size_t>(floor(randVal_ * 7.7 + 11.1)) % this->labels.size();
            AnimLabel &labelRef = this->labels.at( fstI );
            key = this->apply_random_secKey(this->labelKeys.at(labelRef), randVal_);

        }else if( labelSz == 1 ){  
            //-- 第2个 label 为随机值 --
            key = this->apply_random_secKey(this->labelKeys.at(labels_.at(0)), randVal_);
        }else{       
            //-- 2个 label 都为具体值 --
            key = animLabels_2_key(labels_.at(0), labels_.at(1));
        }
        auto &subSquadRef = this->subSquads.at(key);
        return subSquadRef.apply_a_random_animSubspeciesId( randVal_ );
    }


    inline void check()noexcept{
        //--- labelKeys --
        tprAssert( !this->labelKeys.empty() );
        //--- labels --
        for( const auto &i : this->labelKeys ){
            this->labels.push_back( i.first );
        }
        //--- subSquads ---
        tprAssert( !this->subSquads.empty() );
        for( const auto &i : this->subSquads ){
            i.second.check();
        }
    }


private:

    inline animLabelKey_t apply_random_secKey( const std::vector<animLabelKey_t> &v_, double randVal_ ){
        if( v_.size() == 1 ){
            return v_.at(0);
        }
        size_t i = static_cast<size_t>(floor(randVal_ * 5.7 + 13.3)) % v_.size();
        return v_.at(i);
    }

    
    inline animSubspeciesId_t find_or_insert_inn( AnimLabel fstLabel_, AnimLabel secLabel_, size_t subIdx_ )noexcept{
        
        animLabelKey_t key = animLabels_2_key( fstLabel_, secLabel_ );

        if( this->subSquads.find(key) == this->subSquads.end() ){
            this->subSquads.insert({ key, AnimSubspeciesSquad{} });
            //--- fstLabel ---
            if( this->labelKeys.find(fstLabel_) == this->labelKeys.end() ){
                this->labelKeys.insert({ fstLabel_, std::vector<animLabelKey_t>{} });
            }
            auto &fstV = this->labelKeys.at(fstLabel_);
            if( std::find(fstV.cbegin(), fstV.cend(), key) == fstV.cend() ){
                fstV.push_back( key );
            }
            //--- secLabel ---
            if( this->labelKeys.find(secLabel_) == this->labelKeys.end() ){
                this->labelKeys.insert({ secLabel_, std::vector<animLabelKey_t>{} });
            }
            auto &secV = this->labelKeys.at(secLabel_);
            if( std::find(secV.cbegin(), secV.cend(), key) == secV.cend() ){
                secV.push_back( key );
            }
        }
        return this->subSquads.at(key).find_or_insert( subIdx_ );
    }

    std::vector<AnimLabel> labels {};
    std::unordered_map<AnimLabel, std::vector<animLabelKey_t>> labelKeys {};
    std::unordered_map<animLabelKey_t, AnimSubspeciesSquad> subSquads {};
};



//-- 已废弃 --

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

