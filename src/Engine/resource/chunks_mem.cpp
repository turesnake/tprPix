/*
 * ========================= chunks_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "esrc_chunk.h"
#include "esrc_renderPool.h"
#include "config.h"
#include "chunkBuild.h"


#include "tprDebug.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace{//------------ namespace --------------//

    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, Chunk> chunks {};


}//---------------- namespace end --------------//



/* ===========================================================
 *             insert_and_init_new_chunk
 * -----------------------------------------------------------
 * 创建 chunk实例，放入 全局容器，且初始化它
 * ---
 * 仅被 check_and_build_sections.cpp -> build_one_chunk() 调用
 */
Chunk *insert_and_init_new_chunk(const IntVec2 &_anyMPos,
                                ShaderProgram *_sp ){

            //-- ShaderProgram 是固定的，可以在游戏初期就绑定。
            //   不需要每次 创建chunk 时都指定。

    // ***| INSERT FIRST, INIT LATER  |***
    Chunk *chunkPtr;
    Chunk  chunk {};
    chunk.set_by_anyMPos( _anyMPos );
    chunkKey_t key = chunk.get_key();
        assert( esrc::chunks.find(key) == esrc::chunks.end() );//- must not exist
    esrc::chunks.insert({ key, chunk }); //- copy
    chunkPtr = &(esrc::chunks.at(key));
    chunkPtr->set_mesh_shader_program( _sp );
    chunkPtr->init();
    return chunkPtr;
}


/* ===========================================================
 *                  get_memMapEntPtr
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 chunk 不存在，就要：加载它／创建它
 * -----------------
 * 这组函数存在缺陷：
 *   如果 mapent 所在的 chunk 并不存在，将直接出错。
 *   尤其是 collision 模块中
 *   ----
 *   目前的做法是，当发现目标 chunk 不存在时，调用一个 特殊的函数，阻塞主线程，直到目标chunk制作好
 *   这个方法是防止程序崩溃的最后办法，临时的
 *   未来希望更好的办法...
 */
MemMapEnt *get_memMapEntPtr( const MapCoord &_anyMCpos ){

    //-- 计算 目标 chunk 的 key --
    const IntVec2 &mposRef = _anyMCpos.get_mpos();
    chunkKey_t     chunkKey = anyMPos_2_chunkKey( mposRef );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( mposRef );


                //-- 若 目标chunk实例不存在，调用特殊函数来 处理 --
                if( esrc::chunks.find(chunkKey) == esrc::chunks.end() ){
                        cout << "get_memMapEntPtr(): wait_and_build_chunk..." 
                            << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                }
                //-- 再次检测
                assert( esrc::chunks.find(chunkKey) != esrc::chunks.end() ); //- tmp


    return esrc::chunks.at(chunkKey).getnc_mapEntPtr_by_lMPosOff( lMPosOff );
}

MemMapEnt *get_memMapEntPtr( const IntVec2 &_anyMPos ){

    //-- 计算 目标 chunk 的 key --
    chunkKey_t    chunkKey = anyMPos_2_chunkKey( _anyMPos );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( _anyMPos );


                //-- 若 目标chunk实例不存在，调用特殊函数来 处理 --
                if( esrc::chunks.find(chunkKey) == esrc::chunks.end() ){
                        cout << "get_memMapEntPtr(): wait_and_build_chunk..." 
                            << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                }
                //-- 再次检测
                assert( esrc::chunks.find(chunkKey) != esrc::chunks.end() ); //- tmp


    return esrc::chunks.at(chunkKey).getnc_mapEntPtr_by_lMPosOff( lMPosOff );
}

/* ===========================================================
 *                find_from_chunks
 * -----------------------------------------------------------
 */
bool find_from_chunks( chunkKey_t _chunkKey ){
    return (esrc::chunks.find(_chunkKey) != esrc::chunks.end());
}

/* ===========================================================
 *                 get_chunkPtr
 * -----------------------------------------------------------
 */
Chunk *get_chunkPtr( chunkKey_t _key ){
        assert( esrc::chunks.find(_key) != esrc::chunks.end() );//- must exist
    return &(esrc::chunks.at(_key));
}


/* ===========================================================
 *                 render_chunks
 * -----------------------------------------------------------
 */
void render_chunks(){
    for( auto& p : esrc::chunks ){
            p.second.refresh_translate_auto(); //-- MUST !!!
            esrc::renderPool_meshs.insert({ p.second.get_mesh().get_render_z(),
                                                const_cast<Mesh*>(&p.second.get_mesh()) });
    }
}


}//---------------------- namespace: esrc -------------------------//

