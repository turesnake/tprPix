/*
 * ========================= shaders_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.26
 *                                        修改 -- 2018.12.26
 * ----------------------------------------------------------
 *  shaders  在 内存中的 管理
 * ----------------------------
 */
#include "Engine/resource/srcs_manager.h" //- 所有资源

namespace src{ //------------------ namespace: src -------------------------//

/* ===========================================================
 *                    shaders_init
 * -----------------------------------------------------------
 * -- init 游戏中 所有 shader实例
 */
void shaders_init(){

    //---- rect_sahder ----//
    src::rect_shader.init(); 
    src::rect_shader.get_uniform_location( "model" );
    src::rect_shader.get_uniform_location( "view" );
    src::rect_shader.get_uniform_location( "projection" );
    src::rect_shader.get_uniform_location( "texture1" );
    src::rect_shader.use_program();
}



}//---------------------- namespace: src -------------------------//
