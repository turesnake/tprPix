//
//========================= __GLSL.h ==========================
//                         -- OpenGL --
//                                        创建 -- 2018.08.09
//                                        修改 -- 2018.08.09
//----------------------------------------------------------
//   着色器 程序 笔记
//   
//----------------------------

#define vec2 void*
#define vec3 void*
#define vec4 void*

#define bvec2 void*
#define bvec3 void*
#define bvec4 void*

#define ivec2 void*
#define ivec3 void*
#define ivec4 void*


//--------------- discard ----------------
//    if( color == black ){
//        discard;
//    }
//-----------------------
// glsl 中的一种 流控制语句，
// 使用 discard 会退出片段着色器，
// 不执行后面的片段着色操作。片段也不会写入帧缓冲区。
// 这将意味着，本像素 什么也不绘制。






//--------------- step ----------------
//-- param: edge  极限或阀值
//-- param: x     想要检测或通过的值，一般就是 x轴的值
//-- return
//--     若 x 小于 edge，返回 0.0
//--     若 x 大于等于 edge，返回 1.0
float step(float edge, float x);
vec2 step(vec2 edge, vec2 x); 
vec3 step(vec3 edge, vec3 x); 
vec4 step(vec4 edge, vec4 x);

vec2 step(float edge, vec2 x); 
vec3 step(float edge, vec3 x);
vec4 step(float edge, vec4 x);


//-------------- smoothstep -------------
//-- param: edge0  下限
//-- param: edge1  上限
//-- param: x   就是谁来代表 x轴
//
//-- smoothstep 的性质和 step 是一样的，只是多了个 柔和的过渡
float smoothstep(float edge0, float edge1, float x); 
vec2 smoothstep(vec2 edge0, vec2 edge1, vec2 x);
vec3 smoothstep(vec3 edge0, vec3 edge1, vec3 x);
vec4 smoothstep(vec4 edge0, vec4 edge1, vec4 x);

vec2 smoothstep(float edge0, float edge1, vec2 x); 
vec3 smoothstep(float edge0, float edge1, vec3 x); 
vec4 smoothstep(float edge0, float edge1, vec4 x);


//-- smoothstep( x-0.005, x, y ) -smoothstep( x, x+0.005, y );
//-- 通过两个 smoothstep 的组合，可以制作 一种平滑曲线，如上图
//-- 返回的值是一种 强度值。


//------------- pow -----------
//-- 计算 x 的 y 次方值
float pow(float x, float y);
vec2 pow(vec2 x, vec2 y);
vec3 pow(vec3 x, vec3 y);
vec4 pow(vec4 x, vec4 y);


//------------- exp -----------
//-- 计算 x 的 log e 的值
//-- 以自然常数e为底的指数函数
float exp(float x);
vec2 exp(vec2 x);
vec3 exp(vec3 x);
vec4 exp(vec4 x);


//------------- log -----------
//-- 对数函数
float log(float x);
vec2 log(vec2 x);
vec3 log(vec3 x);
vec4 log(vec4 x);


//------------- sqrt -----------
//-- 解平方根函数
//-- 运算效率不高
float sqrt(float x);
vec2 sqrt(vec2 x);
vec3 sqrt(vec3 x);
vec4 sqrt(vec4 x);


//------------- abs -----------
//-- 绝对值
float abs(float x); 
vec2 abs(vec2 x);
vec3 abs(vec3 x);
vec4 abs(vec4 x);

//------------- fract -----------
//-- 只选取 x 的小数部分
float fract(float x);
vec2 fract(vec2 x);
vec3 fract(vec3 x);
vec4 fract(vec4 x);

//------------- ceil -----------
//-- 使用向正无穷取整 
//-- 获得 比 x 大的那个整数
float ceil(float x);
vec2 ceil(vec2 x);
vec3 ceil(vec3 x);
vec4 ceil(vec4 x);

//------------- floor -----------
//-- 使用向负无穷取整 
//-- 获得 比 x 小的那个整数
float floor(float x);
vec2 floor(vec2 x);
vec3 floor(vec3 x);
vec4 floor(vec4 x);


//------------- mix -----------
//-- x,y是两个 目标值， a 是个百分比，表示 分别取多少x 和 y，然后混合。
//-- 比如，当a是 0.2 时。就取 20% 的x，和 80%的y
float mix(float x, float y, float a); 
vec2 mix(vec2 x, vec2 y, vec2 a);
vec3 mix(vec3 x, vec3 y, vec3 a);
vec4 mix(vec4 x, vec4 y, vec4 a);

vec2 mix(vec2 x, vec2 y, float a);
vec3 mix(vec3 x, vec3 y, float a);
vec4 mix(vec4 x, vec4 y, float a);



//------------- length -----------
//-- 计算 一个 向量的 长度值
//-- 依赖 sqrt，效率不高，建议用 dot 代替
//
//-- dot 和 sqrt系列函数的区别。 dot也是 x*x + y*y
//-- 但是 dot 没有执行 开平方。 可以看作一种 近似解法
float length(float x); 
float length(vec2 x);
float length(vec3 x);
float length(vec4 x);


//------------- distance -----------
//-- 计算 两个 向量 之间的 距离
//-- 此函数 内部 调用了 length ，进一步依赖 sqrt，效率不高，
//-- 建议用 dot 代替
//
//-- dot 和 sqrt系列函数的区别。 dot也是 x*x + y*y
//-- 但是 dot 没有执行 开平方。 可以看作一种 近似解法
float distance(float p0, float p1);
float distance(vec2 p0, vec2 p1);
float distance(vec3 p0, vec3 p1);
float distance(vec4 p0, vec4 p1);


//------------- normalize -----------
//-- 获得 x 的标准向量。
float normalize(float x);
vec2 normalize(vec2 x);
vec3 normalize(vec3 x);
vec4 normalize(vec4 x);


