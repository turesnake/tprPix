/*
 * ========================= simplexNoise.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   glsl版 simplex-noise, 移植到 cpu上
 * ----------------------------
 */
#include "simplexNoise.h"

//-------------------- C ----------------------//
//#include <cassert>
#include <cmath>


/* ===========================================================
 *                prepare funcs...
 * -----------------------------------------------------------
 */
glm::dvec2 mod289V2( const glm::dvec2 &_vec ){


    glm::dvec2 tmpv {floor(_vec.x / 289.0),
                     floor(_vec.y / 289.0) };
    tmpv *= 289.0;
    return _vec - tmpv; 
}

glm::dvec3 mod289V3( const glm::dvec3 &_vec){ 
    glm::dvec3 tmpv {floor(_vec.x / 289.0),
                     floor(_vec.y / 289.0),
                     floor(_vec.z / 289.0) };
    tmpv *= 289.0;
    return _vec - tmpv;
}

glm::dvec4 mod289V4( const glm::dvec4 &_vec){ 
    glm::dvec4 tmpv {floor(_vec.x / 289.0),
                     floor(_vec.y / 289.0),
                     floor(_vec.z / 289.0),
                     floor(_vec.w / 289.0) };
    tmpv *= 289.0;
    return _vec - tmpv;
}


glm::dvec3 permuteV3( const glm::dvec3 &_vec ){ 
    return mod289V3( glm::dvec3{ ((_vec.x*34.0) + 1.0) * _vec.x,
                                 ((_vec.y*34.0) + 1.0) * _vec.y,
                                 ((_vec.z*34.0) + 1.0) * _vec.z } );
}

glm::dvec4 permuteV4( const glm::dvec4 &_vec){
    return mod289V4( glm::dvec4{ ((_vec.x*34.0) + 1.0) * _vec.x,
                                 ((_vec.y*34.0) + 1.0) * _vec.y,
                                 ((_vec.z*34.0) + 1.0) * _vec.z,
                                 ((_vec.w*34.0) + 1.0) * _vec.w } );
}

glm::dvec4 taylorInvSqrtV4( const glm::dvec4 &_vec ){ 
    return glm::dvec4{   1.79284291400159 - 0.85373472095314 * _vec.x,
                         1.79284291400159 - 0.85373472095314 * _vec.y,
                         1.79284291400159 - 0.85373472095314 * _vec.z,
                         1.79284291400159 - 0.85373472095314 * _vec.w };
}

double dotV2( double _ax, double _ay, double _bx, double _by ){
    return (_ax*_bx + _ay*_by);
}



/* ===========================================================
 *                   simplex_noise2
 * -----------------------------------------------------------
 *  和 glsl 中 同名函数，内容完全一致 
 *  return 【-1.0， 1.0】
 */
float simplex_noise2( float _x, float _y ){
    return simplex_noise2( glm::vec2{_x, _y} );
}
float simplex_noise2( const glm::vec2 &_v ){

    const glm::dvec2 v {    static_cast<double>(_v.x),
                            static_cast<double>(_v.y) };

    const glm::dvec4 C {0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,   // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,   // -1.0 + 2.0 * C.x
                        0.024390243902439 }; // 1.0 / 41.0
                
    // First corner
    glm::dvec2 i  = glm::floor( v + dotV2(v.x, v.y, C.y, C.y) );
    glm::dvec2 x0 = v -         i + dotV2(i.x, i.y, C.x, C.x);
    // Other corners
    glm::dvec2 i1;
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

    glm::dvec3 m = glm::max( glm::dvec3{0.5 - dotV2(x0.x, x0.y, x0.x, x0.y), 
                                        0.5 - dotV2(x12.x, x12.y, x12.x, x12.y), 
                                        0.5 - dotV2(x12.z, x12.w, x12.z, x12.w) }, 
                            glm::dvec3{ 0.0, 0.0, 0.0 } );
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
    glm::dvec3 g;
    g.x = a0.x  * x0.x + h.x  * x0.y;
    g.y = a0.y * x12.x + h.y * x12.y;
    g.z = a0.z * x12.z + h.z * x12.w;
    return static_cast<float>( 130.0f * glm::dot(m, g) );
}


