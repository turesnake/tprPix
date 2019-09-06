# =======================================================
#                       glm_API.h 
# -------------------------------------------------------
#   glm  一个专门为 OpenGL 服务的 数学／向量库
#   glm  的一个目标，就是模仿 GLSL 中的函数
#   所以，如果 GLSL 拥有某个函数，那么，glm 多半也拥有 一个同名函数
# ----------------------------


#  float glm::radians( float _angle );
   输入一个 角度值
   转换为一个 弧度值，并返回之


#  glm::vec2 glm::normalize( glm::vec2 _v );
#  glm::vec3 glm::normalize( glm::vec3 _v );
#  glm::vec4 glm::normalize( glm::vec4 _v );
    将一个 向量 标准化。


# float glm::dot( glm::vec2 _v1, glm::vec2 _v2 );
    计算 两个 向量 的 点积 （通常对两个 单位向量使用）
    获得 这两个 单位向量 的 夹角的 cos 值。
    由于：
        cos(0) = 1
        cos(90) = o
    所以，常用此函数，来判断两个向量 是否 垂直／平行


#  glm::vec3 glm::cross( glm::vec3 _v, glm::vec3 _v );
    对两个 v3 向量进行叉乘，获得 垂直于它们的 一个 v3 向量


#  float glm::smoothstep( float _a, float _b, float _x );
    常规用法：
        _a  传入  左侧边界（x轴上的一个值）
        _b  传入  右侧边界（x轴上的一个值）
        _x  传入  x变量

    函数返回一个 y值。 实现一个 曲线，从 _a, 到 _b， 平滑过度。


#  float glm::fract( float _f );
    取 一个 float数 的小数部分

#  float glm::distance( const glm::vec2 &_v1, const glm::vec2 &_v2 );
    计算两个 向量的 距离，很方便
    -----
    真的是 2个向量的 距离吗？？？
    有待测试


#  double glm::length( const glm::dvec2 &v_ );
    计算一个向量 的模（长度）（勾股定理）
