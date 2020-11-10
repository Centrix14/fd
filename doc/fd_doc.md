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

#### vertical bindings
these bindings are to the x or y of another object:

![](vb.png)

#### functions
+ `void bl_bind(list *lptr, double *x, double *y);` -- this function creates a binding for the x and y coordinates relative to the list of lptr shapes
+ `char *bl_try_make_object_bind(list *lptr, double *x, double *y);` -- an internal function that attempts to create an object binding to an object
+ `char *bl_try_make_intersection_binding(list *lptr, double *x, double *y);` -- tries to create an intersection binding, otherwise similar `bl_try_make_object_binding`
+ `char *bl_try_make_vertical_binding(list *lptr, double *x, double *y);` -- tries to create a vertical binding, similar to `bl_try_make_object_binding`
+ `int bl_is_create_binding(figure *target, double x, double y);` -- returns 1 if you can create an object binding, 0 otherwise
+ `double bl_get_coords_dif(double c1, double c2);` -- returns the difference of the coordinates
+ `int bl_get_binding_possibility_point(figure *point, double x, double y);` -- returns the ability to create a link to a point (1 or 0)
+ `int bl_get_binding_possibility_line(figure *line, double x, double y);` -- similar th `bl_get_binding_possibility_point`
+ `int bl_get_binding_possibility_rect(figure *line, double x, double y);` -- similar th `bl_get_binding_possibility_point`
+ `void bl_make_binding(figure *fptr, double *x, double *y);` -- directly creates a binding
+ `void bl_make_binding_point(figure *point, double *x, double *y);` -- directly creates a binding to point
+ `void bl_make_binding_line(figure *point, double *x, double *y);` -- directly creates a binding to line
+ `void bl_make_binding_rect(figure *point, double *x, double *y);` -- directly creates a binding to rect
+ `char *bl_make_vertical_bind_if_possible(list *node, double *x, double *y);` -- creates a vertical binding

#### symbols
+ `BINDING_AREA` -- the distance within which the binding is created

<a name="ch"></a>
### callbacks.h

<a name="chh"></a>
### ckick_handle.h

<a name="clh"></a>
### color.h

<a name="dbh"></a>
### dbg.h

<a name="dh"></a>
### draw.h

<a name="clh"></a>
### color.h

<a name="ffh"></a>
### fd_format.h

<a name="fh"></a>
### figure.h

<a name="gh"></a>
### geometry.h

<a name="lh"></a>
### list.h
