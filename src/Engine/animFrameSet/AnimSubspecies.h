/*
 * ===================== AnimSubspecies.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ANIM_SUB_SPECIES_H
#define TPR_ANIM_SUB_SPECIES_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <algorithm> 

//-------------------- Engine --------------------//
#include "ID_Manager.h" 
#include "AnimAction.h"
#include "animSubspeciesId.h"
#include "AnimActionEName.h"
#include "NineDirection.h"
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
        auto [insertIt, insertBool] = container.emplace( actionEName_, std::make_unique<AnimAction>() );
        tprAssert( insertBool ); // Must
        //---
        // if target key is existed, nothing will happen
        this->actionsDirs.insert({ actionEName_, std::unordered_set<NineDirection>{} }); //- maybe
        this->actionsDirs.at(actionEName_).insert( dir_ ); //- maybe 
        //---
        return *(container.at(actionEName_).get());
    }
        

    // 三层参数都有可能出错，用 opt 来管理。让外部处理
    inline std::optional<AnimAction*> get_animActionPtr(    NineDirection   dir_,
                                                            BrokenLvl       brokenLvl_,
                                                            AnimActionEName actionEName_ )const noexcept{

        if( this->animActions.find(dir_) == this->animActions.end() ){ return std::nullopt; }
        auto &container_1 = this->animActions.at(dir_);
        //---
        if( container_1.find(brokenLvl_) == container_1.end() ){ return std::nullopt; }
        auto &container_2 = container_1.at(brokenLvl_);
        //---
        if( container_2.find(actionEName_) == container_2.end() ){ return std::nullopt; }
        return { container_2.at(actionEName_).get() };
    }
    
    inline const std::unordered_set<NineDirection> &get_actionDirs( AnimActionEName actionEName_ )const noexcept{
        tprAssert( this->actionsDirs.find(actionEName_) != this->actionsDirs.end() );
        return this->actionsDirs.at(actionEName_);
    }

    inline static animSubspeciesId_t apply_new_animSubspeciesId()noexcept{
        animSubspeciesId_t id = AnimSubspecies::id_manager.apply_a_u32_id();
        if( is_empty_animSubspeciesId(id) ){
            id = AnimSubspecies::id_manager.apply_a_u32_id(); // do apply again
        }
        return id;
    }

private:
    //-- 丑陋的实现，3层嵌套容器 
    std::unordered_map<NineDirection, 
        std::unordered_map<BrokenLvl,
            std::unordered_map<AnimActionEName, std::unique_ptr<AnimAction>>>> animActions {};

    std::unordered_map<AnimActionEName, std::unordered_set<NineDirection>> actionsDirs {};

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id
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
            animSubspeciesId_t id = AnimSubspecies::apply_new_animSubspeciesId();
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
    inline animSubspeciesId_t find_or_create_a_animSubspeciesId( const std::string & label_, size_t  subIdx_ )noexcept{
        if( this->subSquads.find(label_) == this->subSquads.end() ){
            auto [insertIt, insertBool] = this->subSquads.emplace( label_, AnimSubspeciesSquad{} );
            tprAssert( insertBool );
            //---
            this->labels.push_back( label_ );
        }
        return this->subSquads.at(label_).find_or_create( subIdx_ );
    }


    // param: uWeight_ [0, 9999]
    inline animSubspeciesId_t apply_a_random_animSubspeciesId(  const std::string & label_, size_t uWeight_ )noexcept{
        if( label_ == "" ){
            size_t idx = (uWeight_ + 735157) % this->labels.size();
            const std::string &tmpLabel = this->labels.at(idx);
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
    std::vector<std::string> labels {}; // 所有被登记的 labels.  ( "" == default )
    std::unordered_map<std::string, AnimSubspeciesSquad> subSquads {};
};




#endif 

