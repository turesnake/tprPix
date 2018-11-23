#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; //-- 每个顶点的 法向量



out vec3 FragPos; //-- 顶点的世界空间坐标
out vec3 Normal;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position =  projection * view * model * vec4( aPos, 1.0 ); 


    FragPos = vec3(model * vec4(aPos, 1.0)); //-- 传输给 fs用
    
    Normal = mat3(transpose(inverse(model))) * aNormal; 
                //-- 一种更完善的计算 法向量 的方法。

}

