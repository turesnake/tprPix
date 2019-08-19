/*
 * ===================== esrc_renderPool.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "esrc_renderPool.h"

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace renderP_inn {//-------- namespace: renderP_inn  --------------//

    //--- goMesh ---//
    std::multimap<float, ChildMesh*> renderPool_goMeshs_opaque {};      // 不透明队列
    std::multimap<float, ChildMesh*> renderPool_goMeshs_translucent {}; // 半透明队列
            //- key 是 图元的 z值。map会自动排序(负无穷在前，正无穷在后，符合我们要的顺序)
            //- 有的 go.pos.z 值可能相同，所以要使用 multimap !

    //--- ui ---//
    std::multimap<float, UIChildMesh*> renderPool_uiMeshs_pic {}; 
            //- 暂时只需要渲染 ui.gomesh.pic
            //  在未来，ui 图元 也可能会准备 shadow 

    //-- 一切以 Mesh为标准的 图元，都可以丢进这个 容器中
    //-- 比如：
    //    - Chunk （废弃）
    //    - go脚下的阴影
    //    - UI图元等
    std::multimap<float, Mesh*> renderPool_meshs {};

}//------------- namespace: renderP_inn end --------------//


void init_renderPools(){
    // nothing...
}

void insert_2_renderPool_goMeshs_opaque( float off_z_, ChildMesh *meshPtr_ ){
    renderP_inn::renderPool_goMeshs_opaque.insert({ off_z_, meshPtr_ }); //- copy
}
void insert_2_renderPool_goMeshs_translucent( float off_z_, ChildMesh *meshPtr_ ){
    renderP_inn::renderPool_goMeshs_translucent.insert({ off_z_, meshPtr_ }); //- copy
}

void insert_2_renderPool_meshs( float off_z_, Mesh *meshPtr_ ){
    renderP_inn::renderPool_meshs.insert({ off_z_, meshPtr_ }); //- copy
}

void insert_2_renderPool_uiMeshs_pic( float off_z_, UIChildMesh *meshPtr_ ){
    renderP_inn::renderPool_uiMeshs_pic.insert({ off_z_, meshPtr_ }); //- copy
}

void clear_renderPool_goMeshs_opaque(){
    renderP_inn::renderPool_goMeshs_opaque.clear();
}
void clear_renderPool_goMeshs_translucent(){
    renderP_inn::renderPool_goMeshs_translucent.clear();
}
void clear_renderPool_uiMeshs_pic(){
    renderP_inn::renderPool_uiMeshs_pic.clear();
}
void clear_renderPool_meshs(){
    renderP_inn::renderPool_meshs.clear();
}



/* ===========================================================
 *              draw_renderPool_goMeshs_opaque
 * -----------------------------------------------------------
 */
void draw_renderPool_goMeshs_opaque(){
    //-- 渲染次序，pos.z值大的 先渲染 --
    for( auto it=renderP_inn::renderPool_goMeshs_opaque.rbegin(); 
            it != renderP_inn::renderPool_goMeshs_opaque.rend(); it++  ){
        it->second->draw();
    }
}
/* ===========================================================
 *           draw_renderPool_goMeshs_translucent
 * -----------------------------------------------------------
 */
void draw_renderPool_goMeshs_translucent(){
    for( auto &pairRef : renderP_inn::renderPool_goMeshs_translucent ){
        //-- 渲染次序，pos.z值小的 先渲染 --
        pairRef.second->draw();
    }
}


/* ===========================================================
 *                draw_renderPool_uiMeshs_pic
 * -----------------------------------------------------------
 */
void draw_renderPool_uiMeshs_pic(){

    for( auto &pairRef : renderP_inn::renderPool_uiMeshs_pic ){
        //-- 渲染持续，pos.z值小的 先渲染 --
        pairRef.second->draw();
    }
}

/* ===========================================================
 *                draw_renderPool_meshs
 * -----------------------------------------------------------
 */
void draw_renderPool_meshs(){

    for( auto &pairRef : renderP_inn::renderPool_meshs ){
        pairRef.second->draw();
    }
}


}//---------------------- namespace: esrc -------------------------//

