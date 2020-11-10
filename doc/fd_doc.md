# description of all functions and header files included in FlatDraw

## header list
Name                   | Short description
---------------------- | -------------------------------------------------
[binding.h](#bh)       | implements bindings
[callbacks.h](#ch)     | contains callback functions
[click_handle.h](#chh) | contains functions for handling clicks
[color.h](#clh)        | contains color schemes
[dbg.h](#dbh)          | special header that defines the DBG character
[draw.h](#dh)          | contains functions for drawing
[fd_format.h](#ffh)    | contains functions for outputting shapes to a file
[figure.h](#fh)        | includes functions for creating and processing shapes
[geometry.h](#gh)      | includes functions for creating and processing geometry
[list.h](#lh)          | implements a doubly linked list of shapes

## headers description
<a name="bh"></a>
### binding.h
this file includes functions for creating three types of bindings: object bindings, binding of intersection, and vertical bindings
#### object bindings
object snaps, the cursor is fixed on the object

![](obb.png)
![](lb2.png)

*here white point is the cursor snapped to the edge of the rect*

#### intersection bindings
these bindings are formed at the intersection of 2 objects:

![](ib.png)

#### functions
+ `void bl_bind(list *lptr, double *x, double *y);` -- this function creates a binding for the x and y coordinates relative to the list of lptr shapes
+ `char *bl_try_make_object_bind(list *lptr, double *x, double *y);` -- an internal function that attempts to create an object binding to an object
+ `char *bl_try_make_intersection_binding(list *lptr, double *x, double *y);` -- tries to create an intersection binding, otherwise similar `bl_try_make_object_binding`
