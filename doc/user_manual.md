# FlatDraw user manual

## Content
+ [Interface](#interface)
+ [Drawing](#drawing)
    - [Points](#points)
	- [Lines](#lines)
	    - [Free](#lfree)
	    - [Parametric](#lprm)
	- [Rectangles](#rect)
	    - [Free](#rfree)
	    - [Parametric](#rprm)
	- [Circles](#circles)
	    - [Free](#cfree)
	    - [Parametric](#cprm)
	- [Arcs](#arcs)
	    - [Parametric](#aprm)
	- [Text](#text)
+ [Modification](#mod)
    - [Delete](#delete)
	- [Copy/Paste](#cp)
	- [Move](#move)
	- [Rotate](#rotate)
	- [Decouple](#dc)
+ [Common tool](#ct)
    - [Layer field](#lf)
	- [Save](#save)
	- [Open](#open)
	- [Drawing mode](#dm)
	- [Options](#opt)
	- [+ / -](#+-)
	- [All](#all)
	- [Set](#set)
+ [Information panel](#ip)

<a name="interface"></a>
## Interface
In the screenshot below, the main interface elements are highlighted and signed in red.
![](ui.png)
1. The workspace where you are drawing
2. Modification panel. The tools in this panel allow you to move, copy, Ungroup, rotate, and delete figures
3. Drawbar. The tools in this panel allow you to draw points, lines, rectangles, and circles.
4. Common tool.
5. The information panel here you can see the tooltip and the cursor position.

### Interaction with the workspace
1. To create \ start, left-click on the desired area
2. If you want to abort the action, click the right mouse button

<a name="drawing"></a>
## Drawing
<a name="points"></a>
### Points
To put a point, click on the "Point" button and then click in the appropriate places in the workspace. Points will appear after each click.
#### point button
![point button](bttn/point.png)

<a name="lines"></a>
### Lines
FD supports 2 ways to create lines: free and parametric.
#### button line
![line button](bttn/line.png)

<a name="lfree"></a>
#### Free mode
First, make sure that the "Line" button is set to free mode. Otherwise, switch it. Click on the "Line" button
Then you can start drawing. Click to start the line, and click again to finish it, after moving the cursor to the desired location. When creating a drawing, you don't need to hold down the left mouse button.

<a name="lprm"></a>
#### Parametric mode
First, make sure that the "Line" button is set to parametric mode. Otherwise, switch it. Click on the "Line" button
You will see a dialog box where you can set the angle and length of the line, as well as the position relative to the cursor, in the corresponding fields.

<a name="rect"></a>
### Rectangles
FD supports 2 ways to create rectangles: free and parametric.

#### rect button
![rect button](bttn/rect.png)

<a name="rfree"></a>
#### Free mode
First, make sure that the "Rect" button is set to free mode. Otherwise, switch it. Click on the "Rect" button
Then you can start drawing. Click to start the rect, and click again to finish it, after moving the cursor to the desired location. When creating a drawing, you don't need to hold down the left mouse button.

<a name="rprm"></a>
#### Parametric mode
First, make sure that the "Rect" button is set to parametric mode. Otherwise, switch it. Click on the "Rect" button
You will see a dialog box where you can set the angle and length of the rect, as well as the position relative to the cursor, in the corresponding fields.

<a name="circles"></a>
### Circles
At the moment, we only support 1 way to draw circles: free. Click on the "Circle" button.

#### circle button
![circle button](bttn/circle.png)

<a name="cfree"></a>
#### Free mode
Click to start the circle, and click again to finish it, after moving the cursor to the desired location. When creating a drawing, you don't need to hold down the left mouse button.

<a name="cprm"></a>
#### Parametric mode
Since version 0.16, parameterized arcs have been added. To start drawing, switch to parametric mode.
After that, click on the "Circle" button. In the window that opens, fill in the fields, click "OK" and then click on the area where the center of the circle should be located.

<a name="arcs"></a>
### Arcs
Please use parametric mode only!

#### arc button
![arc button](bttn/arc.png)

<a name="aprm"></a>
#### Parametric mode
Since version 0.16, parameterized arcs have been added. To start drawing, switch to parametric mode.
After that, click on the "Arc" button. In the window that opens, fill in the fields, click "OK" and then click on the area where the center of the arc should be located.

<a name="text"></a>
### Text
Text in FlatDraw supports the following features: different color, font, size.
To create a text, click on the "Text" button. Then enter the required parameters in the input fields (or make a selection using the appropriate buttons). After that, close the dialog, and click to add the text in the desired place

#### text button
![text button](bttn/text.png)

<a name="mod"></a>
## Modification
To use the modification tools, switch to cursor mode (the "Cursor" button on the drawing panel). Then select the shape or group of shapes to apply the modification to.

#### cursor button
![cursor button](bttn/cursor.png)

<a name="delete"></a>
### Delete
To delete the selected shapes, click the button.

#### delete button
![delete button](bttn/delete.png)

<a name="cp"></a>
### Copy/Paste
To copy the selected shapes, click the button. Then, select the base point relative to which you want to move. Then click on the point where the copied image should be inserted.

#### copy/paste button
![copy-paste button](bttn/copy-paste.png)

<a name="move"></a>
### Move
This tool works similarly to Copy/Paste. But unlike it, "Move" deletes the original.

#### move button
![move button](bttn/move.png)

<a name="rotate"></a>
### Rotate
By clicking on the button, a dialog will open in which you can select the rotation angle using the corresponding field. once the angle is specified, click "Apply".
After that, you must specify the point relative to which the shape should rotate.
The rotate option is not available for text.

#### rotate button
![rotate button](bttn/rotate.png)

<a name="dc"></a>
### Decouple
This tool is only available for rectangles. It turns the rectangle into four adjacent lines.
Click the button and the rectangle will be ungrouped.
The rotate option is not available for text.

#### decouple bttn
![decouple button](bttn/decouple.png)

<a name="ct"></a>
## Common tool
#### common tool panel
![common tool panel](bttn/info-panel.png)

<a name="lf"></a>
### Layer field
The only field located on the General toolbar indicates the current working level, or the level that will be made current.

<a name="save"></a>
### Save 
To save the current drawing, click on the "Save" button. after that, a dialog will open, enter the path where the file should be located, then click "OK".

<a name="open"></a>
### Open
To open a drawing, click on the "Open" button and a dialog will open. In the text field, enter the path to the file and click "OK".

<a name="dm"></a>
### Drawing mode
This button changes the drawing mode. The label on it indicates the current mode. "Free" is free mode, "Prmt" is parametric mode.

<a name="opt"></a>
### Options
Don't use this feature!

<a name="+-"></a>
### + / -
This button is used to add or remove the current layer from the projection list. Projection layers are the layers that will be displayed along with the current working layer.

<a name="all"></a>
### All
This button displays all drawing layers in the workspace.

<a name="set"></a>
### Set
This button selects the level set in the field as current.

<a name="ip"></a>
## Information panel
In this panel, you can see the cursor position in parentheses on the left, and short hints on the right.

---
FlatDraw v0.17 14.02.2021 by Centrix14
