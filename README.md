# FlatDraw -- the program is a cross between Paint and professional CAD

## Features
the program is still under development, but the following features are already available:
+ Drawing points
+ Drawing lines at 2 points
+ Drawing lines by point, length, and angle
+ Drawing a rectangle by 2 points
+ Drawing a rectangles by width and height
+ Drawing circles
+ Layers
+ Save / Open
+ Selecting
+ Deleting
+ Free / parametric mode
+ Moving
+ Copy / paste
+ Decouple of rectangles

in development:
+ arcs
+ additional funcs (rotate)

supported formats
+ fd -- internal format of the FlatDraw

the program also supports bindings:
+ To points
+ Intersection bindings
+ Vertical Bindings

## building
To compile and run the program successfully, you will need:
+ gcc compiler
+ gtk library
+ cairo library
+ pkg-config
+ make

To compile, type:
```
make rel
```

## documentation
at the moment the documentation for fd sources is written
see [here](doc/fd_doc.md)

## screenshots
![](screenshots/scr1.png)
![](screenshots/scr2.png)
![](screenshots/scr3.png)

## icons
the icons were borrowed from [this](https://www.flaticon.com/packs/vector-editing-tools-15?k=1605709294421) and [this](https://www.flaticon.com/packs/text-edition) sites
