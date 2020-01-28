/*
 * ========================= VAOVBO.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   used for most of meshs/goMeshs
 *   excep groundGo meshs
 * ----------------------------
 */
#ifndef TPR_VAO_VBO_H
#define TPR_VAO_VBO_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h>  


class VAOVBO{
public:
    VAOVBO()=default;

    ~VAOVBO(){
        glDeleteVertexArrays( 1, &this->VAO );
        glDeleteBuffers(      1, &this->VBO );
    }

    void init(const GLvoid *pointsPtr_, GLsizeiptr  vboSize_ )noexcept;

    // can be copy and hold by caller
    inline GLuint get_VAO()const noexcept{ return this->VAO; }

private:
    GLuint VAO  {}; //- obj id
    GLuint VBO  {}; //- obj id
};


#endif 

