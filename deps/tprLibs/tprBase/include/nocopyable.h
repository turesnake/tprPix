/*
 * ========================= nocopyable.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.19
 *                                        修改 -- 2018.11.19
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#ifndef _TPR_NOCOPYABLE_H_
#define _TPR_NOCOPYABLE_H_

namespace tpr{ //------------- namespace: tpr ----------------//


class nocopyable{
public:
    nocopyable( const nocopyable& ) = delete;
    void operator=( const nocopyable ) = delete;

protected:
    nocopyable() = default;
    ~nocopyable() = default;
};

}//------------------- namespace: tpr ------------------------//
#endif
