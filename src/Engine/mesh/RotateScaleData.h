/*
 * ================== RotateScaleData.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHILD_MESH_ROTATE_DATA_H
#define TPR_CHILD_MESH_ROTATE_DATA_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <utility> // move


//-------------------- Engine --------------------//

//- 暂时只支持 xyz 三个轴线的旋转
enum class RotateType{
    X,
    Y,
    Z
};

// 外部向 GoMesh 传入的 rotate/scale 数据
class RotateScaleData{
public:
    RotateScaleData() = default;

    //--- set ---//
    inline void reset_rotateOrder( std::vector<RotateType> order_ )noexcept{
        this->rotateOrder.swap( order_ );
    }
    inline void set_rotateDegree( const glm::vec3 &rad_ )noexcept{
        this->rotateDegree = rad_;
    }
    inline void set_scale( const glm::vec2 &scale_ )noexcept{
        this->scale = scale_;
    }

    //--- get ---//
    inline const glm::vec3 &get_rotateDegreeRef() const noexcept{
        return this->rotateDegree;
    }
    inline const glm::vec2 &get_scaleRef() const noexcept{
        return this->scale;
    }
    inline bool isNeedToRotate() const noexcept{
        return !rotateOrder.empty();
    }
    inline const std::vector<RotateType> &get_rotateOrder() const noexcept{
        return this->rotateOrder;
    }

private:
    //- 一个不是很完善的解决方案：
    //  依次向 rotateOrder 压入 x／y／z，每一种只能压入一次，次序不限
    //  实际旋转时，按照 这个 vector 中的次序依次旋转
    //  ---
    //  如果 rotateOrder 为空，就说明不需要旋转
    std::vector<RotateType> rotateOrder {};
    glm::vec3 rotateDegree {glm::vec3(0.0f, 0.0f, 0.0f)}; //- Degree [0.0f, 360.0f]
    glm::vec2 scale        {glm::vec2(1.0f, 1.0f)};       // 仅仅是 外部希望的 缩放比例
                                                          // 不是最终算进 mat4 的值
};


#endif 

