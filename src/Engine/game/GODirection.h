/*
 * ========================= GODirection.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   go direction
 * ----------------------------
 */
#ifndef TPR_DIRECTION_H
#define TPR_DIRECTION_H


//-------------------- Engine --------------------//
#include "NineDirection.h"
#include "DirAxes.h"



class GODirection{
public:



    inline void calc_newDir( const DirAxes &dieAxes_ ){
        this->newDir = dirAxes_2_nineDirection(dieAxes_);


        
    }





private:
    
    NineDirection   oldDir {NineDirection::Mid};
    NineDirection   newDir {NineDirection::Mid};




};


#endif 

