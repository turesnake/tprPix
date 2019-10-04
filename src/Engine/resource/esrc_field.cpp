/*
 * ======================= esrc_field.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- CPP --------------------//
#include <unordered_map>
#include <mutex>
#include <shared_mutex> //- c++17 读写锁
#include <set>
#include <memory>


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_field.h"
#include "config.h"
#include "chunkKey.h"

#include "esrc_ecoObj.h"
#include "esrc_gameObj.h" 
#include "esrc_chunk.h" 
#include "esrc_mapSurfaceRand.h"
#include "esrc_state.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h"



#include "tprDebug.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace field_inn {//------------ namespace: field_inn --------------//

    //-- cross threads --
    std::unordered_map<fieldKey_t,std::unique_ptr<MapField>> fields {};
    std::shared_mutex  fieldsSharedMutex; //- 读写锁

                        // 是否可以让 fields 变成 主线程专用
                        // job线程 仅生成数据 ...

    //-- 正在创建的 field 表 --
    std::set<fieldKey_t> fieldsBuilding {};
    std::mutex  fieldsBuildingMutex;

    //===== funcs =====//
    void insert_2_fieldsBuilding( fieldKey_t fieldKey_ );
    bool is_in_fieldsBuilding( fieldKey_t fieldKey_ );
    void erase_from_fieldsBuilding( fieldKey_t fieldKey_ );

    bool is_find_in_fields_( fieldKey_t _key ){
        return (field_inn::fields.find(_key) != field_inn::fields.end());
    }

}//---------------- namespace: field_inn end --------------//


void init_fields(){
    field_inn::fields.reserve(10000);
    esrc::insertState("field");
}


/* ===========================================================
 *      atom_try_to_insert_and_init_the_field_ptr   [-WRITE-]
 * -----------------------------------------------------------
 * 检测是否存在，若不存在，生成之。
 */
void atom_try_to_insert_and_init_the_field_ptr( IntVec2 fieldMPos_ ){

    fieldKey_t fieldKey = fieldMPos_2_fieldKey( fieldMPos_ );
    //--- lock---//
    std::unique_lock<std::shared_mutex> ul( field_inn::fieldsSharedMutex ); //- write -
    if( field_inn::is_find_in_fields_(fieldKey) ||
        ( field_inn::is_in_fieldsBuilding(fieldKey) ) ){
        return;
    }
    field_inn::insert_2_fieldsBuilding( fieldKey );
    
        //--- unlock ---//
        ul.unlock();
        auto fieldUPtr = std::make_unique<MapField>();
        fieldUPtr->init( fieldMPos_ );
                //-- 这里耗时有点长, 所以在 解锁状态运行
                //   这样就不会耽误 其他线程 对 全局容器的 访问

    //--- lock ---//
    ul.lock();
    auto outPair = field_inn::fields.insert({ fieldKey, std::move(fieldUPtr) });
    tprAssert( outPair.second ); //- MUST NOT EXIST
    field_inn::erase_from_fieldsBuilding( fieldKey );    
}


/* ===========================================================
 *      atom_erase_all_fields_in_chunk   [-WRITE-]
 * -----------------------------------------------------------
 * 遍历 chunk 中所有 field， 直接删除这些实例
 */
void atom_erase_all_fields_in_chunk( IntVec2 chunkMPos_ ){

    //--- lock---//
    std::unique_lock<std::shared_mutex> ul( field_inn::fieldsSharedMutex ); //- write -
    
    //--- unlock ---//
    ul.unlock();
    IntVec2    tmpFieldMPos {};
    fieldKey_t tmpFieldKey {};
    size_t     eraseNum {};

    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){
            tmpFieldMPos.set(   chunkMPos_.x + w*ENTS_PER_FIELD,
                                chunkMPos_.y + h*ENTS_PER_FIELD );
            tmpFieldKey = fieldMPos_2_fieldKey( tmpFieldMPos );

            //--- lock ---//
            ul.lock();
            eraseNum = field_inn::fields.erase(tmpFieldKey);
            tprAssert( eraseNum == 1 );

            //--- unlock ---//
            ul.unlock();
        }
    }
}



/* ===========================================================
 *           atom_write_2_field_from_jobData     [-WRITE-]
 * -----------------------------------------------------------
 * data from ChunkData, write to field 
 */
void atom_write_2_field_from_jobData(   fieldKey_t fieldKey_, 
                                    sectionKey_t ecoObjkey_,
                                    colorTableId_t id_,
                                    Density d_,
                                    MapAltitude nodeAlti_,
                                    MapAltitude minAlti_,
                                    MapAltitude maxAlti_,
                                    bool        isCrossEcoObj_,
                                    bool        isCrossColorTable_ ){
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( field_inn::fieldsSharedMutex ); //- write -
        tprAssert( field_inn::is_find_in_fields_(fieldKey_) ); //- MUST EXIST
        auto *fieldPtr = field_inn::fields.at(fieldKey_).get();
        //---
        fieldPtr->set_ecoObjKey( ecoObjkey_ );
        fieldPtr->set_colorTableId( id_ );
        fieldPtr->set_density( d_ );
        fieldPtr->set_nodeMapAlti( nodeAlti_ );
        fieldPtr->set_minAlti( minAlti_ );
        fieldPtr->set_maxAlti( maxAlti_ );
        fieldPtr->set_isCrossEcoObj( isCrossEcoObj_ );
        fieldPtr->set_isCrossColorTable( isCrossColorTable_ );
    }
}


/* ===========================================================    tmp.......
 *              atom_get_field     [-READ-]
 * -----------------------------------------------------------
 *     debug 用.....
 */
const MapField &atom_get_field( fieldKey_t fieldKey_ ){
    //--- atom ---//
    std::shared_lock<std::shared_mutex> sl( field_inn::fieldsSharedMutex ); //- read -
    tprAssert( field_inn::is_find_in_fields_(fieldKey_) ); //- MUST EXIST
    return *(field_inn::fields.at( fieldKey_ ));
}




namespace field_inn {//------------ namespace: field_inn --------------//



void insert_2_fieldsBuilding( fieldKey_t fieldKey_ ){
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( fieldsBuildingMutex );
        auto outPair = fieldsBuilding.insert( fieldKey_ );
        tprAssert( outPair.second );
    }
}
bool is_in_fieldsBuilding( fieldKey_t fieldKey_ ){
    bool ret {};
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( fieldsBuildingMutex );
        ret = fieldsBuilding.find(fieldKey_) != fieldsBuilding.end();
    }
    return ret;
}
void erase_from_fieldsBuilding( fieldKey_t fieldKey_ ){
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( fieldsBuildingMutex );
        size_t eraseNum = fieldsBuilding.erase(fieldKey_);
        tprAssert( eraseNum == 1 );
    }
}




}//---------------- namespace: field_inn end --------------//
}//---------------------- namespace: esrc end -------------------------//

