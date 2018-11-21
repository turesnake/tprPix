#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

in vec3 Normal; //-- 每个顶点的 法向量
in vec3 FragPos; //-- 顶点的世界空间坐标

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 2个 纹理采样器
uniform sampler2D texture1;
uniform sampler2D texture2;

//-- obj颜色，光照颜色
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos; //-- 光源坐标
uniform vec3 viewPos;  //-- 摄像机坐标



float ambientStrength = 0.2; //-- 环境光程度 --
vec3 ambient; //-- 计算获得 环境光颜色
vec3 result;  //-- 物体最终颜色

float specularStrength = 0.5; //-- 镜面反射程度

void main()
{
    
    ambient = ambientStrength * lightColor;

    //-- diffuse 漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize( lightPos - FragPos );
    float diff = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = diff * lightColor;

    //-- specular 镜面反射
    vec3 viewDir = normalize( viewPos - FragPos );
    vec3 reflectDir = reflect( -lightDir, norm );
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 


    result = (ambient + diffuse + specular ) * objectColor;

    FragColor = vec4( result, 1.0 );
                    //-- 将光源颜色 和 物体固有色 简单相乘，便获得最终呈现色。
                    //--
                    //--
}
