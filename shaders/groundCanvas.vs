/*
 * ====================== groundCanvas.vs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.31
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  RenderLayerType::Ground
 *  颜色跟随 ecoObj 的改变而改变
 */
#version 330 core

// 由用户程序中 glVertexAttribPointer() 配置 (VAOVBO 模块中)--
layout (location = 0) in vec3 aPos;      //-- 每个pix 在 图元上的坐标 [-1.0,1.0]
layout (location = 1) in vec2 aTexCoord; //-- 每个pix 在 tecture 上的坐标 [0.0,1.0]-[left_bottom]

out vec2 TexCoord;     //-- 每个pix 在 tecture 上的坐标 [0.0,1.0] [left_bottom]

uniform mat4 model;

//===== UBO =====//
layout (shared, std140) uniform Camera {
    mat4 view;
    mat4 projection;
    vec2 canvasCFPos;
} camera;


void main()
{
    gl_Position =  camera.projection * camera.view * model * vec4( aPos, 1.0 ); 
    TexCoord = aTexCoord;
}

