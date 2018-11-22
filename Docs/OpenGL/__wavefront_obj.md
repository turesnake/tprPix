
# ================================================================//
#                     wavefront obj
# ================================================================//
一种 较为简单的 3D模型 文件格式。


#--  v 0.123 0.234 0.345 1.0
    geometric vertices
    with (x, y, z [,w]) coordinates, 
    w is optional and defaults to 1.0.





#-- vt 0.500 1 [0]
    texture coordinates
    in (u, v [,w]) coordinates, 
    these will vary between 0 and 1, 
    w is optional and defaults to 0.


#-- vn 0.707 0.000 0.707
    vertex normals 
    in (x,y,z) form; 
    normals might not be unit vectors.


#-- vp 0.310000 3.210000 2.100000
    Parameter space vertices 
    in ( u [,v] [,w] ) form; 
    free form geometry statement ( see below )


#-- f 1 2 3
#-- f 3/1 4/2 5/3
#-- f 6/4/1 3/5/3 7/6/5
#-- f 7//1 8//2 9//3
#-- f ...
    Polygonal face element 


#-- l 5 8 1 2 4 9
    Line element 

















