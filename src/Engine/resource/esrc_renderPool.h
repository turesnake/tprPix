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
#include "Mesh.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void insert_2_renderPool_goMeshs_opaque( float off_z_, ChildMesh *meshPtr_ );
void insert_2_renderPool_goMeshs_translucent( float off_z_, ChildMesh *meshPtr_ );

void clear_renderPool_goMeshs_opaque();
void clear_renderPool_goMeshs_translucent();
       
void draw_renderPool_goMeshs_opaque();
void draw_renderPool_goMeshs_translucent();

}//---------------------- namespace: esrc -------------------------//
#endif

