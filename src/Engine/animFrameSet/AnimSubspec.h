/*
 * ===================== AnimSubspec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ANIM_SUB_SPEC_H
#define TPR_ANIM_SUB_SPEC_H


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
#include "animSubspecId.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "tprCast.h"
#include "BrokenLvl.h"


//-- 亚种 --
//   保存并管理一组 animAction 实例
class AnimSubspec{
public:
    AnimSubspec()=default;

    inline AnimAction &insert_new_animAction(   NineDirection   dir_,
                                                BrokenLvl       brokenLvl_,
                                                const std::string &actionName_ )noexcept{
        // if target key is existed, nothing will happen
        this->animActions.insert({ dir_, std::unordered_map<BrokenLvl, std::unordered_map<std::string, std::unique_ptr<AnimAction>>>{} }); // maybe
        this->animActions.at(dir_).insert({ brokenLvl_, std::unordered_map<std::string, std::unique_ptr<AnimAction>>{} }); // maybe

        auto &container = this->animActions.at(dir_).at(brokenLvl_);
        auto outPair = container.insert({ actionName_, std::make_unique<AnimAction>() });
        tprAssert( outPair.second ); // Must
        //---
        // if target key is existed, nothing will happen
        this->actionsDirs.insert({ actionName_, std::unordered_set<NineDirection>{} }); //- maybe
        this->actionsDirs.at(actionName_).insert( dir_ ); //- maybe 
        //---
        return *(container.at(actionName_).get());
    }
        
    /*
    AnimAction *get_animActionPtr(   NineDirection   dir_,
                                            BrokenLvl       brokenLvl_,
                                            const std::string &actionName_ )const noexcept;
    */
    
    inline AnimAction *get_animActionPtr(   NineDirection   dir_,
                                            BrokenLvl       brokenLvl_,
                                            const std::string &actionName_ )const noexcept{
        tprAssert( this->animActions.find(dir_) != this->animActions.end() );
        auto &container_1 = this->animActions.at(dir_);
        //---
        tprAssert( container_1.find(brokenLvl_) != container_1.end() );
        auto &container_2 = container_1.at(brokenLvl_);
        //---
        tprAssert( container_2.find(actionName_) != container_2.end() );
        return container_2.at(actionName_).get();
    }
    
    

    inline const std::unordered_set<NineDirection> &get_actionDirs( const std::string &actionName_ )const noexcept{
        tprAssert( this->actionsDirs.find(actionName_) != this->actionsDirs.end() );
        return this->actionsDirs.at(actionName_);
    }
    
    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id
private:
    //-- 丑陋的实现，3层嵌套容器 
    std::unordered_map<NineDirection, 
        std::unordered_map<BrokenLvl,
            std::unordered_map<std::string, std::unique_ptr<AnimAction>>>> animActions {};

    std::unordered_map<std::string, std::unordered_set<NineDirection>> actionsDirs {};
};




//-- 一组亚种ids，相互间，拥有相同的 animLabel，不同的序号idx --
class AnimSubspecSquad{
public:
    AnimSubspecSquad()
        {
            this->subIdxs.reserve(4);
            this->subspecIds.reserve(4);
        }

    inline animSubspecId_t find_or_create( size_t subIdx_ )noexcept{
        if( this->subspecIds.find(subIdx_) == this->subspecIds.end() ){
            animSubspecId_t id = AnimSubspec::id_manager.apply_a_u32_id();
            this->subspecIds.insert({ subIdx_, id });
            this->subIdxs.push_back( subIdx_ );
            return id;
        }else{
            return this->subspecIds.at(subIdx_);
        }
    }

    inline animSubspecId_t apply_a_random_animSubspecId( size_t uWeight_ )const noexcept{
        if( this->subspecIds.size() == 1 ){
            return this->subspecIds.begin()->second; //- only one
        }
        size_t i = (uWeight_ + 366179) % this->subIdxs.size();
        return this->subspecIds.at( this->subIdxs.at(i) );
    }

    inline void check()const noexcept{
        tprAssert( !this->subIdxs.empty() );
        tprAssert( !this->subspecIds.empty() );
    }

private:
    std::vector<size_t> subIdxs {};
    std::unordered_map<size_t,animSubspecId_t> subspecIds {}; 
};





//-- 通过 animLabel 来管理 所有亚种 --
class AnimSubspecGroup{
public:
    AnimSubspecGroup()
        {
            this->labels.reserve(4);
            //this->labelKeys.reserve(4);
            this->subSquads.reserve(4);
        }

