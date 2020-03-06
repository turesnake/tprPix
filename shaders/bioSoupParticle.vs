/*
 * ===================== bioSoupParticle.vs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  RenderLayerType::
 */
#version 330 core

// 由用户程序中 glVertexAttribPointer() 配置  (VAOVBO 模块中) --
layout (location = 0) in vec3 aPos;      //-- 每个pix 在 图元上的坐标 [-1.0,1.0]
layout (location = 1) in vec2 aTexCoord; //-- 每个pix 在 tecture 上的坐标 [0.0,1.0]

//-- 输出给 fs 的颜色数据
out vec2 TexCoord;  //-- 每个pix 在 tecture 上的坐标 [-1.0,1.0]

uniform mat4 model;

//===== UBO =====//
layout (shared, std140) uniform Camera {
    mat4 view;
    mat4 projection;
    vec2 canvasCFPos;
} camera;

//-- 顶点着色器，只需要 每个顶点 运行一次 --
//  在我们的 pix 游戏中，所有 图元都是 4个顶点的 矩形。
//  所以，顶点着色器的 工作负担，要比 片段着色器 轻很多
void main()
{
    gl_Position =  camera.projection * camera.view * model * vec4( aPos, 1.0 ); 
    TexCoord = aTexCoord;
}

