#
#  render animAction frames in 8-directions 
#  before run, remember set target-obj.Rotation.z == 0 !!!
#
import bpy
import os
import sys
from random import randint

sys.path.append("C:/Users/Administrator/Desktop/blender_script/tree_create/")
from tools import *

#-----


class DirData(object):
    def __init__(self, dir_rotation_degree_z_, light_rotation_degree_x_ ):
        self.dir_rotationRad_z = math.radians( dir_rotation_degree_z_ )
        self.light_rotationRad_x = math.radians( light_rotation_degree_x_ )
        #-----



#-------------------- out path ---------------------------
pathDir_out = "C:\\Users\\Administrator\\Desktop\\OUT2\\"
pathDirs= [ "1_Left", "2_LeftTop", "3_Top", "4_RightTop", "5_Right", "6_RightBottom", "7_Bottom", "8_LeftBottom" ]
paths = []

for dir in pathDirs:
    path = pathDir_out + dir + "\\"
    paths.append( path )


dirdatas = []
dirdatas.append( DirData( 285, 25 ) ) # 1 left
dirdatas.append( DirData( 230, 10 ) ) # 2 leftTop
dirdatas.append( DirData( 195, 25 ) ) # 3 top
dirdatas.append( DirData( 145, 10 ) ) # 4 rightTop
dirdatas.append( DirData( 105, 25 ) ) # 5 right
dirdatas.append( DirData( 50,  10 ) ) # 6 rightBottom 
dirdatas.append( DirData( 15,  25 ) ) # 7 bottom
dirdatas.append( DirData( 320, 10 ) ) # 8 leftBottom



#----- check -----#
assert len(paths) == 8
assert len(dirdatas) == 8


#--------- select target obj ----------
rootCir = get_obj( "rootCir" )
rightLight = get_obj( "Sun.right" )

 
#--------- render and rotate ----------

for idx in range(8):
    
    #----- set obj first -----#
    select_and_active_obj( rootCir )
    rootCir.rotation_euler[2] = dirdatas[idx].dir_rotationRad_z # z

    select_and_active_obj( rightLight )
    rightLight.rotation_euler[0] = dirdatas[idx].light_rotationRad_x # x
    rightLight.rotation_euler[1] = math.radians( 90 )
    rightLight.rotation_euler[2] = math.radians( 0 )


    #----- render -----#
    bpy.context.scene.render.filepath = paths[idx]
    bpy.ops.render.render( animation=True, write_still=True ) # render anim frames and save







