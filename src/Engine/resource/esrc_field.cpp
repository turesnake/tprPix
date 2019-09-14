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
#include "esrc_mapSurfaceRandSet.h"


//-------------------- Script --------------------//
#include "Script/resource/ssrc.h"
#include "Script/gameObjs/create_goes.h"


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
}


/* ===========================================================
 *      atom_try_to_insert_and_init_the_field_ptr   [-WRITE-]
 * -----------------------------------------------------------
 * 检测是否存在，若不存在，生成之。
 * ----
 * 展示了如何使用 unique_lock 来实现 实例init。
 * ----
 */
void atom_try_to_insert_and_init_the_field_ptr( const IntVec2 &fieldMPos_ ){

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
        tprAssert( field_inn::is_find_in_fields_(fieldKey) == false ); //- MUST NOT EXIST
    field_inn::fields.insert({ fieldKey, std::move(fieldUPtr) }); //- copy
    field_inn::erase_from_fieldsBuilding( fieldKey );    
}


/* ===========================================================
 *      atom_erase_all_fields_in_chunk   [-WRITE-]
 * -----------------------------------------------------------
 * 遍历 chunk 中所有 field， 直接删除这些实例
 */
void atom_erase_all_fields_in_chunk( const IntVec2 &chunkMPos_ ){

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
 *           atom_field_reflesh_min_and_max_altis     [-WRITE-]
 * -----------------------------------------------------------
 */
void atom_field_reflesh_min_and_max_altis(fieldKey_t fieldKey_, const MapAltitude &alti_ ){
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( field_inn::fieldsSharedMutex ); //- write -
        tprAssert( field_inn::is_find_in_fields_(fieldKey_) ); //- MUST EXIST
        field_inn::fields.at(fieldKey_)->reflesh_min_and_max_altis( alti_ );
    }
}



/* ===========================================================
 *           atom_field_set_nodeAlti_2     [-WRITE-]
 * -----------------------------------------------------------
 * -- 仅被 Chunk::init() 使用
 */
void atom_field_set_nodeAlti_2( fieldKey_t fieldKey_, 
                                const std::vector<std::unique_ptr<MemMapEnt>> &_chunkMapEnts ){
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( field_inn::fieldsSharedMutex ); //- write -
        tprAssert( field_inn::is_find_in_fields_(fieldKey_) ); //- MUST EXIST
        field_inn::fields.at(fieldKey_)->set_nodeAlti_2( _chunkMapEnts );
    }
}


/* ===========================================================
 *       atom_get_mapFieldData_in_chunkCreate     [-READ-]
 * -----------------------------------------------------------
 * only used by job-thread: build_chunkData.cpp
 */
std::unique_ptr<MapFieldData_In_ChunkCreate> atom_get_mapFieldData_in_chunkCreate( fieldKey_t fieldKey_ ){

    auto uptr = std::make_unique<MapFieldData_In_ChunkCreate>();
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( field_inn::fieldsSharedMutex ); //- read -
            tprAssert( field_inn::is_find_in_fields_(fieldKey_) ); //- MUST EXIST
        const auto &field = *(field_inn::fields.at( fieldKey_ ).get());
        uptr->occupyWeight = field.get_occupyWeight();
        uptr->fieldKey = field.get_fieldKey();
        uptr->ecoObjKey = field.get_ecoObjKey();
        uptr->densityIdx = field.get_density().get_idx();
        uptr->fieldBorderSetId = field.get_fieldBorderSetId();
        uptr->nodeMPos = field.get_nodeMPos();
    }
    return uptr;
}



/* ===========================================================
 *           atom_create_gos_in_field       [-READ-]
 * -----------------------------------------------------------
 * -1- 根据 field 信息，确定将要生成的 go 类型
 * -2- 计算生成概率
 * -3- 正式执行生成
 */
void atom_create_gos_in_field( fieldKey_t fieldKey_ ){
    //--- atom ---//
    std::shared_lock<std::shared_mutex> sl( field_inn::fieldsSharedMutex ); //- read -
        tprAssert( field_inn::is_find_in_fields_(fieldKey_) ); //- MUST EXIST
    const auto &fieldRef = *(field_inn::fields.at( fieldKey_ ).get());

    sectionKey_t   ecoObjKey = fieldRef.get_ecoObjKey();

    double randV = fieldRef.get_uWeight() * 0.35 + 313.17; //- 确保大于0
    double fract = randV - floor(randV); //- 小数部分
    tprAssert( (fract>=0.0) && (fract<=1.0) );


    //----- mapsurface go ------//
    {               
        mapSurfaceRandEntId_t entId = esrc::get_chunkRef(anyMPos_2_chunkKey(fieldRef.get_mpos())).get_mapSurfaceRandEntId();
        MapSurfaceRandLvl mapSurfaceLvl = esrc::get_mapSurfaceRandLvl( entId, fieldRef.calc_fieldIdx_in_chunk() );

        if( mapSurfaceLvl != MapSurfaceRandLvl::Nil ){

            //--- dyParam ---//
            ParamBinary dyParam {};
            auto *mapSurfaceBp = reinterpret_cast<DyParams_MapSurface*>( dyParam.init_binary(ParamBinaryType::MapSurface) );
            mapSurfaceBp->spec = MapSurfaceLowSpec::WhiteRock; //- tmp，其实要根据 eco 来分配 ...
            mapSurfaceBp->lvl = mapSurfaceLvl;
            mapSurfaceBp->randVal = fieldRef.get_uWeight();

            //--- goSpecId ---//
            goSpecId_t goSpecId = ssrc::get_goSpecId( "mapSurfaceLower" );
            gameObjs::create_a_Go(  goSpecId,
                                    mpos_2_dpos( fieldRef.get_mpos() ), // 这会显得工整，但减少了重叠，未来可修改
                                    dyParam );
        }
    }


    //----- land go -----//
    if( fieldRef.is_land() ){
        const auto &goSpecData = esrc::atom_ecoObj_apply_a_rand_goSpecData( ecoObjKey,
                                                            fieldRef.get_density().get_idx(),
                                                            fieldRef.get_uWeight() );
        const auto &animLabels = goSpecData.get_animLabels();

        if( fract <= esrc::atom_ecoObj_get_applyPercent( ecoObjKey, fieldRef.get_density()) ){

            //--- dyParam ---//
            ParamBinary dyParam {};
            auto *fieldBp = reinterpret_cast<DyParams_Field*>( dyParam.init_binary(ParamBinaryType::Field) );
            fieldBp->fieldWeight = fieldRef.get_uWeight();
            fieldBp->fieldNodeMapEntAlti = fieldRef.get_nodeMapAlti(); //- tmp 有问题
            fieldBp->fieldDensity = fieldRef.get_density();
            fieldBp->animLabels.insert( fieldBp->animLabels.end(), animLabels.cbegin(), animLabels.cend() );//- maybe empty

            gameObjs::create_a_Go(  goSpecData.get_goSpecId(),
                                    fieldRef.get_nodeDPos(),
                                    dyParam );
        }
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
    return *(field_inn::fields.at( fieldKey_ ).get());
}




namespace field_inn {//------------ namespace: field_inn --------------//


/* ===========================================================
 *              fieldsBuilding funcs
 * -----------------------------------------------------------
 */
void insert_2_fieldsBuilding( fieldKey_t fieldKey_ ){
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( fieldsBuildingMutex );
            tprAssert( fieldsBuilding.find(fieldKey_) == fieldsBuilding.end() );
        fieldsBuilding.insert( fieldKey_ );
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

