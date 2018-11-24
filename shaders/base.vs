#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


//-- 输出给 fs 的颜色数据
out vec2 TexCoord;

uniform mat4 transform; //-- 矩阵 变换 变量。

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position =  projection * view * model * vec4( aPos, 1.0 ); 

    TexCoord = vec2(aTexCoord.x, aTexCoord.y);

}

