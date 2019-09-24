// gpgpu FBO -- pix
#version 330 core

// 由用户程序中 glVertexAttribPointer() 配置 (VAOVBO 模块中)--
layout (location = 0) in vec3 aPos;      //-- 每个pix 在 图元上的坐标 [-1.0,1.0]
layout (location = 1) in vec2 aTexCoord; //-- 每个pix 在 tecture 上的坐标 [0.0,1.0]-[left_bottom]


out vec2 TexCoord;     //-- 每个pix 在 tecture 上的坐标 [0.0,1.0] [left_bottom]
out vec2 u_resolution; //-- 每个pix 在 window 上的坐标  [-1.0,1.0]


void main()
{
    gl_Position =  vec4( aPos.x, aPos.y, 0.0, 1.0 );
    //------
    TexCoord = aTexCoord;
    u_resolution = vec2( gl_Position.x, gl_Position.y );
}

