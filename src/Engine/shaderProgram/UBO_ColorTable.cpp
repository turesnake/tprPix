/*
 * ================== UBO_ColorTable.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ubo_all.h"

//-------------------- Engine --------------------//
#include "GameObj.h"

#include "esrc_colorTableSet.h"
#include "esrc_uniformBlockObj.h"
#include "esrc_player.h"
#include "esrc_field.h"
#include "esrc_chunk.h"

#include "esrc_time.h"

namespace ubo{//------------- namespace ubo ----------------

namespace ubo_colorTable_inn{//------------- namespace ubo ----------------
    colorTableId_t currentColorTableId {99999};


}//------------- namespace ubo: end ----------------



// called only once, at app.init 
void write_ubo_OriginColorTable(){

    auto &colorTableSetRef = esrc::get_colorTabelSet();
    auto id = colorTableSetRef.get_colorTableId("origin");
    auto &originColorTableRef = colorTableSetRef.get_colorTableRef(id);
    //---
    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::OriginColorTable );
    ubo.write(0, 
            static_cast<GLsizeiptr>(originColorTableRef.get_dataSize()),
            originColorTableRef.get_dataPtr<const GLvoid*>() );
}


// called only once, at app.init 
void write_ubo_GroundColorTable(){

    auto &colorTableSetRef = esrc::get_colorTabelSet();
    //---
    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::GroundColorTable );
    ubo.write( 0,
            static_cast<GLsizeiptr>(colorTableSetRef.get_groundColor_dataSize()),
            colorTableSetRef.get_groundColor_dataPtr<const GLvoid*>() );
            // 实际上，在传入的数据中，[0]号元素是空的，[1]是origin，不会被访问到，
}


void write_ubo_colorTableId( colorTableId_t id_ ){

    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::ColorTableId );
    ubo.write(0, 
            static_cast<GLsizeiptr>(sizeof(colorTableId_t)),
            static_cast<const GLvoid*>(&id_) );
}


//-- called each render frame --
void update_and_write_ubo_UnifiedColorTable(){

    //--------------------------//
    //  check if rebind targetColorTable
    //--------------------------//
    auto &goRef = esrc::get_player().get_goRef();

    auto mapEntPair = esrc::getnc_memMapEntPtr( dpos_2_mpos(goRef.get_dpos()) );
    tprAssert( mapEntPair.first == ChunkMemState::Active );
    colorTableId_t id = mapEntPair.second->get_colorTableId();

    // 最简方案，仅在 playergo 跨越 ecoObj 时，才绑定新 colorTable --
    if( id != ubo_colorTable_inn::currentColorTableId ){
        ubo_colorTable_inn::currentColorTableId = id;
        esrc::rebind_currentColorTabel_target( id );
    }

        // 改进，不会立即切换到新的 id
        // 而是设置一个延时期，当读取到新的 id时，启动延时器，比如1～2秒
        //  若发现 playergo 再次将 id 改回旧id，说明此时，playergo 在两个 eco 间来回横跳
        //  此时将立即重置延时器，
        //  只有那些读完延时器 的新id，才会被真的执行 color切换
        //  ...

    //--------------------------//
    //         update
    //--------------------------//
    auto &currentColorTableRef = esrc::get_currentColorTabel();
    currentColorTableRef.update();

    if( !currentColorTableRef.get_isWorking() ){
        return;
    }
    
    //--------------------------//
    //        write
    //--------------------------//
    auto &colorTableRef = currentColorTableRef.get_colorTableRef();

    auto &ubo = esrc::get_uniformBlockObjRef( ubo::UBOType::UnifiedColorTable );
    ubo.write(  0, 
                static_cast<GLsizeiptr>(colorTableRef.get_dataSize()),
                colorTableRef.get_dataPtr<const GLvoid*>() );
                //-- 很多时候，也不需要每帧都 彻底重写数据 --
                //   而是可以只改写 某一段

}


}//------------- namespace ubo: end ----------------