//------------- dot -----------
//-- 计算两个 向量的 点积
//  = x.x*y.x + x.y*y.y
// 两个 单位向量的 点积，就是 两向量夹角的 cos值。
float dot(float x, float y);
float dot(vec2 x, vec2 y);
float dot(vec3 x, vec3 y);
float dot(vec4 x, vec4 y);


//------------- cross -----------
//-- 计算两个 向量的 叉积。
//-- 生成的 新向量，将正交与 向量 x，y（前提是，xy不能平行）
vec3 cross(vec3 x, vec3 y);


//------------- faceforward -----------
//-- N    -- Specifies the vector to orient.
//-- I    -- Specifies the incident vector.   入射向量
//-- Nref -- Specifies the reference vector.
//
// Return a vector pointing in the same direction as another
// If dot(Nref, I) < 0 faceforward returns N, 
// otherwise it returns -N.
float faceforward(float N, float I, float Nref);
vec2 faceforward(vec2 N, vec2 I, vec2 Nref);
vec3 faceforward(vec3 N, vec3 I, vec3 Nref);
vec4 faceforward(vec4 N, vec4 I, vec4 Nref);


//------------- reflect -----------
//-- I -- specifies the incident vector. 入射向量
//-- N -- specifies the normal vector.or. 表面法向量
//
//-- 计算获得 入射向量 I 在 表面（法向量为 N）上的 反射向量
//-- N 应该为 标准向量。
float reflect(float I, float N);
vec2 reflect(vec2 I, vec2 N);
vec3 reflect(vec3 I, vec3 N);
vec4 reflect(vec4 I, vec4 N);



//------------- refract -----------
//-- I  -- specifies the incident vector. 入射向量
//-- N  -- specifies the normal vector.   表面法向量
//-- eta -- specifies the ratio of indices of refraction. 折射比例
//--
//--
float refract(float I, float N, float eta);
vec2 refract(vec2 I, vec2 N, float eta);
vec3 refract(vec3 I, vec3 N, float eta);
vec4 refract(vec4 I, vec4 N, float eta);



//------------- lessThan -----------
//-- returns a boolean vector 
//-- in which each element i is computed as x[i] < y[i].
bvec2 lessThan(vec2 x, vec2 y);
bvec3 lessThan(vec3 x, vec3 y);
bvec4 lessThan(vec4 x, vec4 y);

bvec2 lessThan(ivec2 x, ivec2 y);
bvec3 lessThan(ivec3 x, ivec3 y);
bvec4 lessThan(ivec4 x, ivec4 y);


//------------- lessThanEqual -----------
//--  returns a boolean vector 
//-- in which each element i is computed as x[i] ≤ y[i].
bvec2 lessThanEqual(vec2 x, vec2 y);
bvec3 lessThanEqual(vec3 x, vec3 y);
bvec4 lessThanEqual(vec4 x, vec4 y);

bvec2 lessThanEqual(ivec2 x, ivec2 y);
bvec3 lessThanEqual(ivec3 x, ivec3 y);
bvec4 lessThanEqual(ivec4 x, ivec4 y);


//------------- greatThan -----------
//--  returns a boolean vector 
//-- in which each element i is computed as x[i] > y[i].
bvec2 greaterThan(vec2 x, vec2 y);
bvec3 greaterThan(vec3 x, vec3 y);
bvec4 greaterThan(vec4 x, vec4 y);

bvec2 greaterThan(ivec2 x, ivec2 y);
bvec3 greaterThan(ivec3 x, ivec3 y);
bvec4 greaterThan(ivec4 x, ivec4 y);


//------------- greatThanEqual -----------
//-- returns a boolean vector 
//-- in which each element i is computed as x[i] ≥ y[i].
bvec2 greaterThanEqual(vec2 x, vec2 y);
bvec3 greaterThanEqual(vec3 x, vec3 y);
bvec4 greaterThanEqual(vec4 x, vec4 y);

bvec2 greaterThanEqual(ivec2 x, ivec2 y);
bvec3 greaterThanEqual(ivec3 x, ivec3 y);
bvec4 greaterThanEqual(ivec4 x, ivec4 y);


//------------- equal -----------
//-- returns a boolean vector 
//-- in which each element i is computed as x[i] == y[i].
bvec2 equal(vec2 x, vec2 y);
bvec3 equal(vec3 x, vec3 y);
bvec4 equal(vec4 x, vec4 y);

bvec2 equal(ivec2 x, ivec2 y);
bvec3 equal(ivec3 x, ivec3 y);
bvec4 equal(ivec4 x, ivec4 y);


//------------- notEqual -----------
//-- returns a boolean vector 
//-- in which each element i is computed as x[i] != y[i].
bvec2 notEqual(vec2 x, vec2 y);
bvec3 notEqual(vec3 x, vec3 y);
bvec4 notEqual(vec4 x, vec4 y);

bvec2 notEqual(ivec2 x, ivec2 y);
bvec3 notEqual(ivec3 x, ivec3 y);
bvec4 notEqual(ivec4 x, ivec4 y);


//------------- all -----------
//-- returns true if all elements of x are true
//-- and false otherwise
bool all(bvec2 x);
bool all(bvec3 x);
bool all(bvec4 x);

//------------- any -----------
//-- returns true if any element of x is true 
//-- and false otherwise
bool any(bvec2 x);
bool any(bvec3 x);
bool any(bvec4 x);


//------------- not -----------
//-- not 会引发编译器错误，暂用 not_ 代替
//-- returns a new boolean vector for which 
//-- each element i is computed as !x[i].
bvec2 not_(bvec2 x);
bvec3 not_(bvec3 x);
bvec4 not_(bvec4 x);









































