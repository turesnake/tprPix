/*
 * ====================== groundCanvas.fs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.31
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  RenderLayerType::Ground
 *  颜色跟随 ecoObj 的改变而改变
 */
#version 330 core

//-- in: 从 顶点着色器 传来的 数据（名称和类型 都要相同）
in vec2 TexCoord;      //-- 每个pix 在 tecture 上的坐标 [0.0,1.0]-[left_bottom]

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 1个 纹理采样器
uniform sampler2D texture1; //- 好像没什么用

uniform float u_time; //-- glfwGetTime(); 未做缩放。
                      // 在一些 必须 [0.0, 1.0] 的使用场合中，
                      // 通过 abs(sin(u_time)) 来转换

//---- 用户定制 uniforms -----
uniform vec2 canvasCFPos; //- 以 chunk 为单位的, canvas左下角 cfpox
                          // 参见 waterAnim 中的同款值，让 sky 微弱的运动

//- 对于每个游戏存档，这组值是静态的，只需要传入一次...
// canvas 本质就是一个 texture，此size（pix）通常等于 gameSZ
uniform float texSizeW;
uniform float texSizeH;


//===== UBO =====//

layout (shared, std140) uniform UnifiedColorTable {
    vec4 base_0;
    vec4 base_1;
    vec4 base_2;
    vec4 base_3;
    vec4 base_4;
    vec4 base_5;
    vec4 base_6;
    vec4 base_7;
    //---
    vec4 ground;
    //---
} unifiedColorTable;



//---- test -----//
layout (std140) uniform Mat0
{
    vec4 testColor_1;
    vec4 testColor_2;
};


float freqCloud = 1.4;
float freqSml   = 4.4;

//============ funcs ===========//
//-- qualifer/限定符：
// in     -- 只读
// out    -- 只写
// inout  -- 可读可写




/* ====================================================
 *                      main
 * ----------------------------------------------------
 */
void main()
{

    //prepare();
    //discard;
    //FragColor = vec4( 0.98, 0.96, 0.85, 1.0 );//- 色1(最浅色)
    //FragColor = vec4( testColor_1.rgb, 1.0 );//- 色2
    FragColor = vec4( unifiedColorTable.ground.rgb, 1.0 );
}





