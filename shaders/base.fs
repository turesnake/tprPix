#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- 从 顶点着色器 传来的 数据（名称和类型 都要相同）
//in vec3 ourColor;
//in vec2 TexCoord;

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 2个 纹理采样器
//uniform sampler2D texture1;
//uniform sampler2D texture2;

//sampler2D k = 0; 

void main()
{
    FragColor = vec4( 1,0f, 0.5f, 0.2f, 1.0f );
    //FragColor = mix(texture(k, TexCoord), texture(k, TexCoord), 0.8 );
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2 );
                    //- 使用 texture 内建函数 来采样纹理的颜色。
                    //- param: texture -- 纹理采样器 的 号码
                    //- param: TexCoord   -- 对应的 纹理坐标
}
