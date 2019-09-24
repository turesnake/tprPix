/*
 * ======================= ColorTable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.021
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

    //======== static ========//
    inline static size_t get_dataSize()noexcept{ 
        return (colorTableEntNames.size()*sizeof(FloatVec4)); 
    }

    static ID_Manager  id_manager;
private:
    std::vector<FloatVec4> data {}; // colorTable
    std::unordered_set<std::string> isSets {};
};
//============== static ===============//
inline ID_Manager  ColorTable::id_manager { ID_TYPE::U32, 0};



class ColorTableSet{
public:
    ColorTableSet()=default;

    inline ColorTable &apply_new_colorTable( const std::string &name_ )noexcept{
        tprAssert( !this->isFindIn_name_ids(name_) );
        colorTableId_t id = ColorTable::id_manager.apply_a_u32_id();
        tprAssert( this->colorTableUPtrs.find(id) == this->colorTableUPtrs.end() );
        //---
        this->name_ids.insert({ name_, id });
        this->colorTableUPtrs.insert({ id, std::make_unique<ColorTable>() });
        //---
        return *(this->colorTableUPtrs.at(id).get());
    }

    inline void final_check()const noexcept{
        tprAssert( this->isFindIn_name_ids("origin") );// Must Have!!!
    }

    inline colorTableId_t get_colorTableId( const std::string &colorTableName_ )const noexcept{
            tprAssert( this->isFindIn_name_ids(colorTableName_) );
        return this->name_ids.at(colorTableName_);
    }

    inline const ColorTable &get_colorTableRef( colorTableId_t id_ )const noexcept{
            tprAssert( this->colorTableUPtrs.find(id_) != this->colorTableUPtrs.end() );
        return *(this->colorTableUPtrs.at(id_).get());
    };


private:
    inline bool isFindIn_name_ids( const std::string name_ )const noexcept{
        return (this->name_ids.find(name_) != this->name_ids.end());
    }

    //----- vals -----//
    std::unordered_map<std::string, colorTableId_t> name_ids {};
    std::unordered_map<colorTableId_t, std::unique_ptr<ColorTable>> colorTableUPtrs {};
            // 0:    origin colorTable
            // 1~n:  ent 
};






#endif 

