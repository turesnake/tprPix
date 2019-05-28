/*
 * ========================= esrc_renderPool.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_RENDER_POOL_H_
#define _TPR_ESRC_RENDER_POOL_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <map>

//-------------------- Engine --------------------//
#include "ChildMesh.h"
#include "UIChildMesh.h"
#include "Mesh.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

//--- goMesh ---//
inline std::multimap<float, ChildMesh*> renderPool_goMeshs_pic {}; 
            //- key 是 图元的 z值。map会自动排序(负无穷在前，正无穷在后，符合我们要的顺序)
            //- 遍历 渲染池，就能从远到近地 渲染每一个 图元
            //- 有的 go.pos.z 值可能相同，所以要使用 multimap !
inline std::vector<ChildMesh*> renderPool_goMeshs_shadow {};
            //- 所有 shadow 拥有相同的 z-deep. 对渲染次序 无要求

//-----
inline std::vector<ChildMesh*> renderPool_mapSurfaces {};
            //- 地表go，没有 shadow，只需要渲染 pic


inline std::multimap<float, UIChildMesh*> renderPool_uiMeshs_pic {}; 
            //- 暂时只需要渲染 ui.gomesh.pic
            //  在未来，ui 图元 也可能会准备 shadow

            
void draw_renderPool_goMeshs_pic();
void draw_renderPool_goMeshs_shadow();
void draw_renderPool_mapSurfaces();
void draw_renderPool_uiMeshs_pic();


//-- 一切以 Mesh为标准的 图元，都可以丢进这个 容器中
//-- 比如：
//    - Chunk
//    - go脚下的阴影
//    - UI图元等
inline std::multimap<float, Mesh*> renderPool_meshs {};

void draw_renderPool_meshs();



}//---------------------- namespace: esrc -------------------------//
#endif

