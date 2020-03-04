/*
 * ========================= History.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_HISTORY_H
#define TPR_HISTORY_H


// 如果 某个变量 需要暂存旧状态，使用此装置来管理
template< typename T >
class History{
public:
    explicit History( const T &val_ ):
        oldVal(val_),
        newVal(val_),
        isDirty(false)
        {}

    inline void reset( const T &val_ )noexcept{
        this->oldVal = val_;
        this->newVal = val_;
        this->isDirty = false;
    }


    // 更新 new 值，和 flag
    // 若 T 没有 != 运算符，编译阶段会直接报错 
    inline void set_newVal( const T &newVal_ )noexcept{  
        if( this->newVal != newVal_ ){
            this->newVal = newVal_; 
            this->isDirty = true;
        }   
    }

    inline const T &get_oldVal()const noexcept{ return this->oldVal; }
    inline const T &get_newVal()const noexcept{ return this->newVal; }

    // 每一渲染帧 起始处调用，检测 上一帧的操作 是否被同步
    // 如果未同步，说明流程不干净，外部应该直接报错
    inline bool     get_isDirty()const noexcept{ return this->isDirty; }

    // 同步数据，清空 old 槽
    inline void sync()noexcept{
        if( this->isDirty ){
            this->isDirty = false;
            this->oldVal = this->newVal;
        }
    }

private:
    T oldVal; // last
    T newVal; // current
    bool isDirty;
};


#endif 

