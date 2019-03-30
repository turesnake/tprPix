// gpgpu FBO -- pix
#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- in: 从 顶点着色器 传来的 数据（名称和类型 都要相同）
in vec2 TexCoord;      //-- 每个pix 在 tecture 上的坐标 [0.0,1.0] [left_bottom]
in vec2 u_resolution;  //-- 每个pix 在 window 上的坐标  [-1.0,1.0]


//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 1个 纹理采样器
uniform sampler2D texture1;

//---- 用户定制 uniforms -----
uniform int chunkCPosX; //- 以 chunk 为单位的 cpox
uniform int chunkCPosY;

uniform float altiSeed; //- 影响 simplex-noise.z 值。充当种子

//-- chunk 周边4个点，记录 海平面高度 节点 --
// chunk 内每个pix，根据这4个值 平滑得出
uniform float seaLvl_leftBottom;
uniform float seaLvl_rightBottom;
uniform float seaLvl_leftTop;
uniform float seaLvl_rightTop;

/*
uniform float freq1;
uniform float freq2;
uniform float freq3;
uniform float freq4;

uniform float seed1;
uniform float seed2;
uniform float seed3;
uniform float seed4;
*/



//============ vals ===========//
vec2 lb; //- [left_bottom] [0,1]
//vec2 lt; //- [left_top] [0,1] 和 鼠标同坐标系
//-------

float PIXES_PER_CHUNK = 256.0;

float freqBig = 0.4;
float freqMid = 2.0;
float freqSml = 4.0;

float zOffBig = 0.2;
float zOffMid = 0.5;
float zOffSml = 0.8;

float seaLvl;  //- 海平面。 值越小，land区越大。通过平滑曲线生成
                      


//============ funcs ===========//
//-- qualifer/限定符：
// in     -- 只读
// out    -- 只写
// inout  -- 可读可写
void prepare();
float simplex_noise3( float _x, float _y, float _z );
float mod_(float x, float _mod);


/* ====================================================
 *                      main
 * ----------------------------------------------------
 */
void main()
{
    prepare();

    //------------------//
    // pixFCPos: 以 chunk 为晶格的 fpos
    vec2 chunkPPos = vec2( chunkCPosX, chunkCPosY );
    vec2 pixFCPos = chunkPPos + lb;

    //------------------//
    // calc each pix.seaLvl
    vec2 smoothVal = smoothstep( 0.0, 1.0, lb/PIXES_PER_CHUNK );
    float vBottom = smoothVal.x * (seaLvl_rightBottom-seaLvl_leftBottom) + seaLvl_leftBottom;
    float vTop    = smoothVal.x * (seaLvl_rightTop-seaLvl_leftTop) + seaLvl_leftTop;
    seaLvl = smoothVal.y * (vTop-vBottom) + vBottom;
    //seaLvl = 0.0; //- tmp...

    //------------------//
    //    alti.val
    //------------------//
    float pnValBig = simplex_noise3(pixFCPos.x * freqBig, 
                                    pixFCPos.y * freqBig,
                                    (altiSeed+zOffBig) ) * 100.0 - seaLvl; // [-100.0, 100.0]

    float pnValMid = simplex_noise3(pixFCPos.x * freqMid, 
                                    pixFCPos.y * freqMid,
                                    (altiSeed+zOffMid) ) * 50.0 - seaLvl; // [-50.0, 50.0]

    float pnValSml  = simplex_noise3(pixFCPos.x * freqSml, 
                                    pixFCPos.y * freqSml,
                                    (altiSeed+zOffSml) ) * 30.0 - seaLvl; // [-30.0, 30.0]

    float altiVal = floor(pnValBig + pnValMid + pnValSml);

    //------- 抹平头尾 -------//
    if( altiVal > 100.0 ){
        altiVal = 100.0;
    }else if( altiVal < -100.0 ){
        altiVal = -100.0;
    }
    // now, altiVal: [-100.0, 100.0]


        //....


    FragColor = vec4( altiVal, 0.0, 0.0, 1.0 ); //- rgba.alpha MUST be 1.0 !!!
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
    //-- 左上 坐标系 [0,1]
    //lt.x = TexCoord.x; //-- [0,1]
    //lt.y = 1.0 - TexCoord.y; //-- [0,1]
}


/* ====================================================
 *                     mod_
 * ----------------------------------------------------
 */
float mod_(float x, float _mod){
  return x - floor(x * (1.0 / _mod)) * _mod;
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
vec3 mod289(vec3 x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x){ return mod289(((x*34.0)+1.0)*x); }
vec4 taylorInvSqrt(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }

//-- ret: [-1.0, 1.0]
float simplex_noise3( float _x, float _y, float _z ){ 
  vec3 v = vec3( _x, _y, _z );
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
  // First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;
  // Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );
  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y
  // Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
  // Gradients: 7x7 points over a square, mapped onto an octahedron.
  // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);
  //Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;
  // Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}


