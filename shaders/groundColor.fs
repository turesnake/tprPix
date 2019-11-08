/*
 * ======================= groundColor.vs ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  世界地面
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
layout (shared, std140) uniform GroundColorTable {
    vec4 colors[32];  //-- 务必大于 json 中 colorTable 个数 -- 
} groundColorTable;

layout (shared, std140) uniform ColorTableId {
    int id;  //-- 未来可能被整合进一个 大杂烩 ubo 中
} colorTableId;


void main()
{
    //-- 取得 texture 颜色 --
    
    vec4 texColor = texture(texture1, TexCoord);

    //-- 半透明度小于 0.05 的像素直接不渲染 --
    //-- 通过这个方法来处理大部分 “要么实心要么全透明” 的图元
    if( texColor.a < 0.05 ){
        discard;
    }
    

    vec4 targetColor = groundColorTable.colors[colorTableId.id];
    //FragColor = vec4( targetColor.rgb, 0.8 );
            // 故意设置为 半透明，这样，当从一个 eco 进入另一个 eco，ground color 会发生轻微的变化


    FragColor = vec4( targetColor.rgb, 1.0 );
}


