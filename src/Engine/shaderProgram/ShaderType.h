/*
 * ========================= ShaderType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SHADER_TYPE_H
#define TPR_SHADER_TYPE_H

#include <string>

enum class ShaderType{

    Shadow,
    //---
    Ground,
    BioSoup, // 异世界生物汤，会变色，暂未实现 ...

    //---
    OriginColor,
    UnifiedColor,
    GroundColor,
    //---
    PlayerGoCircle,
    Floor
    //-- gos --
    
};


std::string shaderType_2_str( ShaderType type_ )noexcept;
ShaderType str_2_shaderType( const std::string &str_ )noexcept;





#endif 

