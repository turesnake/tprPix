/*
 * ========================= timeLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "timeLog.h"

//------------------- C --------------------//
#include <cmath>

//------------------- CPP --------------------//
#include <utility>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>

//------------------- Libs --------------------//
#include "tprGeneral.h"


//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"
#include "tprCast.h"

using std::cout;
using std::endl;


class TimeFrameData{
public:
    TimeFrameData( size_t idx_, double deltaTime_, size_t frame_ ):
        idx(idx_),
        deltaTime(deltaTime_),
        frame(frame_)
        {}
    //===== vals =====//
    size_t idx;
    double deltaTime;
    size_t frame;
};


namespace timeLog_inn {//------------ namespace: timeLog_inn --------------//

    size_t   count {};
    std::stringstream ss;

    std::vector<TimeFrameData> deltaTimes {};

    
}//---------------- namespace: timeLog_inn end --------------//

void init_timeLog(){
    timeLog_inn::count = 0;
    timeLog_inn::ss.precision(12);
    timeLog_inn::deltaTimes.reserve(100000); //- support 20 mins
}

void collect_deltaTime(double deltaTime_){

    size_t frame =  cast_2_size_t( floor(1.0 / deltaTime_) );

    TimeFrameData tf { timeLog_inn::count,deltaTime_,frame };
    timeLog_inn::deltaTimes.push_back( tf );//- copy

    //---
    timeLog_inn::ss << "  " << timeLog_inn::count 
                    << ": " << deltaTime_ 
                    << "; frame: " << frame
                    << "\n";
    timeLog_inn::count++;
}


/* ===========================================================
 *              process_and_echo_timeLog
 * -----------------------------------------------------------
 * 
 */
void process_and_echo_timeLog(){


    //--------------------------//
    //         process
    //--------------------------//
    double minFrame {200.0};
    double maxFrame {0.0};
    size_t minIdx {};
    size_t maxIdx {};

    for( auto &tf : timeLog_inn::deltaTimes ){

        if( tf.frame > maxFrame ){
            maxFrame = tf.frame;
            maxIdx = tf.idx;
        }
        if( tf.frame < minFrame ){
            minFrame = tf.frame;
            minIdx = tf.idx;
        }
    }




    //--------------------------//
    //    write to file
    //--------------------------//
    std::string path_timeLog = tprGeneral::path_combine(path_tprLog, "timeLog.txt");

    std::ofstream writeFile;
    writeFile.open( path_timeLog );
    if( writeFile ){

        writeFile.precision(12);
        writeFile << "~~~~~~~~~~~~~~~~~~~~ Kernel Info: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
                << "\nidx:" << minIdx << "; minFrame = " << minFrame
                << "\nidx:" << maxIdx << "; maxFrame = " << maxFrame
                << "\n\n"
                << "~~~~~~~~~~~~~~~~~~~~ timeLogData: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
                << timeLog_inn::ss.str()
                << "~~~~~~~~~~~~~~~~~~~~ timeLogData: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                
                << endl;        

    }


}


