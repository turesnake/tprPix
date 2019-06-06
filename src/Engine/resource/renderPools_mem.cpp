/*
 * ========================= renderPools_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "esrc_renderPool.h"

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace {//-------- namespace: --------------//

    //--- goMesh ---//
    std::multimap<float, ChildMesh*> renderPool_goMeshs_pic {}; 
            //- key 是 图元的 z值。map会自动排序(负无穷在前，正无穷在后，符合我们要的顺序)
            //- 遍历 渲染池，就能从远到近地 渲染每一个 图元
            //- 有的 go.pos.z 值可能相同，所以要使用 multimap !
    std::vector<ChildMesh*> renderPool_goMeshs_shadow {};
            //- 所有 shadow 拥有相同的 z-deep. 对渲染次序 无要求

    //-----
    std::vector<ChildMesh*> renderPool_mapSurfaces {};
            //- 地表go，没有 shadow，只需要渲染 pic


    std::multimap<float, UIChildMesh*> renderPool_uiMeshs_pic {}; 
            //- 暂时只需要渲染 ui.gomesh.pic
            //  在未来，ui 图元 也可能会准备 shadow 

    //-- 一切以 Mesh为标准的 图元，都可以丢进这个 容器中
    //-- 比如：
    //    - Chunk
    //    - go脚下的阴影
    //    - UI图元等
    std::multimap<float, Mesh*> renderPool_meshs {};

}//------------- namespace: end --------------//



void insert_2_renderPool_goMeshs_pic( float _off_z, ChildMesh *_meshPtr ){
    renderPool_goMeshs_pic.insert({ _off_z, _meshPtr }); //- copy
}

void pushback_2_renderPool_mapSurfaces( ChildMesh *_meshPtr ){
    renderPool_mapSurfaces.push_back( _meshPtr ); //- copy
}

void pushback_2_renderPool_goMeshs_shadow( ChildMesh *_meshPtr ){
    renderPool_goMeshs_shadow.push_back( _meshPtr ); //- copy
}

void insert_2_renderPool_meshs( float _off_z, Mesh *_meshPtr ){
    renderPool_meshs.insert({ _off_z, _meshPtr }); //- copy
}

void insert_2_renderPool_uiMeshs_pic( float _off_z, UIChildMesh *_meshPtr ){
    renderPool_uiMeshs_pic.insert({ _off_z, _meshPtr }); //- copy
}

void clear_renderPool_goMeshs_pic(){
    renderPool_goMeshs_pic.clear();
}
void clear_renderPool_goMeshs_shadow(){
    renderPool_goMeshs_shadow.clear();
}
void clear_renderPool_mapSurfaces(){
    renderPool_mapSurfaces.clear();
}
void clear_renderPool_uiMeshs_pic(){
    renderPool_uiMeshs_pic.clear();
}
void clear_renderPool_meshs(){
    renderPool_meshs.clear();
}



/* ===========================================================
 *                draw_renderPool_goMeshs_pic
 * -----------------------------------------------------------
 */
void draw_renderPool_goMeshs_pic(){

    for( auto &pairRef : esrc::renderPool_goMeshs_pic ){
        //-- 渲染持续，pos.z值小的 先渲染 --
        pairRef.second->draw();
    }
}

/* ===========================================================
 *                draw_renderPool_uiMeshs_pic
 * -----------------------------------------------------------
 */
void draw_renderPool_uiMeshs_pic(){

    for( auto &pairRef : esrc::renderPool_uiMeshs_pic ){
        //-- 渲染持续，pos.z值小的 先渲染 --
        pairRef.second->draw();
    }
}


/* ===========================================================
 *                draw_renderPool_goMeshs_shaodw
 * -----------------------------------------------------------
 */
void draw_renderPool_goMeshs_shadow(){
    
    for( auto &ptr : esrc::renderPool_goMeshs_shadow ){
        ptr->draw();
    }
}

/* ===========================================================
 *               draw_renderPool_mapSurfaces
 * -----------------------------------------------------------
 */
void draw_renderPool_mapSurfaces(){

    for( auto &ptr : esrc::renderPool_mapSurfaces ){
        ptr->draw();
    }
}


/* ===========================================================
 *                draw_renderPool_meshs
 * -----------------------------------------------------------
 */
void draw_renderPool_meshs(){

    for( auto &pairRef : esrc::renderPool_meshs ){
        pairRef.second->draw();
    }
}



}//---------------------- namespace: esrc -------------------------//

