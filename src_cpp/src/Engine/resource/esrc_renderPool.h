/*
 * ========================= esrc_renderPool.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_RENDER_POOL_H_
#define TPR_ESRC_RENDER_POOL_H_

//-------------------- CPP --------------------//
#include <vector>
#include <map>

//-------------------- Engine --------------------//
#include "ChildMesh.h"
#include "UIChildMesh.h"
#include "Mesh.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void insert_2_renderPool_goMeshs_pic( float _off_z, ChildMesh *_meshPtr );
void pushback_2_renderPool_mapSurfaces( ChildMesh *_meshPtr );
void pushback_2_renderPool_goMeshs_shadow( ChildMesh *_meshPtr );
void insert_2_renderPool_uiMeshs_pic( float _off_z, UIChildMesh *_meshPtr );

void clear_renderPool_goMeshs_pic();
void clear_renderPool_goMeshs_shadow();
void clear_renderPool_mapSurfaces();
void clear_renderPool_uiMeshs_pic();

            
void draw_renderPool_goMeshs_pic();
void draw_renderPool_goMeshs_shadow();
void draw_renderPool_mapSurfaces();
void draw_renderPool_uiMeshs_pic();


void insert_2_renderPool_meshs( float _off_z, Mesh *_meshPtr );
void clear_renderPool_meshs();
void draw_renderPool_meshs();



}//---------------------- namespace: esrc -------------------------//
#endif

