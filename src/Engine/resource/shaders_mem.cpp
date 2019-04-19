/*
 * ========================= shaders_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  shaders  在 内存中的 管理
 * ----------------------------
 */
//-------------------- Engine --------------------//
#include "esrc_shader.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//

/* ===========================================================
 *                    init_shaders     [pure]
 * -----------------------------------------------------------
 * -- init 游戏中 所有 shader实例
 */
void init_shaders(){

    //---- rect_sahder ----//
    rect_shader.init( "/base.vs", "/base.fs" ); 
    rect_shader.use_program();
    rect_shader.add_new_uniform( "model" );
    rect_shader.add_new_uniform( "view" );
    rect_shader.add_new_uniform( "projection" );
    rect_shader.add_new_uniform( "texture1" );
    rect_shader.use_program();
}



}//---------------------- namespace: esrc -------------------------//
