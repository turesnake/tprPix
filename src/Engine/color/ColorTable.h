/*
 * ======================= ColorTable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  数组色卡，管理 游戏世界 中的所有颜色 
 * ---------
 */
#ifndef TPR_COLOR_TABLE_H
#define TPR_COLOR_TABLE_H

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "FloatVec.h"
#include "ID_Manager.h" 
#include "colorTableId.h"

//-- colorTable.data 中实际存储的 entName --
inline std::unordered_map<std::string, size_t> colorTableEntNames{
    //-- 基础色 --
    { "base_0", 0 },
    { "base_1", 1 },
    { "base_2", 2 },
    { "base_3", 3 },
    { "base_4", 4 },
    { "base_5", 5 },
    { "base_6", 6 },
    { "base_7", 7 },
    //-- 地面色 --
    { "ground", 8 },

    //-- water --
    /*
    { "water_0", 9 },
    { "water_1", 10 },
    { "water_2", 11 },
    { "water_3", 12 }
    */
    // more ...
};

class ColorTable{
public:
    ColorTable()
        {
            this->data.resize(colorTableEntNames.size());
        }

    template< typename PtrType >
    inline PtrType get_dataPtr()const noexcept{
        return static_cast<PtrType>( &(this->data.at(0)) );
    }

    inline std::vector<FloatVec4> &getnc_dataRef()noexcept{ return this->data; }
    inline const std::vector<FloatVec4> &get_dataRef()const noexcept{ return this->data; }

    inline void insert_a_color(const std::string &name_, 
                                const FloatVec4 &color_ )noexcept{
        tprAssert( colorTableEntNames.find(name_) != colorTableEntNames.end() );
        tprAssert( this->isSets.find(name_) == this->isSets.end() );
        this->data.at(colorTableEntNames.at(name_)) = color_;
        this->isSets.insert( name_ );
    }

    inline void final_check()const noexcept{
        for( const auto &pair : colorTableEntNames ){
            tprAssert( this->isSets.find(pair.first) != this->isSets.end() );
        }
    }

    //- Must called in init --
    inline void init_all_color_white()noexcept{
        tprAssert( this->isSets.empty() );
        for( const auto &pair : colorTableEntNames ){
            tprAssert( this->isSets.find(pair.first) == this->isSets.end() );
            this->isSets.insert( pair.first );
            //---
            this->data.at(pair.second) = { 1.0, 1.0, 1.0, 1.0 };//- white
        }
    }

    inline const FloatVec4 &get_groundColor()const noexcept{
        return this->data.at(colorTableEntNames.at("ground"));
    }

    //======== static ========//
    inline static size_t get_dataSize()noexcept{ 
        return (colorTableEntNames.size()*sizeof(FloatVec4)); 
    }

    static ID_Manager  id_manager;
private:
    std::vector<FloatVec4> data {}; // colorTable
    std::unordered_set<std::string> isSets {}; // just used in json-read
};



class ColorTableSet{
public:
    ColorTableSet()=default;

    inline ColorTable &apply_new_colorTable( const std::string &name_ )noexcept{
        tprAssert( !this->isFindIn_name_ids(name_) );
        colorTableId_t id = ColorTable::id_manager.apply_a_u32_id();// start from 1
        tprAssert( this->colorTableUPtrs.find(id) == this->colorTableUPtrs.end() );
        //---
        this->name_ids.insert({ name_, id });
        this->colorTableUPtrs.insert({ id, std::make_unique<ColorTable>() });        
        //---
        return *(this->colorTableUPtrs.at(id).get());
    }

    inline void final_check()noexcept{
        tprAssert( this->isFindIn_name_ids("origin") );// Must Have!!!
        //--- ground color table ---//
        //-- fst ent will be skip --
        this->groundColorTable.resize( this->colorTableUPtrs.size() + 1 );
        for( const auto &pair : this->colorTableUPtrs ){
            this->groundColorTable.at(pair.first) = pair.second->get_groundColor();
        }
        //---
        this->debug();
    }

    inline colorTableId_t get_colorTableId( const std::string &colorTableName_ )const noexcept{
            tprAssert( this->isFindIn_name_ids(colorTableName_) );
        return this->name_ids.at(colorTableName_);
    }

    inline const std::unordered_map<std::string, colorTableId_t> &
    get_name_idsRef()const noexcept{
        return this->name_ids;
    }

    inline const ColorTable &get_colorTableRef( colorTableId_t id_ )const noexcept{
            tprAssert( this->colorTableUPtrs.find(id_) != this->colorTableUPtrs.end() );
        return *(this->colorTableUPtrs.at(id_).get());
    }
    inline const ColorTable *get_colorTablePtr( colorTableId_t id_ )const noexcept{
            tprAssert( this->colorTableUPtrs.find(id_) != this->colorTableUPtrs.end() );
        return this->colorTableUPtrs.at(id_).get();
    }

    //----- groundColor -----//
    template< typename PtrType >
    inline PtrType get_groundColor_dataPtr()const noexcept{
        return static_cast<PtrType>( &(this->groundColorTable.at(0)) );
    }

    inline size_t get_groundColor_dataSize()const noexcept{ 
        return (this->groundColorTable.size() * sizeof(FloatVec4));
    }

    //--- JUST used in TEST !!!!!!!
    inline const std::vector<FloatVec4> &get_groundColorTable_test()const noexcept{
        return this->groundColorTable;
    }


private:
    inline bool isFindIn_name_ids( const std::string name_ )const noexcept{
        return (this->name_ids.find(name_) != this->name_ids.end());
    }

    void debug()noexcept;

    //----- vals -----//
    std::unordered_map<std::string, colorTableId_t> name_ids {};
    std::unordered_map<colorTableId_t, std::unique_ptr<ColorTable>> colorTableUPtrs {};
    std::vector<FloatVec4> groundColorTable {}; // idx by colorTableId
            // 0:    empty, skip it !!!
            // 1:    origin colorTable
            // 2~n:  ent 
};




//-- 维护一个动态的 ColorTable 实例，表示当前帧的 世界颜色 --
//   可以让它趋近于某种目标色
class CurrentColorTable{
public:
    CurrentColorTable()
        {
            this->data.init_all_color_white();
        }

    inline void rebind_target( const ColorTable *targetPtr_ )noexcept{
        if( this->targetColorTablePtr != targetPtr_ ){
            this->targetColorTablePtr = targetPtr_;
            this->isWorking = true;
        }
    }

    void update()noexcept;

    inline const ColorTable &get_colorTableRef()const noexcept{
        return this->data;
    }

    inline bool get_isWorking()const noexcept{ return this->isWorking; }

private:
    ColorTable data {};
    const ColorTable *targetColorTablePtr {nullptr};

    //===== flags =====//
    bool isWorking {true}; // 本色是否正在趋近于 目标色

};

#endif 

