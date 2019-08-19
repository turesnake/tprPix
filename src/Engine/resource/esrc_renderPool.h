/*
 * ========================= esrc_renderPool.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_RENDER_POOL_H
#define TPR_ESRC_RENDER_POOL_H

//-------------------- CPP --------------------//
#include <vector>
#include <map>
#include <unordered_set>

//-------------------- Engine --------------------//
#include "ChildMesh.h"
#include "UIChildMesh.h"
#include "Mesh.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_renderPools();


void insert_2_renderPool_goMeshs_opaque( float off_z_, ChildMesh *meshPtr_ );
void insert_2_renderPool_goMeshs_translucent( float off_z_, ChildMesh *meshPtr_ );

void insert_2_renderPool_uiMeshs_pic( float off_z_, UIChildMesh *meshPtr_ );

void clear_renderPool_goMeshs_opaque();
void clear_renderPool_goMeshs_translucent();
void clear_renderPool_mapSurfaces();
void clear_renderPool_uiMeshs_pic();

            
void draw_renderPool_goMeshs_opaque();
void draw_renderPool_goMeshs_translucent();

void draw_renderPool_mapSurfaces();
void draw_renderPool_uiMeshs_pic();


void insert_2_renderPool_meshs( float off_z_, Mesh *meshPtr_ );
void clear_renderPool_meshs();
void draw_renderPool_meshs();



}//---------------------- namespace: esrc -------------------------//
#endif

