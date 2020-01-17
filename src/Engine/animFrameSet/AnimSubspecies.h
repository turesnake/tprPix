/*
 * ===================== AnimSubspecies.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
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
#include "AnimActionEName.h"
#include "NineDirection.h"
#include "tprCast.h"
#include "BrokenLvl.h"


//-- 亚种 --
//   保存并管理一组 animAction 实例
class AnimSubspecies{
public:
    AnimSubspecies()=default;

    inline AnimAction &insert_new_animAction(   NineDirection   dir_,
                                                BrokenLvl       brokenLvl_,
                                                AnimActionEName actionEName_ )noexcept{
        // if target key is existed, nothing will happen
        this->animActions.insert({ dir_, std::unordered_map<BrokenLvl, std::unordered_map<AnimActionEName, std::unique_ptr<AnimAction>>>{} }); // maybe
        this->animActions.at(dir_).insert({ brokenLvl_, std::unordered_map<AnimActionEName, std::unique_ptr<AnimAction>>{} }); // maybe

        auto &container = this->animActions.at(dir_).at(brokenLvl_);
        auto outPair = container.insert({ actionEName_, std::make_unique<AnimAction>() });
        tprAssert( outPair.second ); // Must
        //---
        // if target key is existed, nothing will happen
        this->actionsDirs.insert({ actionEName_, std::unordered_set<NineDirection>{} }); //- maybe
        this->actionsDirs.at(actionEName_).insert( dir_ ); //- maybe 
        //---
        return *(container.at(actionEName_).get());
    }
        
    
    inline AnimAction *get_animActionPtr(   NineDirection   dir_,
                                            BrokenLvl       brokenLvl_,
                                            AnimActionEName actionEName_ )const noexcept{
        tprAssert( this->animActions.find(dir_) != this->animActions.end() );
        auto &container_1 = this->animActions.at(dir_);
        //---
        tprAssert( container_1.find(brokenLvl_) != container_1.end() );
        auto &container_2 = container_1.at(brokenLvl_);
        //---
        tprAssert( container_2.find(actionEName_) != container_2.end() );
        return container_2.at(actionEName_).get();
    }


    inline const std::unordered_set<NineDirection> &get_actionDirs( AnimActionEName actionEName_ )const noexcept{
        tprAssert( this->actionsDirs.find(actionEName_) != this->actionsDirs.end() );
        return this->actionsDirs.at(actionEName_);
    }
    
    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id
private:
    //-- 丑陋的实现，3层嵌套容器 
    std::unordered_map<NineDirection, 
        std::unordered_map<BrokenLvl,
            std::unordered_map<AnimActionEName, std::unique_ptr<AnimAction>>>> animActions {};

    std::unordered_map<AnimActionEName, std::unordered_set<NineDirection>> actionsDirs {};
};



//-- 一组亚种ids，相互间，拥有相同的 animLabel，不同的序号idx --
class AnimSubspeciesSquad{
public:
    AnimSubspeciesSquad()
        {
            this->subIdxs.reserve(4);
            this->subspeciesIds.reserve(4);
        }

    inline animSubspeciesId_t find_or_create( size_t subIdx_ )noexcept{
        if( this->subspeciesIds.find(subIdx_) == this->subspeciesIds.end() ){
            animSubspeciesId_t id = AnimSubspecies::id_manager.apply_a_u32_id();
            this->subspeciesIds.insert({ subIdx_, id });
            this->subIdxs.push_back( subIdx_ );
            return id;
        }else{
            return this->subspeciesIds.at(subIdx_);
        }
    }

    inline animSubspeciesId_t apply_a_random_animSubspeciesId( size_t uWeight_ )const noexcept{
        if( this->subspeciesIds.size() == 1 ){
            return this->subspeciesIds.begin()->second; //- only one
        }
        size_t i = (uWeight_ + 366179) % this->subIdxs.size();
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
            this->subSquads.reserve(4);
        }


    //-- 空值需要传入 AnimLabel::Default
    inline animSubspeciesId_t find_or_create_a_animSubspeciesId( AnimLabel label_, size_t  subIdx_ )noexcept{
        if( this->subSquads.find(label_) == this->subSquads.end() ){
            auto outPair1 = this->subSquads.insert({ label_, AnimSubspeciesSquad{} });
            tprAssert( outPair1.second );
            //---
            this->labels.push_back( label_ );
        }
        return this->subSquads.at(label_).find_or_create( subIdx_ );
    }


    // param: uWeight_ [0, 9999]
    inline animSubspeciesId_t apply_a_random_animSubspeciesId(  AnimLabel label_, size_t uWeight_ )noexcept{
        if( label_ == AnimLabel::Default ){
            size_t idx = (uWeight_ + 735157) % this->labels.size();
            AnimLabel tmpLabel = this->labels.at( idx );
            return this->subSquads.at(tmpLabel).apply_a_random_animSubspeciesId( uWeight_ );
        }else{
            tprAssert( this->subSquads.find(label_) != this->subSquads.end() );
            return this->subSquads.at(label_).apply_a_random_animSubspeciesId( uWeight_ );
        }
    }


    inline void check()noexcept{
        tprAssert( !this->labels.empty() );
        //--- subSquads ---
        tprAssert( !this->subSquads.empty() );
        for( const auto &i : this->subSquads ){
            i.second.check();
        }
    }


private:
    std::vector<AnimLabel> labels {}; // 所有被登记的 labels
    std::unordered_map<AnimLabel, AnimSubspeciesSquad> subSquads {};
};




#endif 

