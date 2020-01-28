/*
 * ========================= UniformBlockObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UNIFORM_BLOCK_OBJ_H
#define TPR_UNIFORM_BLOCK_OBJ_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"

namespace ubo{//------------- namespace ubo ----------------

class UniformBlockObj{
public:
    UniformBlockObj(GLuint     bindPoint_,
                    GLsizeiptr dataSize_,
                    const std::string &uboName_ ):
        bindPoint(bindPoint_),
        dataSize(dataSize_),
        uboName(uboName_)
        {
            glGenBuffers(1, &this->ubo);
            glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
            glBufferData(GL_UNIFORM_BUFFER, this->dataSize, nullptr, GL_STATIC_DRAW); // 分配n字节的内存
            glBindBuffer(GL_UNIFORM_BUFFER, 0);//- release bind
            glBindBufferBase(GL_UNIFORM_BUFFER, this->bindPoint, this->ubo); // set bindPoint
        }

    // 与本ubo 相关的着色器 执行bind，从而可以访问这个 绑定点idx 的 ubo 数据
    inline void bind_2_shaderProgram(GLuint shaderProgram_ )const noexcept{
        unsigned int index = glGetUniformBlockIndex( shaderProgram_, this->uboName.c_str() );   
        glUniformBlockBinding( shaderProgram_, index, this->bindPoint); // set bindPoint
    }


    inline void write(  GLintptr offset_, // 首字节偏移
                        GLsizeiptr size_, // 写入尺寸
                        const GLvoid *dataPtr_ )noexcept{
        tprAssert( (offset_+size_) <= this->dataSize );
        // 现在，所有的东西都配置完毕了，可以开始向 ubo 中添加数据了
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, offset_, size_, dataPtr_ ); 
        glBindBuffer(GL_UNIFORM_BUFFER, 0);//- release bind
    }

private:
    GLuint ubo {};
    GLuint bindPoint {};
    GLsizeiptr dataSize {}; //- 也许会被拆分为 n * ent, 但 本class 并不关心
    std::string uboName {};
};


}//------------- namespace ubo: end ----------------
#endif 

