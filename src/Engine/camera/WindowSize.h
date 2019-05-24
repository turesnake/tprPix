/*
 * ====================== WindowSize.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 *       已废弃 ..........
 * 
 */
#ifndef _TPR_WINDOW_SIZE_H_
#define _TPR_WINDOW_SIZE_H_


//-------------------- Engine --------------------//
#include "IntVec.h"



class WindowSize{
public:
    WindowSize( const int _screenX, 
                const int _screenY,
                const int _pixed_per_gamePix ):
        srceenSZ( _screenX, _screenY )
        {
        //this->gameSZ = this->srceenSZ 
        }

    /*
    void reset( const int _x, const int _y ){
        this->x = _x;
        this->y = _y;
    }
    */



    inline const IntVec2 &get_screenSZ() const {
        return this->srceenSZ;
    }
    inline const IntVec2 &get_gameSZ() const {
        return this->gameSZ;
    }


private:
    //----- vals -----//
    IntVec2   srceenSZ; //-
    IntVec2   gameSZ;   //- 
};






#endif 

