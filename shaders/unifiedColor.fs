/*
 * ===================== unifiedColor.fs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  RenderLayerType::
 *  颜色被统一为 unifiedColorTable
 */
#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- 从 顶点着色器 传来的 数据（名称和类型 都要相同）
in vec2 TexCoord;   //-- 每个pix 在 tecture 上的坐标 [0.0, 1.0] [left_bottom]

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 1个 纹理采样器
uniform sampler2D texture1;


//===== UBO =====//
layout (shared, std140) uniform OriginColorTable {
    vec4 base_0;
    vec4 base_1;
    vec4 base_2;
    vec4 base_3;
    vec4 base_4;
    vec4 base_5;
    vec4 base_6;
    vec4 base_7;
    //---
    vec4 ground;
    //---
} originColorTable;

layout (shared, std140) uniform UnifiedColorTable {
    vec4 base_0;
    vec4 base_1;
    vec4 base_2;
    vec4 base_3;
    vec4 base_4;
    vec4 base_5;
    vec4 base_6;
    vec4 base_7;
    //---
    vec4 ground;
    //---
} unifiedColorTable;


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

    //FragColor = texColor;
    FragColor = vec4( outColor3.rgb, texColor.a );
}


vec3 change_color( in vec3 val_ ){
    if(       closeEnough( val_, originColorTable.base_0.rgb) ){ return unifiedColorTable.base_0.rgb;
    }else if( closeEnough( val_, originColorTable.base_1.rgb) ){ return unifiedColorTable.base_1.rgb;
    }else if( closeEnough( val_, originColorTable.base_2.rgb) ){ return unifiedColorTable.base_2.rgb;
    }else if( closeEnough( val_, originColorTable.base_3.rgb) ){ return unifiedColorTable.base_3.rgb;
    }else if( closeEnough( val_, originColorTable.base_4.rgb) ){ return unifiedColorTable.base_4.rgb;
    }else if( closeEnough( val_, originColorTable.base_5.rgb) ){ return unifiedColorTable.base_5.rgb;
    }else if( closeEnough( val_, originColorTable.base_6.rgb) ){ return unifiedColorTable.base_6.rgb;
    }else if( closeEnough( val_, originColorTable.base_7.rgb) ){ return unifiedColorTable.base_7.rgb;
    }else{
        return val_; // e.g. shadows
    }
}


bool closeEnough( in vec3 a_, in vec3 b_ ){
    float threshold = 0.01;
    vec3 off = abs(a_ - b_);
    return ((off.r <= threshold) &&
            (off.g <= threshold) &&
            (off.b <= threshold) );
}