    /*
    inline animSubspecId_t find_or_create_a_animSubspecId(const std::vector<AnimLabel> &labels_, 
                                                                size_t            subIdx_ )noexcept{
        size_t labelSz = labels_.size();
        tprAssert( labelSz <= 2 );
        if( labelSz == 0 ){         return this->find_or_create_inn( AnimLabel::Default, AnimLabel::Default, subIdx_ );
        }else if( labelSz == 1 ){   return this->find_or_create_inn( labels_.at(0), AnimLabel::Default, subIdx_ );
        }else{                      return this->find_or_create_inn( labels_.at(0), labels_.at(1), subIdx_ );
        }
    }
    */

    //-- 空值需要传入 AnimLabel::Default
    inline animSubspecId_t find_or_create_a_animSubspecId( AnimLabel label_, size_t  subIdx_ )noexcept{
        if( this->subSquads.find(label_) == this->subSquads.end() ){
            auto outPair1 = this->subSquads.insert({ label_, AnimSubspecSquad{} });
            tprAssert( outPair1.second );
            //---
            this->labels.push_back( label_ );
        }
        return this->subSquads.at(label_).find_or_create( subIdx_ );
    }



    // param: uWeight_ [0, 9999]
    /*
    inline animSubspecId_t apply_a_random_animSubspecId(  const std::vector<AnimLabel> &labels_, 
                                                                size_t uWeight_ )noexcept{
            
        animLabelKey_t key {};
        size_t labelSz = labels_.size();
        tprAssert( labelSz <= 2 );
        if( labelSz == 0 ){        
            //-- 2个 label 都为随机值 --
            size_t fstI = (uWeight_ + 735157) % this->labels.size();
            AnimLabel &labelRef = this->labels.at( fstI );
            key = this->apply_random_secKey(this->labelKeys.at(labelRef), uWeight_);

        }else if( labelSz == 1 ){  
            //-- 第2个 label 为随机值 --
            key = this->apply_random_secKey(this->labelKeys.at(labels_.at(0)), uWeight_);
        }else{       
            //-- 2个 label 都为具体值 --
            key = animLabels_2_key(labels_.at(0), labels_.at(1));
        }
        auto &subSquadRef = this->subSquads.at(key);
        return subSquadRef.apply_a_random_animSubspecId( uWeight_ );
    }
    */
    inline animSubspecId_t apply_a_random_animSubspecId(  AnimLabel label_, size_t uWeight_ )noexcept{

        if( label_ == AnimLabel::Default ){
            size_t idx = (uWeight_ + 735157) % this->labels.size();
            AnimLabel tmpLabel = this->labels.at( idx );
            return this->subSquads.at(tmpLabel).apply_a_random_animSubspecId( uWeight_ );
        }else{
            tprAssert( this->subSquads.find(label_) != this->subSquads.end() );
            return this->subSquads.at(label_).apply_a_random_animSubspecId( uWeight_ );
        }
    }



    inline void check()noexcept{
        //--- labelKeys --
        //tprAssert( !this->labelKeys.empty() );
        //--- labels --
        /*
        for( const auto &i : this->labelKeys ){
            this->labels.push_back( i.first );
        }
        */

        tprAssert( !this->labels.empty() );

        //--- subSquads ---
        tprAssert( !this->subSquads.empty() );
        for( const auto &i : this->subSquads ){
            i.second.check();
        }
    }


private:

    /*
    inline animLabelKey_t apply_random_secKey( const std::vector<animLabelKey_t> &v_, size_t uWeight_ )noexcept{
        if( v_.size() == 1 ){
            return v_.at(0);
        }
        size_t i = (uWeight_ + 103171) % v_.size();
        return v_.at(i);
    }
    */

    /*
    inline animSubspecId_t find_or_create_inn( AnimLabel fstLabel_, AnimLabel secLabel_, size_t subIdx_ )noexcept{
        
        animLabelKey_t key = animLabels_2_key( fstLabel_, secLabel_ );

        if( this->subSquads.find(key) == this->subSquads.end() ){
            this->subSquads.insert({ key, AnimSubspecSquad{} });
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
        return this->subSquads.at(key).find_or_create( subIdx_ );
    }
    */

    
    std::vector<AnimLabel> labels {}; // 所有被登记的 labels


    //std::unordered_map<AnimLabel, std::vector<animLabelKey_t>> labelKeys {};
    //std::unordered_map<animLabelKey_t, AnimSubspecSquad> subSquads {};
    


    std::unordered_map<AnimLabel, AnimSubspecSquad> subSquads {};




};




#endif 

