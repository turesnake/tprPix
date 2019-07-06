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

//#include "tprDebug.h" //- tmp

namespace esrc{//------------------ namespace: esrc -------------------------//


namespace canvas_inn {//-------- namespace: canvas_inn --------------//

    std::unique_ptr<Canvas> groundCanvasUPtr;
    std::unique_ptr<Canvas> waterAnimCanvasUPtr;

    bool  is_waterAnim_baseUniforms_transmited {false}; //- pixGpgpu 的几个 静态uniform值 是否被传输
                                        // 这些值是固定的，每次游戏只需传入一次...

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

    canvas_inn::groundCanvasUPtr->init( &(ViewingBox::windowSZ),
                            "/groundCanvas.vs",
                            "/groundCanvas.fs" );

    canvas_inn::groundCanvasUPtr->add_new_uniform( "u_time" ); //- 1-float
    canvas_inn::groundCanvasUPtr->add_new_uniform( "canvasCFPos" ); //- 2-float
    

    canvas_inn::groundCanvasUPtr->add_new_uniform( "SCR_WIDTH" ); //- 1-float
    canvas_inn::groundCanvasUPtr->add_new_uniform( "SCR_HEIGHT" ); //- 1-float
                        //-- 当 窗口发生变化，此组值需要被重传

    //------------------//
    //    waterAnimCanvas
    //------------------//

    canvas_inn::waterAnimCanvasUPtr->init( &(ViewingBox::windowSZ),
                            "/waterAnimCanvas.vs",
                            "/waterAnimCanvas.fs" );


    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "u_time" ); //- 1-float
    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "canvasCFPos" ); //- 2-float

    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "SCR_WIDTH" ); //- 1-float
    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "SCR_HEIGHT" ); //- 1-float

    //-- 以下 uniforms 只需传一次 --
    
    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "altiSeed_pposOffSeaLvl" ); //- 2-float
    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "altiSeed_pposOffBig" ); //- 2-float
    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "altiSeed_pposOffMid" ); //- 2-float
    canvas_inn::waterAnimCanvasUPtr->add_new_uniform( "altiSeed_pposOffSml" ); //- 2-float


}

/* ===========================================================
 *                    draw_groundCanvas
 * -----------------------------------------------------------
 */
void draw_groundCanvas(){

    canvas_inn::groundCanvasUPtr->use_shaderProgram(); //- MUST !!! 

    glUniform1f(canvas_inn::groundCanvasUPtr->get_uniform_location("u_time"), 
                static_cast<float>(glfwGetTime()) ); //- 1-float

    const glm::vec2 cameraFPos = glm_dvec2_2_vec2(get_camera().get_camera2DDPos());


    float windowSZ_fx = static_cast<float>(ViewingBox::windowSZ.x);
    float windowSZ_fy = static_cast<float>(ViewingBox::windowSZ.y);

    glm::vec2 canvasFPos = cameraFPos - glm::vec2{  0.5f * windowSZ_fx , 
                                                    0.5f * windowSZ_fy };

    canvas_inn::groundCanvasUPtr->set_translate( canvasFPos.x,
                                      canvasFPos.y,
                                      static_cast<float>(get_camera().get_zFar() + ViewingBox::ground_zOff) );
    
    glUniform2f(canvas_inn::groundCanvasUPtr->get_uniform_location("canvasCFPos"), 
                    canvasFPos.x / PIXES_PER_CHUNK,
                    canvasFPos.y / PIXES_PER_CHUNK ); //- 2-float
    
    glUniform1f(canvas_inn::groundCanvasUPtr->get_uniform_location("SCR_WIDTH"), windowSZ_fx ); //- 1-float
    glUniform1f(canvas_inn::groundCanvasUPtr->get_uniform_location("SCR_HEIGHT"), windowSZ_fy ); //- 1-float

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

    const glm::vec2 cameraFPos = glm_dvec2_2_vec2(get_camera().get_camera2DDPos());

    float windowSZ_fx = static_cast<float>(ViewingBox::windowSZ.x);
    float windowSZ_fy = static_cast<float>(ViewingBox::windowSZ.y);

    glm::vec2 canvasFPos = cameraFPos - glm::vec2{  0.5f * windowSZ_fx , 
                                                    0.5f * windowSZ_fy };

    canvas_inn::waterAnimCanvasUPtr->set_translate(canvasFPos.x,
                                        canvasFPos.y,
                                        static_cast<float>(get_camera().get_zFar() + ViewingBox::waterAnim_zOff) );
                                        //- 这一步是正确的，canvas 与 window 成功对齐，
                                        //  进而可知，SCR_WIDTH，SCR_HEIGHT 的使用也是正确的

    glUniform2f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("canvasCFPos"), 
                    canvasFPos.x / PIXES_PER_CHUNK,
                    canvasFPos.y / PIXES_PER_CHUNK ); //- 2-float

    
    //-- 每个游戏存档的这组值 其实是固定的，游戏运行期间，只需传输一次 --
    GameSeed &gameSeedRef = get_gameSeed();
    const glm::dvec2 &altiSeed_pposOffSeaLvl = gameSeedRef.get_altiSeed_pposOffSeaLvl();
    const glm::dvec2 &altiSeed_pposOffBig    = gameSeedRef.get_altiSeed_pposOffBig();
    const glm::dvec2 &altiSeed_pposOffMid    = gameSeedRef.get_altiSeed_pposOffMid();
    const glm::dvec2 &altiSeed_pposOffSml    = gameSeedRef.get_altiSeed_pposOffSml();

    glUniform1f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("SCR_WIDTH"), windowSZ_fx ); //- 1-float
    glUniform1f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("SCR_HEIGHT"), windowSZ_fy ); //- 1-float

    if( canvas_inn::is_waterAnim_baseUniforms_transmited == false ){
        canvas_inn::is_waterAnim_baseUniforms_transmited = true;

        glUniform2f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("altiSeed_pposOffSeaLvl"), 
                    static_cast<float>(altiSeed_pposOffSeaLvl.x),
                    static_cast<float>(altiSeed_pposOffSeaLvl.y) ); //- 2-float
        
        glUniform2f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("altiSeed_pposOffBig"), 
                    static_cast<float>(altiSeed_pposOffBig.x),
                    static_cast<float>(altiSeed_pposOffBig.y) ); //- 2-float
        
        glUniform2f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("altiSeed_pposOffMid"), 
                    static_cast<float>(altiSeed_pposOffMid.x),
                    static_cast<float>(altiSeed_pposOffMid.y) ); //- 2-float

        glUniform2f(canvas_inn::waterAnimCanvasUPtr->get_uniform_location("altiSeed_pposOffSml"), 
                    static_cast<float>(altiSeed_pposOffSml.x),
                    static_cast<float>(altiSeed_pposOffSml.y) ); //- 2-float
    }

    canvas_inn::waterAnimCanvasUPtr->draw();
}



}//---------------------- namespace: esrc -------------------------//

