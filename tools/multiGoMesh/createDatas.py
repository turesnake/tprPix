#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  fieldDistributePlan Sml1
#

import os
import sys
import json
import random # [0,1)
import string

from tools import *

#------------ INPUT ------------#
#  need user input:
entNums_Sml = 2
entNums_Mid = 1
entNums_Big = 0

#------------ global Vals ------------#
PIXES_PER_MAPENT = 64
ENTS_PER_FIELD = 4

#- mesh radius:
RARDIUS_ASSIST = PIXES_PER_MAPENT * 0.2

RADIUS_SML = 64 * 1   # 64
RADIUS_MID = 64 * 1.3 # 83.8 tmp
RADIUS_BIG = 64 * 1.5 # 96   tmp


windowSZ = PIXES_PER_MAPENT * ENTS_PER_FIELD # 256
windowRadius = windowSZ * 0.5 # 384
windowCenter = Vec2( windowRadius, windowRadius )


#--- not detect collide yet ---#
# param: entRadius_ float
# return: Cir
def apply_a_cir( entRadius_ ):
    ADJUST = 20 
    rad = windowRadius - entRadius_ + ADJUST
    dpos = apply_new_randVec2(rad) + windowCenter
    cir = Cir( dpos, entRadius_ )
    return cir
    #------------------#


# param: radius_   float
# param: nums_     int
# param: cirs_     Cir[] container
def apply_cirs( radius_, nums_, cirs_ ):
    for i in range(nums_) :
        #- detect:
        while True:
            cir = apply_a_cir( radius_ )
            if( detect_is_collide(cir, cirs) == False ):
                break
        cirs_.append( cir )
    #------------------#


# param: cirs_     Cir[] container
def append_assist_cir( cirs_ ):
    cir = Cir( windowCenter, RARDIUS_ASSIST )
    cirs_.append( cir )
    #------------------#


# param: radius_   float
# return: str
def radiusVal_2_str( radius_ ):
    if( is_closeEnough(radius_, RADIUS_SML, 0.1) ):
        return "Sml"
    elif( is_closeEnough(radius_, RADIUS_MID, 0.1) ):
        return "Mid"
    elif( is_closeEnough(radius_, RADIUS_BIG, 0.1) ):
        return "Big"
    else:
        assert False
    #------------------#




# param: filePath_  str
# param: buf_       str
def write_to_file( filePath_, buf_ ):
    f = open( filePath_, "w+"  )
    f.write( buf_ )
    f.close()
    #------------------#

if __name__=="__main__":
    print("======== create random dpos  ===========")

    #----- path -----#
    path_root = os.path.abspath('.') # current dir path "/xxx/xx"
    path_out   = os.path.join( path_root, "data" ) # out dir

    #----- apply and detect -----#
    cirs = []

    append_assist_cir( cirs ) # perpare a assist-cir

    apply_cirs( RADIUS_BIG, entNums_Big, cirs )
    apply_cirs( RADIUS_MID, entNums_Mid, cirs )
    apply_cirs( RADIUS_SML, entNums_Sml, cirs )
    
    cirs.pop(0) # erase the assist-cir


    
    #----- dump to JSON data -----#
    jsonData = []
    for cir in cirs:
        dpos = []

        x = cir.dpos.x - windowRadius
        y = cir.dpos.y - windowRadius
        
        dpos.append( int(x) )
        dpos.append( int(y) )

        dic = { "tmp" : 1 } # tmp val
        dic["dpos"] = dpos
        dic["BodySize"] = radiusVal_2_str( cir.radius )
        del dic["tmp"] # no need 
        jsonData.append( dic )
    jsonDataBuf = json.dumps(jsonData)

    #--- write ---#
    path_outFile = os.path.join( path_out, "out__.json" ) # out
    write_to_file( path_outFile, jsonDataBuf )

