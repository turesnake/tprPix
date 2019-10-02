#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  meshs_in_2m2_field
#

import os
import sys
import json
import random # [0,1)

from tools import *

#------------ INPUT ------------#
#  need user input:
entNums_1 = 0
entNums_2 = 4
entNums_3 = 2
entNums_4 = 0

#------------ global Vals ------------#
PIXES_PER_MAPENT = 64
ENTS_PER_FIELD = 4
FIELDS_PER_CHUNK = 8

FIELDS = 2

#- mesh radius:
RADIUS_1 = (64 * 1) * 0.5 # 32
RADIUS_2 = (64 * 2) * 0.5 # 64
RADIUS_3 = (64 * 3) * 0.5 # 96
RADIUS_4 = (64 * 4) * 0.5 # 128

windowSZ = PIXES_PER_MAPENT * ENTS_PER_FIELD * FIELDS # 512
windowRadius = windowSZ * 0.5 # 384
windowCenter = Vec2( windowRadius, windowRadius )


#--- not detect collide yet ---#
# param: entRadius_ float
# return: Cir
def apply_a_cir( entRadius_ ):
    rad = windowRadius - entRadius_
    dpos = apply_new_randVec2(rad) + windowCenter
    cir = Cir( dpos, entRadius_ )
    return cir
    #------------------#


# param: radius_   float
# param: nums_     int
# param: cirs_     Cir[]
def apply_cirs( radius_, nums_, cirs_ ):

    for i in range(nums_) :
        #- detect:
        while True:
            cir = apply_a_cir( radius_ )
            if( detect_is_collide(cir, cirs) == False ):
                break
        cirs_.append( cir )
    #------------------#

# param: radius_   float
# return: str
def radiusVal_2_str( radius_ ):
    if( is_closeEnough(radius_, RADIUS_1, 0.1) ):
        return "1m1"
    elif( is_closeEnough(radius_, RADIUS_2, 0.1) ):
        return "2m2"
    elif( is_closeEnough(radius_, RADIUS_3, 0.1) ):
        return "3m3"
    elif( is_closeEnough(radius_, RADIUS_4, 0.1) ):
        return "4m4"
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
    print("======== create random dpos for ents ===========")

    #----- path -----#
    path_root = os.path.abspath('.') # current dir path "/xxx/xx"
    path_out   = os.path.join( path_root, "data" ) # out dir


    #----- apply and detect -----#
    cirs = []
    apply_cirs( RADIUS_4, entNums_4, cirs )
    apply_cirs( RADIUS_3, entNums_3, cirs )
    apply_cirs( RADIUS_2, entNums_2, cirs )
    apply_cirs( RADIUS_1, entNums_1, cirs )

    
    #----- dump to JSON data -----#
    jsonData = []
    for cir in cirs:
        dpos = []

        x = cir.dpos.x - windowRadius
        y = cir.dpos.y - windowRadius
        #dpos.append( '%.1f' % x )
        #dpos.append( '%.1f' % y )
        dpos.append( float('%.1f' % x) )
        dpos.append( float('%.1f' % y) )

        dic = { "tmp" : 1 } # tmp val
        dic["dpos"] = dpos
        dic["meshLvl"] = radiusVal_2_str( cir.radius )
        del dic["tmp"] # no need 
        jsonData.append( dic )
    jsonDataBuf = json.dumps(jsonData)

    #--- write ---#
    path_outFile = os.path.join( path_out, "randEnt.json" ) # out
    write_to_file( path_outFile, jsonDataBuf )

