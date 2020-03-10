/*
 * ========================= Canvas.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.03.31
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Canvas.h"

//-------------------- Engine --------------------//
#include "create_texNames.h"
#include "esrc_camera.h"


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Canvas::init(  IntVec2 *texSizePtr_, ShaderProgram *shaderPtr_ ){

    this->texSizePtr = texSizePtr_;
    this->shaderPtr = shaderPtr_;

    //-------- mesh ---------
    this->mesh.init( create_a_empty_texName( *this->texSizePtr ) );
    this->mesh.isVisible = true;
    this->mesh.set_shader_program( this->shaderPtr );
    this->mesh.set_scale(glm::vec3{ static_cast<float>(this->texSizePtr->x), 
                                    static_cast<float>(this->texSizePtr->y), 
                                    1.0f });
    this->is_binded = true;
}


/* ===========================================================
 *                       draw
 * -----------------------------------------------------------
 * 正确的 draw call 顺序：
 *   canvas.use_shaderProgram();
 *   "set uniforms" -- get_uniform_location()
 *   canvas.set_translate(...) //- MUST !!!
 *   canvas.draw();
 */
void Canvas::draw(){
    tprAssert( this->is_binded );

    //-- 每一帧都要 重设置，从而能让 canvas 跟随 被拉伸过的 window size --
    this->mesh.set_scale(glm::vec3{ static_cast<float>(this->texSizePtr->x), 
                                    static_cast<float>(this->texSizePtr->y), 
                                    1.0f });

    this->shaderPtr->use_program();
    this->mesh.draw();
    this->is_binded = false;
}



