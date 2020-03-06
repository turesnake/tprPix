/*
 * ===================== bioSoupBase.fs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  RenderLayerType::
 */
#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- 从 顶点着色器 传来的 数据（名称和类型 都要相同）
in vec2 TexCoord;   //-- 每个pix 在 tecture 上的坐标 [0.0, 1.0] [left_bottom]

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 1个 纹理采样器
uniform sampler2D texture1;


// origin colors
vec3 base_light     = vec3( 0.784, 0.784, 0.784 ); // 200
vec3 base_mid       = vec3( 0.588, 0.588, 0.588 ); // 150
vec3 base_dark      = vec3( 0.392, 0.392, 0.392 ); // 100
//--


//===== UBO =====//
layout (shared, std140) uniform BioSoupColorTable {
    vec4 base_light;
    vec4 base_mid;
    vec4 base_dark;
    //---
    vec4 particle_light;
    vec4 particle_mid;
    vec4 particle_dark;
} bioSoupColorTable;


//===== funcs =====//
vec3 change_color( in vec3 val_ );
bool closeEnough( in vec3 a_, in vec3 b_ );

void main()
{
    //-- 取得 texture 颜色 --
    vec4 texColor = texture(texture1, TexCoord);

    //-- 半透明度小于 0.05 的像素直接不渲染 --
    //-- 通过这个方法来处理大部分 “要么实心要么全透明” 的图元
    if( texColor.a < 0.05 ){
        discard;
    }

    vec3 outColor3 = change_color( texColor.rgb );

    FragColor = vec4( outColor3.rgb, texColor.a );
}


vec3 change_color( in vec3 val_ ){
    if(       closeEnough( val_, base_light     ) ){ return bioSoupColorTable.base_light.rgb;
    }else if( closeEnough( val_, base_mid       ) ){ return bioSoupColorTable.base_mid.rgb;
    }else if( closeEnough( val_, base_dark      ) ){ return bioSoupColorTable.base_dark.rgb;
    }else{
        return vec3( 0.0, 0.0, 0.0 ); // error
    }
}


bool closeEnough( in vec3 a_, in vec3 b_ ){
    float threshold = 0.01;
    vec3 off = abs(a_ - b_);
    return ((off.r <= threshold) &&
            (off.g <= threshold) &&
            (off.b <= threshold) );
}

