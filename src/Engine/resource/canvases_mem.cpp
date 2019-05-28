/*
 * ================= canvases_mem.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
//-------------------- CPP --------------------//
//#include <string>

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "esrc_canvas.h"
#include "esrc_window.h"
#include "esrc_camera.h"
#include "esrc_gameSeed.h"


//#include "tprDebug.h" //- tmp

namespace esrc{//------------------ namespace: esrc -------------------------//


namespace {//-------- namespace: --------------//

    bool  is_ground_baseUniforms_transmited {false};
    bool  is_waterAnim_baseUniforms_transmited {false}; //- pixGpgpu 的几个 静态uniform值 是否被传输
                                        // 这些值是固定的，每次游戏只需传入一次...

}//------------- namespace: end --------------//


/* ===========================================================
 *                     init_canvases
 * -----------------------------------------------------------
 */
void init_canvases(){

    //------------------//
    //    groundCanvas
    //------------------//
    /*
    esrc::groundCanvas.init( IntVec2{ _SCR_WIDTH, _SCR_HEIGHT },
                            "/groundCanvas.vs",
                            "/groundCanvas.fs" );
    */

    esrc::groundCanvas.init( &(ViewingBox::windowSZ),
                            "/groundCanvas.vs",
                            "/groundCanvas.fs" );

    esrc::groundCanvas.add_new_uniform( "u_time" ); //- 1-float
    esrc::groundCanvas.add_new_uniform( "canvasCFPos" ); //- 2-float
    

    esrc::groundCanvas.add_new_uniform( "SCR_WIDTH" ); //- 1-float
    esrc::groundCanvas.add_new_uniform( "SCR_HEIGHT" ); //- 1-float
                        //-- 当 窗口发生变化，此组值需要被重传



    //------------------//
    //    waterAnimCanvas
    //------------------//
    /*
    esrc::waterAnimCanvas.init( IntVec2{ _SCR_WIDTH, _SCR_HEIGHT },
                            "/waterAnimCanvas.vs",
                            "/waterAnimCanvas.fs" );
    */

    esrc::waterAnimCanvas.init( &(ViewingBox::windowSZ),
                            "/waterAnimCanvas.vs",
                            "/waterAnimCanvas.fs" );


    esrc::waterAnimCanvas.add_new_uniform( "u_time" ); //- 1-float
    esrc::waterAnimCanvas.add_new_uniform( "canvasCFPos" ); //- 2-float

    esrc::waterAnimCanvas.add_new_uniform( "SCR_WIDTH" ); //- 1-float
    esrc::waterAnimCanvas.add_new_uniform( "SCR_HEIGHT" ); //- 1-float

    //-- 以下 uniforms 只需传一次 --
    
    esrc::waterAnimCanvas.add_new_uniform( "altiSeed_pposOffSeaLvl" ); //- 2-float
    esrc::waterAnimCanvas.add_new_uniform( "altiSeed_pposOffBig" ); //- 2-float
    esrc::waterAnimCanvas.add_new_uniform( "altiSeed_pposOffMid" ); //- 2-float
    esrc::waterAnimCanvas.add_new_uniform( "altiSeed_pposOffSml" ); //- 2-float



}

/* ===========================================================
 *                    draw_groundCanvas
 * -----------------------------------------------------------
 */
void draw_groundCanvas(){

    esrc::groundCanvas.use_shaderProgram(); //- MUST !!! 

    glUniform1f(esrc::groundCanvas.get_uniform_location("u_time"), 
                    (float)glfwGetTime() ); //- 1-float

    const glm::vec2 cameraFPos = esrc::camera.get_camera2DFPos();


    float windowSZ_fx = static_cast<float>(ViewingBox::windowSZ.x);
    float windowSZ_fy = static_cast<float>(ViewingBox::windowSZ.y);


    //glm::vec2 canvasFPos = cameraFPos - glm::vec2{ 0.5*_SCR_WIDTH, 0.5*_SCR_HEIGHT};
    glm::vec2 canvasFPos = cameraFPos - glm::vec2{  0.5 * windowSZ_fx , 
                                                    0.5 * windowSZ_fy };

    esrc::groundCanvas.set_translate( canvasFPos.x,
                                      canvasFPos.y,
                                      esrc::camera.get_zFar() + ViewingBox::ground_zOff );
    
    glUniform2f(esrc::groundCanvas.get_uniform_location("canvasCFPos"), 
                    canvasFPos.x / PIXES_PER_CHUNK,
                    canvasFPos.y / PIXES_PER_CHUNK ); //- 2-float
    
    //-- 每个游戏存档的这组值 其实是固定的，游戏运行期间，只需传输一次 --
    /*
    if( is_ground_baseUniforms_transmited == false ){
        is_ground_baseUniforms_transmited = true;
        glUniform1f(esrc::groundCanvas.get_uniform_location("SCR_WIDTH"), windowSZ_fx ); //- 1-float
        glUniform1f(esrc::groundCanvas.get_uniform_location("SCR_HEIGHT"), windowSZ_fy ); //- 1-float
    }
    */
    glUniform1f(esrc::groundCanvas.get_uniform_location("SCR_WIDTH"), windowSZ_fx ); //- 1-float
    glUniform1f(esrc::groundCanvas.get_uniform_location("SCR_HEIGHT"), windowSZ_fy ); //- 1-float

    esrc::groundCanvas.draw();
}


