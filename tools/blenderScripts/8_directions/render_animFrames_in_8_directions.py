#
#  render animAction frames in 8-directions 
#  before run, remember set target-obj.Rotation.z == 0 !!!
#
import bpy
import os
from random import randint


#-------------------- out path ---------------------------
pathDir_out = "C:\\Users\\Administrator\\Desktop\\OUT\\"
pathDirs= [ "1_Left", "2_LeftTop", "3_Top", "4_RightTop", "5_Right", "6_RightBottom", "7_Bottom", "8_Left_Bottom" ]
paths = []

for dir in pathDirs:
    path = pathDir_out + dir + "\\"
    paths.append( path )



#--------- select target obj ----------
ob = bpy.context.scene.objects["rootCir"]        # get the obj: rootCir
bpy.ops.object.select_all(action='DESELECT') # Deselect all objects
bpy.context.view_layer.objects.active = ob   # Make the cube the active object 
ob.select_set(True)                          # Select the cube



 
#--------- render and rotate ----------
for i in paths:
    bpy.context.scene.render.filepath = i
    bpy.ops.render.render( animation=True, write_still=True ) # render anim frames and save
    bpy.ops.transform.rotate( value=0.785, orient_axis='Z' )  # rotate 45 degrees


