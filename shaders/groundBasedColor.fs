/*
 * ==================== groundBasedColor.fs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.20
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  RenderLayerType::
 *  一种基于 ground 的上色体系，和 unifiedColor 系统平行
 *  animFrameSet png资源，不再标注自己的颜色绝对值，而是一个基于 groundColor 的偏移idx
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
/*
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
*/

// tmp, 手动设置的 5种 基色，0号色，对齐于 最接近 groundColor 的那个颜色
// 剩余的 4 种为 偏移值
// 所以不符合这5种颜色的 数据，都被设置为 0号色
vec3 baseColor_m2 = vec3( 0.706, 0.706, 1.0 );   // {180, 180, 255}  more light
vec3 baseColor_m1 = vec3( 0.588, 0.588, 1.0 );   // {150, 150, 255}
vec3 baseColor_0  = vec3( 0.392, 0.392, 1.0 );   // {100, 100, 255}  0
vec3 baseColor_1  = vec3( 0.196, 0.196, 0.784 ); // {50,  50,  200}
vec3 baseColor_2  = vec3( 0.0,   0.0,   0.392 ); // {0,   0,   100}  more dark

//-- the colorEntIdx similar to groundColor
uniform int groundLikeColorIdx;


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

    

    //FragColor = texColor;

    vec3 outColor3 = change_color( texColor.rgb );
    FragColor = vec4( outColor3.rgb, texColor.a );
}



vec3 change_color( in vec3 val_ ){

    int offIdx;
    if(       closeEnough( val_, baseColor_m2) ){
        offIdx = -2;
    }else if( closeEnough( val_, baseColor_m1) ){
        offIdx = -1;
    }else if( closeEnough( val_, baseColor_0) ){
        offIdx = 0;
    }else if( closeEnough( val_, baseColor_1) ){
        offIdx = 1;
    }else if( closeEnough( val_, baseColor_2) ){
        offIdx = 2;
    }else{
        discard;
        offIdx = 1;
    }
    //---
    int idx = groundLikeColorIdx + offIdx;
    if( idx < 0 ){
        //discard;
        idx = 0;
    }
    if( idx > 7 ){
        //discard;
        idx = 7; //-- tmp， 手动设置上边界
    }
    //---
    switch( idx ){
        case 0: return unifiedColorTable.base_0.rgb; break;
        case 1: return unifiedColorTable.base_1.rgb; break;
        case 2: return unifiedColorTable.base_2.rgb; break;
        case 3: return unifiedColorTable.base_3.rgb; break;
        case 4: return unifiedColorTable.base_4.rgb; break;
        case 5: return unifiedColorTable.base_5.rgb; break;
        case 6: return unifiedColorTable.base_6.rgb; break;
        case 7: return unifiedColorTable.base_7.rgb; break;
        default:
            discard;
            return unifiedColorTable.base_0.rgb; // tmp
            break;
    }
}



bool closeEnough( in vec3 a_, in vec3 b_ ){
    float threshold = 0.01;
    vec3 off = abs(a_ - b_);
    if( (off.r <= threshold) &&
        (off.g <= threshold) &&
        (off.b <= threshold) ){
        return true;
    }else{
        return false;
    }
}

