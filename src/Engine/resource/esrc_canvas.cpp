/*
 * ================= esrc_canvas.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- CPP --------------------//
#include <memory>
//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "esrc_canvas.h"
#include "esrc_window.h"
#include "esrc_camera.h"
#include "esrc_gameSeed.h"

#include "esrc_shader.h"

//#include "tprDebug.h" //- tmp

namespace esrc{//------------------ namespace: esrc -------------------------//


namespace canvas_inn {//-------- namespace: canvas_inn --------------//
    std::unique_ptr<Canvas> groundCanvasUPtr;
    std::unique_ptr<Canvas> waterAnimCanvasUPtr;

}//------------- namespace: canvas_inn end --------------//


/* ===========================================================
 *                     init_canvases
 * -----------------------------------------------------------
 */
void init_canvases(){

    canvas_inn::groundCanvasUPtr = std::make_unique<Canvas>();
    canvas_inn::waterAnimCanvasUPtr = std::make_unique<Canvas>();

    //------------------//
    //    groundCanvas
    //------------------//
    canvas_inn::groundCanvasUPtr->init( &(ViewingBox::gameSZ),
                            esrc::get_shaderPtr(ShaderType::Ground) );

    //------------------//
    //    waterAnimCanvas
    //------------------//
    canvas_inn::waterAnimCanvasUPtr->init( &(ViewingBox::gameSZ),
                            esrc::get_shaderPtr(ShaderType::Water) );
}

/* ===========================================================
 *                    draw_groundCanvas
 * -----------------------------------------------------------
 */
void draw_groundCanvas(){

    canvas_inn::groundCanvasUPtr->use_shaderProgram(); //- MUST !!! 

    glUniform1f(canvas_inn::groundCanvasUPtr->get_uniform_location("u_time"), 
                static_cast<float>(glfwGetTime()) ); //- 1-float

    float texSizeW = static_cast<float>(ViewingBox::gameSZ.x);
    float texSizeH = static_cast<float>(ViewingBox::gameSZ.y);
    

    glm::vec2 canvasFPos = get_camera().calc_canvasFPos();

    canvas_inn::groundCanvasUPtr->set_translate( canvasFPos.x,
                                      canvasFPos.y,
                                      static_cast<float>(get_camera().get_zFar() + ViewingBox::ground_zOff) );
        
    glUniform1f(canvas_inn::groundCanvasUPtr->get_uniform_location("texSizeW"), texSizeW ); //- 1-float
    glUniform1f(canvas_inn::groundCanvasUPtr->get_uniform_location("texSizeH"), texSizeH ); //- 1-float    

    canvas_inn::groundCanvasUPtr->draw();
}


/* ===========================================================
 *                  draw_waterAnimCanvas
 * -----------------------------------------------------------
 */
void draw_waterAnimCanvas(){

    canvas_inn::waterAnimCanvasUPtr->use_shaderProgram(); //- MUST !!! 

    glUniform1f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("u_time"), 
                    static_cast<float>(glfwGetTime()) ); //- 1-float

    float texSizeW = static_cast<float>(ViewingBox::gameSZ.x);
    float texSizeH = static_cast<float>(ViewingBox::gameSZ.y);

    glm::vec2 canvasFPos = get_camera().calc_canvasFPos();

    canvas_inn::waterAnimCanvasUPtr->set_translate(canvasFPos.x,
                                        canvasFPos.y,
                                        static_cast<float>(get_camera().get_zFar() + ViewingBox::waterAnim_zOff) );
                                        //- 这一步是正确的，canvas 与 window 成功对齐，
                                       
    glUniform1f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("texSizeW"), texSizeW ); //- 1-float
    glUniform1f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("texSizeH"), texSizeH ); //- 1-float

                                //-- 不用每次都传递，只要在 窗口尺寸 确实发生变化时，才传递
                                //   ......


    canvas_inn::waterAnimCanvasUPtr->draw();
}

}//---------------------- namespace: esrc -------------------------//

