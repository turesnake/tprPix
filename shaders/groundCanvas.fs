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


/* ====================================================
 *                      main
 * ----------------------------------------------------
 */
void main()
{
    //FragColor = vec4( unifiedColorTable.ground.rgb, 1.0 );
    FragColor = vec4( 0.2, 0.2, 0.2, 1.0 );
}

