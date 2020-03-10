/*
 * ====================== Coordinate.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  self defined coordinate system
 * ------------
 */
#ifndef TPR_COORD_H
#define TPR_COORD_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "NineDirection.h"


class Coordinate{
public:
    Coordinate( const glm::dvec2 &xVec_, const glm::dvec2 &yVec_ ):
        xVec(xVec_),
        yVec(yVec_),
        denominator( xVec.x * yVec.y - xVec.y * yVec.x )
        {
            this->init();
        }

    //-- 将外部世界坐标系的值，转换成 客制坐标系内的 对应值
    inline glm::dvec2 calc_innDPos( const glm::dvec2 &outDPos_ )const noexcept{
        return glm::dvec2{
            (this->yVec.y * outDPos_.x - this->yVec.x * outDPos_.y) / this->denominator,
            (this->xVec.x * outDPos_.y - this->xVec.y * outDPos_.x) / this->denominator
        };
    }

    //-- 将客制坐标系内的 值，转换成 外部世界坐标的 对应值
    inline glm::dvec2 calc_outDPos( const glm::dvec2 &innDPos_ )const noexcept{
        return glm::dvec2{  this->xVec.x * innDPos_.x + this->yVec.x * innDPos_.y,
                            this->xVec.y * innDPos_.x + this->yVec.y * innDPos_.y };
    }

    inline const glm::dvec2 &get_normalVec_in_outCoord( NineDirection dir_ )const noexcept{
        tprAssert( this->normalVecs_in_outCoord.find(dir_) != this->normalVecs_in_outCoord.end() );
        return this->normalVecs_in_outCoord.at(dir_);
    }

    //----- get -----//
    inline const glm::dvec2 &get_rightHand()const noexcept{ return this->rightHand; }
    inline const glm::dvec2 &get_xVec()const noexcept{ return this->xVec; }
    inline const glm::dvec2 &get_yVec()const noexcept{ return this->yVec; }
    inline double            get_denominator()const noexcept{ return this->denominator; }

private:
    void init()noexcept;
    //----- vals -----//
    glm::dvec2  xVec;
    glm::dvec2  yVec;
    double      denominator; // in order to make the calc easy

    glm::dvec2 rightHand {}; // 本坐标系 x方向 单位向量，在 outCoord 中的值
                             // 目前用于，修正 input 方向健输入

    //-- 每个坐标系内部，都存在一个对齐于原点的 正方形。
    //  将这个正方形扭曲，变成其在 外部坐标系的样子，并记录其4条边，在外部坐标系的 对外法线向量
    std::unordered_map<NineDirection, glm::dvec2> normalVecs_in_outCoord {};
};

#endif

