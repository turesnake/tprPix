// gpgpu FBO -- pix
#version 330 core

// 由用户程序中 glVertexAttribPointer() 配置 (VAOVBO 模块中)--
layout (location = 0) in vec3 aPos;      //-- 每个pix 在 图元上的坐标 [-1.0,1.0]
layout (location = 1) in vec2 aTexCoord; //-- 每个pix 在 tecture 上的坐标 [0.0,1.0]-[left_bottom]


out vec2 TexCoord;     //-- 每个pix 在 tecture 上的坐标 [0.0,1.0] [left_bottom]

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position =  projection * view * model * vec4( aPos, 1.0 ); 
    TexCoord = aTexCoord;
}

