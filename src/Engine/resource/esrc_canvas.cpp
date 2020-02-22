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
#include "esrc_state.h"

//#include "tprDebug.h" //- tmp

namespace esrc{//------------------ namespace: esrc -------------------------//


namespace canvas_inn {//-------- namespace: canvas_inn --------------//
    std::unique_ptr<Canvas> groundCanvasUPtr;
    //std::unique_ptr<Canvas> waterAnimCanvasUPtr;

}//------------- namespace: canvas_inn end --------------//


/* ===========================================================
 *                     init_canvases
 * -----------------------------------------------------------
 */
void init_canvases(){

    tprAssert( esrc::is_setState("shader") );

    canvas_inn::groundCanvasUPtr = std::make_unique<Canvas>();
    //canvas_inn::waterAnimCanvasUPtr = std::make_unique<Canvas>();

    //------------------//
    //    groundCanvas
    //------------------//
    canvas_inn::groundCanvasUPtr->init( &(ViewingBox::gameSZ),
                            esrc::get_shaderPtr(ShaderType::Ground) );

    //------------------//
    //    waterAnimCanvas
    //------------------//
    /*
    canvas_inn::waterAnimCanvasUPtr->init( &(ViewingBox::gameSZ),
                            esrc::get_shaderPtr(ShaderType::Water) );
    */

    esrc::insertState("canvas");
}

/* ===========================================================
 *                    draw_groundCanvas
 * -----------------------------------------------------------
 */
void draw_groundCanvas(){

    canvas_inn::groundCanvasUPtr->use_shaderProgram(); //- MUST !!! 

    glm::vec2 canvasFPos = get_camera().calc_canvasFPos();

    canvas_inn::groundCanvasUPtr->set_translate(canvasFPos.x,
                                                canvasFPos.y,
                                      static_cast<float>(get_camera().get_zFar() + ViewingBox::ground_zOff) );
        
    canvas_inn::groundCanvasUPtr->draw();
}


/* ===========================================================
 *                  draw_waterAnimCanvas
 * -----------------------------------------------------------
 */
/*
void draw_waterAnimCanvas(){

    canvas_inn::waterAnimCanvasUPtr->use_shaderProgram(); //- MUST !!! 

    glm::vec2 canvasFPos = get_camera().calc_canvasFPos();

    canvas_inn::waterAnimCanvasUPtr->set_translate( canvasFPos.x,
                                                    canvasFPos.y,
                                        static_cast<float>(get_camera().get_zFar() + ViewingBox::waterAnim_zOff) );
                                        //- 这一步是正确的，canvas 与 window 成功对齐，
                                       
    canvas_inn::waterAnimCanvasUPtr->draw();
}
*/

}//---------------------- namespace: esrc -------------------------//