/* ===========================================================
 *                  draw_waterAnimCanvas
 * -----------------------------------------------------------
 */
void draw_waterAnimCanvas(){

    esrc::waterAnimCanvas.use_shaderProgram(); //- MUST !!! 

    glUniform1f(esrc::waterAnimCanvas.get_uniform_location("u_time"), 
                    (float)glfwGetTime() ); //- 1-float

    const glm::vec2 cameraFPos = esrc::camera.get_camera2DFPos();

    float windowSZ_fx = static_cast<float>(ViewingBox::windowSZ.x);
    float windowSZ_fy = static_cast<float>(ViewingBox::windowSZ.y);

    //glm::vec2 canvasFPos = cameraFPos - glm::vec2{ 0.5*_SCR_WIDTH, 0.5*_SCR_HEIGHT};
    glm::vec2 canvasFPos = cameraFPos - glm::vec2{  0.5 * windowSZ_fx , 
                                                    0.5 * windowSZ_fy };

    esrc::waterAnimCanvas.set_translate(canvasFPos.x,
                                        canvasFPos.y,
                                        esrc::camera.get_zFar() + ViewingBox::waterAnim_zOff );
                                        //- 这一步是正确的，canvas 与 window 成功对齐，
                                        //  进而可知，SCR_WIDTH，SCR_HEIGHT 的使用也是正确的

    glUniform2f(esrc::waterAnimCanvas.get_uniform_location("canvasCFPos"), 
                    canvasFPos.x / PIXES_PER_CHUNK,
                    canvasFPos.y / PIXES_PER_CHUNK ); //- 2-float

    
    //-- 每个游戏存档的这组值 其实是固定的，游戏运行期间，只需传输一次 --
    const glm::vec2 &altiSeed_pposOffSeaLvl = esrc::gameSeed.get_altiSeed_pposOffSeaLvl();
    const glm::vec2 &altiSeed_pposOffBig = esrc::gameSeed.get_altiSeed_pposOffBig();
    const glm::vec2 &altiSeed_pposOffMid = esrc::gameSeed.get_altiSeed_pposOffMid();
    const glm::vec2 &altiSeed_pposOffSml = esrc::gameSeed.get_altiSeed_pposOffSml();

    glUniform1f(esrc::waterAnimCanvas.get_uniform_location("SCR_WIDTH"), windowSZ_fx ); //- 1-float
    glUniform1f(esrc::waterAnimCanvas.get_uniform_location("SCR_HEIGHT"), windowSZ_fy ); //- 1-float

    if( is_waterAnim_baseUniforms_transmited == false ){
        is_waterAnim_baseUniforms_transmited = true;
        //glUniform1f(esrc::waterAnimCanvas.get_uniform_location("SCR_WIDTH"), windowSZ_fx ); //- 1-float
        //glUniform1f(esrc::waterAnimCanvas.get_uniform_location("SCR_HEIGHT"), windowSZ_fx ); //- 1-float

        glUniform2f(esrc::waterAnimCanvas.get_uniform_location("altiSeed_pposOffSeaLvl"), 
                    altiSeed_pposOffSeaLvl.x,
                    altiSeed_pposOffSeaLvl.y ); //- 2-float
        
        glUniform2f(esrc::waterAnimCanvas.get_uniform_location("altiSeed_pposOffBig"), 
                    altiSeed_pposOffBig.x,
                    altiSeed_pposOffBig.y ); //- 2-float
        
        glUniform2f(esrc::waterAnimCanvas.get_uniform_location("altiSeed_pposOffMid"), 
                    altiSeed_pposOffMid.x,
                    altiSeed_pposOffMid.y ); //- 2-float

        glUniform2f(esrc::waterAnimCanvas.get_uniform_location("altiSeed_pposOffSml"), 
                    altiSeed_pposOffSml.x,
                    altiSeed_pposOffSml.y ); //- 2-float
    }

    esrc::waterAnimCanvas.draw();
}







}//---------------------- namespace: esrc -------------------------//

