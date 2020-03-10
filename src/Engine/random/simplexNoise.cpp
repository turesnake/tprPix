/*
 * ========================= simplexNoise.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   glsl版 simplex-noise, 移植到 cpu上
 * ----------------------------
 */
#include "pch.h"
#include "simplexNoise.h"


/* ===========================================================
 *                prepare funcs...
 * -----------------------------------------------------------
 */
glm::dvec2 mod289V2( const glm::dvec2 &vec_ ){


    glm::dvec2 tmpv {floor(vec_.x / 289.0),
                     floor(vec_.y / 289.0) };
    tmpv *= 289.0;
    return vec_ - tmpv; 
}

glm::dvec3 mod289V3( const glm::dvec3 &vec_){ 
    glm::dvec3 tmpv {floor(vec_.x / 289.0),
                     floor(vec_.y / 289.0),
                     floor(vec_.z / 289.0) };
    tmpv *= 289.0;
    return vec_ - tmpv;
}

glm::dvec4 mod289V4( const glm::dvec4 &vec_){ 
    glm::dvec4 tmpv {floor(vec_.x / 289.0),
                     floor(vec_.y / 289.0),
                     floor(vec_.z / 289.0),
                     floor(vec_.w / 289.0) };
    tmpv *= 289.0;
    return vec_ - tmpv;
}


glm::dvec3 permuteV3( const glm::dvec3 &vec_ ){ 
    return mod289V3( glm::dvec3{ ((vec_.x*34.0) + 1.0) * vec_.x,
                                 ((vec_.y*34.0) + 1.0) * vec_.y,
                                 ((vec_.z*34.0) + 1.0) * vec_.z } );
}

glm::dvec4 permuteV4( const glm::dvec4 &vec_){
    return mod289V4( glm::dvec4{ ((vec_.x*34.0) + 1.0) * vec_.x,
                                 ((vec_.y*34.0) + 1.0) * vec_.y,
                                 ((vec_.z*34.0) + 1.0) * vec_.z,
                                 ((vec_.w*34.0) + 1.0) * vec_.w } );
}

glm::dvec4 taylorInvSqrtV4( const glm::dvec4 &vec_ ){ 
    return glm::dvec4{   1.79284291400159 - 0.85373472095314 * vec_.x,
                         1.79284291400159 - 0.85373472095314 * vec_.y,
                         1.79284291400159 - 0.85373472095314 * vec_.z,
                         1.79284291400159 - 0.85373472095314 * vec_.w };
}

double dotV2( double ax_, double ay_, double bx_, double by_ ){
    return (ax_*bx_ + ay_*by_);
}


// to mask the "max" macro in minwindef.h
//#undef max



/* ===========================================================
 *                   simplex_noise2
 * -----------------------------------------------------------
 *  和 glsl 中 同名函数，内容完全一致 
 *  return [-1.0， 1.0]
 */
double simplex_noise2( double x_, double y_ ){
    return simplex_noise2( glm::dvec2{x_, y_} );
}
double simplex_noise2( const glm::dvec2 &v_ ){

    constexpr glm::dvec4 C {0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                            0.366025403784439,   // 0.5*(sqrt(3.0)-1.0)
                            -0.577350269189626,   // -1.0 + 2.0 * C.x
                            0.024390243902439 }; // 1.0 / 41.0
                
    // First corner
    glm::dvec2 i  = glm::floor( v_ + dotV2(v_.x, v_.y, C.y, C.y) );
    glm::dvec2 x0 = v_ - i + dotV2(i.x, i.y, C.x, C.x);
    // Other corners
    glm::dvec2 i1 {};
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    (x0.x > x0.y) ? 
        i1 = glm::dvec2(1.0, 0.0) : 
        i1 = glm::dvec2(0.0, 1.0);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    glm::dvec4 x12 {x0.x + C.x - i1.x,
                    x0.y + C.x - i1.y,
                    x0.x + C.z,
                    x0.y + C.z };
    // Permutations
    i = mod289V2(i); // Avoid truncation effects in permutation
    glm::dvec3 p = permuteV3( glm::dvec3{   i.y + 0.0, 
                                            i.y + i1.y, 
                                            i.y + 1.0 } );
    p = permuteV3( glm::dvec3{  p.x + i.x + 0.0,
                                p.y + i.x + i1.x,
                                p.z + i.x + 1.0 } );


    glm::dvec3 max_1   {0.5 - dotV2(x0.x, x0.y, x0.x, x0.y), 
                        0.5 - dotV2(x12.x, x12.y, x12.x, x12.y), 
                        0.5 - dotV2(x12.z, x12.w, x12.z, x12.w) };

    constexpr glm::dvec3 max_2 { 0.0, 0.0, 0.0 };

    // wrap the function in parenthesis
    // to mask the "max" macro in minwindef.h
    glm::dvec3 m = (glm::max)( max_1, max_2 );


    m *= m;
    m *= m;
    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
    glm::dvec3 x = glm::fract( glm::dvec3{  p.x * C.w,
                                            p.y * C.w,
                                            p.z * C.w } );
    x.x = 2.0 * x.x - 1.0;
    x.y = 2.0 * x.y - 1.0;
    x.z = 2.0 * x.z - 1.0;

    glm::dvec3 h {  std::abs(x.x) -0.5,
                    std::abs(x.y) -0.5,
                    std::abs(x.z) -0.5 };

    glm::dvec3 ox { floor( x.x + 0.5 ),
                    floor( x.y + 0.5 ),
                    floor( x.z + 0.5 ) };
    glm::dvec3 a0 = x - ox;
    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    glm::dvec3 tmpV = a0*a0 + h*h;
    m.x *= 1.79284291400159 - 0.85373472095314 * tmpV.x;
    m.y *= 1.79284291400159 - 0.85373472095314 * tmpV.y;
    m.z *= 1.79284291400159 - 0.85373472095314 * tmpV.z;
    // Compute final noise value at P
    glm::dvec3 g {};
    g.x = a0.x  * x0.x + h.x  * x0.y;
    g.y = a0.y * x12.x + h.y * x12.y;
    g.z = a0.z * x12.z + h.z * x12.w;
    return static_cast<double>( 130.0 * glm::dot(m, g) );
}


