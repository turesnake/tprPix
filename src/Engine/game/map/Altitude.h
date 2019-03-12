/*
 * ====================== Altitude.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   每个 mapEnt／field 都有自身的 高度信息。
 *  -----
 *  这个模块，还在改进中
 * ----------------------------
 */
#ifndef _TPR_ALTITUDE_H_
#define _TPR_ALTITUDE_H_

//-------------------- C --------------------//
#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h" 

using alti_t = u8_t;


inline void assert_alti( alti_t _alti ){
    assert( _alti<= 15 );
}


/*
class Altitude{
public:
    explicit Altitude( alti_t  _alti ):
        alti(_alti)
        {
        assert( _alti<= 15 );
        }
    inline alti_t get_alti() const {
        return alti;
    }
private:
    alti_t  alti; //- [0,15]
};
*/



#endif 

