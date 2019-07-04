/*
 * ========================= esrc_chunk.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- CPP --------------------//
#include <unordered_map>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_chunk.h"
#include "esrc_renderPool.h"
#include "config.h"
#include "chunkBuild.h"


#include "tprDebug.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace chunk_inn {//------------ namespace: chunk_inn --------------//

    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, std::unique_ptr<Chunk>> chunkUPtrs {};


}//---------------- namespace: chunk_inn end --------------//



/* ===========================================================
 *             insert_and_init_new_chunk
 * -----------------------------------------------------------
 * 创建 chunk实例，放入 全局容器，且初始化它
 * ---
 * 仅被 check_and_build_sections.cpp -> build_one_chunk() 调用
 */
Chunk &insert_and_init_new_chunk(const IntVec2 &anyMPos_,
                                ShaderProgram *sp_ ){

            //-- ShaderProgram 是固定的，可以在游戏初期就绑定。
            //   不需要每次 创建chunk 时都指定。

    auto chunkUPtr = std::make_unique<Chunk>();
    chunkUPtr->set_by_anyMPos( anyMPos_ );
    chunkUPtr->set_mesh_shader_program( sp_ );
    chunkUPtr->init();

    chunkKey_t key = chunkUPtr->get_key();
        tprAssert( chunk_inn::chunkUPtrs.find(key) == chunk_inn::chunkUPtrs.end() );//- must not exist
    chunk_inn::chunkUPtrs.insert({ key, std::move(chunkUPtr) });

    return *(chunk_inn::chunkUPtrs.at(key).get());
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
MemMapEnt *get_memMapEntPtr( const MapCoord &anyMCpos_ ){

    //-- 计算 目标 chunk 的 key --
    const IntVec2 &mposRef = anyMCpos_.get_mpos();
    chunkKey_t     chunkKey = anyMPos_2_chunkKey( mposRef );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( mposRef );

                //-- 若 目标chunk实例不存在，调用特殊函数来 处理 --
                if( chunk_inn::chunkUPtrs.find(chunkKey) == chunk_inn::chunkUPtrs.end() ){
                        cout << "get_memMapEntPtr(): wait_and_build_chunk..." 
                            << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                }
                //-- 再次检测
                tprAssert( chunk_inn::chunkUPtrs.find(chunkKey) != chunk_inn::chunkUPtrs.end() ); //- tmp


    return chunk_inn::chunkUPtrs.at(chunkKey)->getnc_mapEntPtr_by_lMPosOff( lMPosOff );
}

MemMapEnt *get_memMapEntPtr( const IntVec2 &anyMPos_ ){

    //-- 计算 目标 chunk 的 key --
    chunkKey_t    chunkKey = anyMPos_2_chunkKey( anyMPos_ );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( anyMPos_ );

                //-- 若 目标chunk实例不存在，调用特殊函数来 处理 --
                if( chunk_inn::chunkUPtrs.find(chunkKey) == chunk_inn::chunkUPtrs.end() ){
                        cout << "get_memMapEntPtr(): wait_and_build_chunk..." 
                            << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                }
                //-- 再次检测
                tprAssert( chunk_inn::chunkUPtrs.find(chunkKey) != chunk_inn::chunkUPtrs.end() ); //- tmp


    return chunk_inn::chunkUPtrs.at(chunkKey)->getnc_mapEntPtr_by_lMPosOff( lMPosOff );
}

/* ===========================================================
 *                find_from_chunks
 * -----------------------------------------------------------
 */
bool find_from_chunks( chunkKey_t chunkKey_ ){
    return (chunk_inn::chunkUPtrs.find(chunkKey_) != chunk_inn::chunkUPtrs.end());
}

/* ===========================================================
 *                 get_chunkRef
 * -----------------------------------------------------------
 */
Chunk &get_chunkRef( chunkKey_t key_ ){
        tprAssert( chunk_inn::chunkUPtrs.find(key_) != chunk_inn::chunkUPtrs.end() );//- must exist
    return *(chunk_inn::chunkUPtrs.at(key_).get());
}



/* ===========================================================
 *                 render_chunks
 * -----------------------------------------------------------
 */
void render_chunks(){
    for( auto& p : chunk_inn::chunkUPtrs ){
            p.second->refresh_translate_auto(); //-- MUST !!!
            insert_2_renderPool_meshs( p.second->get_mesh().get_render_z(),
                                                const_cast<Mesh*>(&p.second->get_mesh()) );
    }
}


}//---------------------- namespace: esrc -------------------------//

