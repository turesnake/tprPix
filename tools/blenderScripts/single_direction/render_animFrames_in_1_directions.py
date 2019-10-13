#
#  render animAction frames in single-directions 
#  before run, remember set target-obj.Rotation.z == 0 !!!
#  ---
#  run: alt+p
# 
#
import bpy
import os
from random import randint


#-------------------- out path ---------------------------
pathDir_osx = "/Users/zouhanjie/Desktop/blenderOut/"
pathDir_win = "C:/Users/Administrator/Desktop/OUT/Sml/"
fileName = "koko"

#--------- select target obj ----------
ob = bpy.context.scene.objects["rootCir"]    # get the obj: rootCir
bpy.ops.object.select_all(action='DESELECT') # Deselect all objects
bpy.context.view_layer.objects.active = ob   # Make the cube the active object 
ob.select_set(True)                          # Select the cube


#--------- render and rotate ----------
frameNums = 12
step = 3.1415926 / frameNums

i=0
while i < frameNums:
    i = i + 1
    path = pathDir_osx + fileName + "_" + str(i)
    bpy.context.scene.render.filepath = path
    bpy.ops.render.render( animation=False, write_still=True ) # render anim frames and save
    bpy.ops.transform.rotate( value=0.785, orient_axis='Z' )  # rotate 45 degrees



