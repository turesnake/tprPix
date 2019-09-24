/*
 * =================== esrc_uniformBlockObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_UNIFORM_BLOCK_OBJ_H
#define TPR_ESRC_UNIFORM_BLOCK_OBJ_H

//-------------------- Engine --------------------//
#include "UniformBlockObj.h"
#include "uniformBlockObjs.h"


namespace esrc {//------------------ namespace: esrc -------------------------//  

void init_uniformBlockObjs()noexcept; //-- must before esrc::init_shaders() !!!

ubo::UniformBlockObj &get_uniformBlockObjRef( ubo::UBOType type_ )noexcept;

}//---------------------- namespace: esrc -------------------------//
#endif

