/*
 * ========================= Canvas.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.03.31
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "Canvas.h"

//-------------------- Engine --------------------//
#include "create_texNames.h"
#include "windowConfig.h"
#include "srcs_engine.h"


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Canvas::init(  const IntVec2 &_texSize,
                    const std::string &_lpath_vs,
                    const std::string &_lpath_fs ){

    this->texSize = _texSize;

    //-------- shaderProgram ---------
    this->ShaderProgram.init( _lpath_vs, _lpath_fs );
    this->ShaderProgram.use_program();
    this->ShaderProgram.add_new_uniform( "model" );
    this->ShaderProgram.add_new_uniform( "view" );
    this->ShaderProgram.add_new_uniform( "projection" );
    this->ShaderProgram.add_new_uniform( "texture1" );
        // 其它的 uniform 需要在此函数之后，由用户程序 添加

    //-------- mesh ---------
    this->mesh.init( create_a_empty_texName(texSize) );
    this->mesh.isVisible = true;
    this->mesh.set_shader_program( &this->ShaderProgram );
    this->mesh.set_scale(glm::vec3{ (float)(this->texSize.x), 
                                    (float)(this->texSize.y), 
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
    assert( this->is_binded );

    this->ShaderProgram.send_mat4_view_2_shader( esrc::camera.update_mat4_view() );
    this->ShaderProgram.send_mat4_projection_2_shader( esrc::camera.update_mat4_projection() );


    this->mesh.draw();
    this->is_binded = false;
}



