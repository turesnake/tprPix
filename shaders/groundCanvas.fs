// gpgpu FBO -- 现在正在履行 skyCanvas 的功能
#version 330 core

//-- in: 从 顶点着色器 传来的 数据（名称和类型 都要相同）
in vec2 TexCoord;      //-- 每个pix 在 tecture 上的坐标 [0.0,1.0]-[left_bottom]

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 1个 纹理采样器
uniform sampler2D texture1; //- 好像没什么用

uniform float u_time; //-- glfwGetTime(); 未做缩放。
                      // 在一些 必须 [0.0, 1.0] 的使用场合中，
                      // 通过 abs(sin(u_time)) 来转换

//---- 用户定制 uniforms -----
uniform vec2 canvasCFPos; //- 以 chunk 为单位的, canvas左下角 cfpox
                          // 参见 waterAnim 中的同款值，让 sky 微弱的运动

//- 对于每个游戏存档，这组值是静态的，只需要传入一次...
uniform float SCR_WIDTH;
uniform float SCR_HEIGHT;


float freqCloud = 1.4;
float freqSml   = 4.4;

//============ vals ===========//
vec2 lb; //- [left_bottom] [0,1]
vec2 lbAlign; //- 对齐过的 lb，首先，校准了 y值比例（毕竟window可能不是正方形）
              //  其次，1个单位 lbAlign，等于 chunk-ppos 尺寸，而不是 canvas尺寸

vec3 color;

float PIXES_PER_CHUNK = 256.0; //- tmp


//============ funcs ===========//
//-- qualifer/限定符：
// in     -- 只读
// out    -- 只写
// inout  -- 可读可写
void prepare();
float simplex_noise2( float _x, float _y );
float simplex_noise2( vec2 v );

/* ====================================================
 *                      main
 * ----------------------------------------------------
 */
void main()
{
    
    prepare();

    //------------------//
    // pixCFPos: 以 chunk 为晶格的 fpos
    vec2 pixCFPos = 0.08 * canvasCFPos + lbAlign;
    pixCFPos.x += u_time * 0.05;
            //- 不完全对齐，让它运动得慢些

    pixCFPos *= 240.0;
    pixCFPos = floor(pixCFPos);
    pixCFPos =  pixCFPos / 240.0;


    //------------------//
    //     cloud
    //------------------//
    float pnValCloud = simplex_noise2( pixCFPos * freqCloud ) * 50.0; // [-50.0, 50.0]
    float pnValSml   = simplex_noise2( (pixCFPos+100.0) * freqSml ) * 30.0; // [-50.0, 50.0]

    pnValCloud += pnValSml;

    vec3 colorCloud = vec3( 1.0, 1.0, 1.0 );
    vec3 colorSky   = vec3( 0.6, 0.7, 0.8 );

    float yv = lb.x*lb.x;
    if( pnValCloud >= 30.0 ){ //- cloud
        color = mix( colorSky, colorCloud, smoothstep(0.0, 1.0, yv) );
    }else{
        color = colorSky;
    }


    //FragColor = vec4( lb.xxy, 1.0 ); 
    FragColor = vec4( color, 1.0 ); 
    


    //FragColor = vec4( 0.8, 0.8, 0.3, 1.0 );
}


/* ====================================================
 *                     prepare
 * ----------------------------------------------------
 * -- 准备工作
 */
void prepare(){
    // TexCoord:      每个pix 在 tecture 上的坐标 [0.0,1.0]
    // u_resolution:  每个pix 在 window 上的坐标  [-1.0,1.0]
    //--------------------------//
    //    初始化 lb, lt
    //--------------------------//
    //-- 左下坐标系 [0,1]
    lb = TexCoord;
    //---
    lbAlign = lb;
    lbAlign.y *= SCR_HEIGHT/SCR_WIDTH;
    lbAlign *= SCR_WIDTH/PIXES_PER_CHUNK;
}




/* ====================================================
 *                 Simplex Noise
 * ----------------------------------------------------
 *  Description : Array and textureless GLSL 2D/3D/4D simplex noise functions.
 *       Author : Ian McEwan, Ashima Arts.     
 *   Maintainer : stegu
 *      Lastmod : 20110822 (ijm)
 *      License : Copyright (C) 2011 Ashima Arts. All rights reserved.
 *                Distributed under the MIT License. See LICENSE file.
 *                https://github.com/ashima/webgl-noise
 *                https://github.com/stegu/webgl-noise
 */
vec2 mod289V2(vec2 x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 mod289V3(vec3 x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289V4(vec4 x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permuteV3(vec3 x){ return mod289V3(((x*34.0)+1.0)*x); }
vec4 permuteV4(vec4 x){ return mod289V4(((x*34.0)+1.0)*x); }
vec4 taylorInvSqrtV4(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }

//------------------- ret: [-1.0, 1.0] ---------------------
float simplex_noise2( float _x, float _y ){
    return simplex_noise2( vec2(_x, _y) );
}

//------------------- ret: [-1.0, 1.0] ---------------------
float simplex_noise2( vec2 v ){
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
  // First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  // Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  // Permutations
  i = mod289V2(i); // Avoid truncation effects in permutation
  vec3 p = permuteV3( permuteV3( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  // Gradients: 41 points uniformly over a line, mapped onto a diamond.
  // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  // Normalise gradients implicitly by scaling m
  // Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  // Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

