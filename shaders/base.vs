#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


//-- 输出给 fs 的颜色数据
out vec2 TexCoord;

uniform mat4 transform; //-- 矩阵 变换 变量。

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


//-- 顶点着色器，只需要 每个顶点 运行一次 --
//  在我们的 pix 游戏中，所有 图元都是 4个顶点的 矩形。
//  所以，顶点着色器的 工作负担，要比 片段着色器 轻很多
void main()
{
    gl_Position =  projection * view * model * vec4( aPos, 1.0 ); 

    TexCoord = vec2(aTexCoord.x, aTexCoord.y);

}

