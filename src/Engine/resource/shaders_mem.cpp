/*
 * ========================= shaders_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.26
 *                                        修改 -- 2018.12.26
 * ----------------------------------------------------------
 *  shaders  在 内存中的 管理
 * ----------------------------
 */
//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源

namespace esrc{ //------------------ namespace: esrc -------------------------//

/* ===========================================================
 *                    shaders_init
 * -----------------------------------------------------------
 * -- init 游戏中 所有 shader实例
 */
void shaders_init(){

    //---- rect_sahder ----//
    rect_shader.init(); 
    rect_shader.get_uniform_location( "model" );
    rect_shader.get_uniform_location( "view" );
    rect_shader.get_uniform_location( "projection" );
    rect_shader.get_uniform_location( "texture1" );
    rect_shader.use_program();
}



}//---------------------- namespace: esrc -------------------------//
