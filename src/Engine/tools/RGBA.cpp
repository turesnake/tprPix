/*
 * ========================= RGBA.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "RGBA.h"

//------------------- CPP --------------------//
#include <sstream> //- stringstream


std::string RGBA::to_string()const noexcept{

    std::stringstream ss;

    int ir = static_cast<int>(this->r);
    int ig = static_cast<int>(this->g);
    int ib = static_cast<int>(this->b);
    int ia = static_cast<int>(this->a);

    ss << "RGBA{ " << ir 
        << ", " << ig
        << ", " << ib
        << ", " << ia 
        << " } ";

    return ss.str();
}






