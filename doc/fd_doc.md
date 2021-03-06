# description of all functions and header files included in FlatDraw

## header list
Name                   | Short description
---------------------- | -------------------------------------------------
[binding.h](#bh)       | implements bindings
[callbacks.h](#ch)     | contains callback functions
[click_handle.h](#chh) | contains functions for handling clicks
[color.h](#clh)        | contains color schemes
[draw.h](#dh)          | contains functions for drawing
[fd_format.h](#ffh)    | contains functions for outputting shapes to a file
[figure.h](#fh)        | includes functions for creating and processing shapes
[geometry.h](#gh)      | includes functions for creating and processing geometry
[list.h](#lh)          | implements a doubly linked list of shapes
[error.h](#eh)         | implements error handling functions
[help.h](#hh)          | implements help messages
[multi_obj.h](#mo)     | combines figure and text
[text.h](#th)          | implements text
[util.h](#uh)          | contains utility functions

## headers description
<a name="bh"></a>
# binding.h
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
##### main
+ `void bl_bind(list *lptr, double *x, double *y);` -- this function creates a binding for the x and y coordinates relative to the list of lptr shapes
  - `lptr` -- pointer to a list of objects
  - `x` -- the x coordinate of the cursor
  - `y` -- the y coordinate of the cursor

##### try
For all of the following functions: 
> `lptr` -- pointer to th list of objects
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `char *bl_try_make_object_bind(list *lptr, double *x, double *y);` -- an internal function that attempts to create an object binding to an object
+ `char *bl_try_make_intersection_binding(list *lptr, double *x, double *y);` -- tries to create an intersection binding, otherwise similar `bl_try_make_object_binding`
+ `char *bl_try_make_vertical_binding(list *lptr, double *x, double *y);` -- tries to create a vertical binding, similar to `bl_try_make_object_binding`

##### is
+ `int bl_is_create_binding(figure *target, double x, double y);` -- returns 1 if you can create an object binding, 0 otherwise
  - `target` -- target figure
  - `x` -- the x coordinate of the cursor
  - `y` -- the y coordinate of the cursor

##### get
+ `double bl_get_coords_dif(double c1, double c2);` -- returns the difference of the coordinates
  - `c1` -- first coordinate
  - `c2` -- second coordinate

For all of the following functions: 
> `figure_name` -- target figure
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `int bl_get_binding_possibility_point(figure *point, double x, double y);` -- returns the ability to create a binding to a point (1 or 0)
+ `int bl_get_binding_possibility_line(figure *line, double x, double y);` -- similar to `bl_get_binding_possibility_point`
+ `int bl_get_binding_possibility_rect(figure *rect, double x, double y);` -- similar to `bl_get_binding_possibility_point`
+ `int bl_get_binding_possibility_circle(figure *circle, double x, double y);` -- similar to `bl_get_binding_possibility_point`

##### make
For all of the following functions: 
> `figure_name` -- target figure
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `void bl_make_binding(figure *fptr, double *x, double *y);` -- directly creates a binding
+ `void bl_make_binding_point(figure *point, double *x, double *y);` -- directly creates a binding to point
+ `void bl_make_binding_line(figure *line, double *x, double *y);` -- directly creates a binding to line
+ `void bl_make_binding_rect(figure *rect, double *x, double *y);` -- directly creates a binding to rect
+ `void bl_make_binding_circle(figure *circle, double *x, double *y);` -- directly creates a binding to circle

---

+ `char *bl_make_vertical_bind_if_possible(list *node, double *x, double *y);` -- creates a vertical binding
  - `node` -- target node
  - `x` -- the x coordinate of the point
  - `y` -- the x coordinate of the point

#### symbols
+ `BINDING_AREA` -- the distance within which the binding is created

<a name="ch"></a>
# callbacks.h
contains callback functions for the interface created in main.c

#### symbols
+ `DRAW_BUTTONS` -- specifies the number of buttons responsible for drawing

#### functions
##### events
+ `gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data);` -- drawing the canvas
  - `area` -- drawing area
  - `cr` -- cairo context
  - `data` -- not used

+ `gboolean mouse_move(GtkWidget *widget, GdkEvent *event, GtkWidget *crd_label);` -- tracks the mouse movement
  - `widget` -- drawing area
  - `event` -- GdkEvent pointer
  - `crd_label` -- label the coordinate display

+ `gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data);` -- tracks mouse clicks
  - `widget` -- drawing area
  - `event` -- GdkEvent pointer
  - `crd_label` -- label the coordinate display

+ `gboolean key_press(GtkWidget *widget, GdkEvent *event, gpointer data);` -- monitors keystrokes of the keyboard. __Currently not used__
  - `widget` -- drawing area
  - `event` -- Gdkevent pointer
  - `data` -- not used


##### bttn (figures)
For all of the following functions: 
> `bttn` -- pointer to the button that the signal is bound to
> `data` -- not used

+ `void point_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for point button
+ `void line_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for line pp button
+ `void line_la_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for line la button
+ `void rect_pp_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for rect pp button
+ `void rect_wh_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for rect wh button
+ `void circle_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for circle button
+ `void arc_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for arc button
+ `void curs_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for curs button

---

For all of the following functions:
> `bttn` -- pointer to the button that the signal is bound to
> `parent_window` -- the parent window of the dialog

+ `void circle_prm_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);` -- callback for parametric circle
+ `void arc_prm_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);` -- callback for parametric arcs
+ `void text_bttn_click(GtkWidget *bttn, GtkWindow *parent_window);` -- callback for text button

##### bttn (file operations)
For all of the following functions:
> `bttn` -- pointer to the button that the signal is bound to
> `parent_window` -- the parent window of the dialog

+ `void save_bttn_click(GtkWidget *bttn, gpointer data);` -- signal for file saving dialog
+ `void open_bttn_click(GtkWidget *bttn, gpointer data);` -- signal for file open dialog

##### bttn (modify)
+ `void del_bttn_click(GtkWidget *bttn, GtkWidget *da);` -- callback for delete button
  - `bttn` -- pointer to the button that the signal is bound to
  - `da` -- drawing area

For all of the following functions:
> `bttn` -- pointer to the button that the signal is bound to
> `parent_window` -- the parent window of the dialog

+ `void move_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for move button
+ `void cp_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for copy-paste button

---

+ `void dc_bttn_click(GtkWidget *bttn, GtkWidget *draw_area);` -- callback for decouple button
  - `bttn` -- pointer to the button that the signal is bound to
  - `drawing_area` -- drawing area

+ `void rot_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);` -- callback for rotation button
  - `bttn` -- pointer to the button that the signal is bound to
  - `parent_window` -- the parent window of the dialog

##### bttn (dialog)
For all of the following functions: 
> `bttn` -- pointer to the button that the signal is bound to
> `data` -- not used

+ `void line_la_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for OK button in line la dialog
+ `void rect_wh_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for OK button in the rect wh dialog
+ `void arc_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for OK button in parametric arcs dialog
+ `void text_dialog_font_button_set(GtkFontButton *bttn, gpointer data);` -- callback for font choosing button in text dialog

---

For all of the following functions: 
> `bttn` -- pointer to the button that the signal is bound to
> `entry` -- a field containing some necessary data

+ `void save_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry);` -- callback for OK button in save dialog
+ `void open_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry);` -- callback for OK button in open dialog
+ `void rot_dialog_apply_bttn(GtkWidget *bttn, GtkWidget *entry)` -- callback for apply button in rotation dialog
+ `void circle_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry);` -- callback for OK button in parametric circle dialog

---

+ `void text_dialog_ok_bttn_click(GtkWidget *bttn, GtkTextBuffer *tb);` -- callback for OK button in text dialog
  - `bttn` -- pointer to the button that the signal is bound to
  - `tb` -- text buffer that contains text

+ `void text_dialog_color_button_set(GtkColorButton *bttn, int *color);` -- gets the RGB color assigned to the button
  - `bttn` -- pointer to the button that the signal is bound to
  - `color` -- an array of three elements representing an rgb palette in the range [0; 255]

+ `void text_dialog_color_button_click(GtkWidget *bttn, GtkWidget *parent);` -- callback for color button in text dialog
  - `bttn` -- pointer to the button that the signal is bound to
  - `parent` -- the parent window of the dialog

##### misc
For all of the following functions: 
> `bttn` -- pointer to the button that the signal is bound to
> `entry` -- a field containing some necessary data

+ `void set_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry);` -- callback for set lay button
+ `void all_bttn_click(GtkWidget *bttn, GtkWidget *entry);` -- callback for all lays button
+ `void add_projection_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry);` -- callback for add projection lay button

---

> `bttn` -- pointer to the button that the signal is bound to
> `parent_window` -- the parent window of the dialog

+ `void options_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);` -- callback for options button
+ `void help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);` -- callback for help button

---

+ `void direction_bttn_click(GtkWidget *bttn, gpointer data);` -- callback for diarection button in line la, rect wh dialogs
  - `bttn` -- pointer to the button that the signal is bound to
  - `data` -- unused

+ `void prm_bttn_click(GtkWidget *bttn, GtkWidget *elms[])` -- callback for prm / free button, its change free / prm drawing mode
  - `bttn` -- pointer to the button that the signal is bound to
  - `elms` -- array of widgets (buttons) to change

+ `void unselect(list *node);` -- function for unselect
  - `node` -- figure list

+ `void cb_dc(list *lptr);` -- function that decouple figure of given node
  - `node` -- figure list

<a name="chh"></a>
# click_handle.h
this section contains functions that handle clicks, and here you can add and select shapes

#### enums
+ `CH_WORK_MODES ` -- contain work modes of click handler
```c
enum CH_WORK_MODES {
	WM_MOVE = FG_TYPE_NONE + 1,
	WM_CP,
	WM_ROTATE,
	WM_TEXT
};
```

#### functions
##### main
+ `void ch_click_handler(GtkWidget *draw_area, list *lptr, int x, int y);` -- common function for click handling, that call rest handle functions
  - `draw_area` -- the area where the click occurs
  - `lptr` -- figure list
  - `x` -- the x coordinate of the click
  - `y` -- the y coordinate of the click

##### set
+ `void ch_set_draw_mode(int new_mode);` -- function for selecting the drawing mode (see figure.h)
  - `new_mode` -- new value

+ `void ch_set_external_figure(figure *fptr);` -- function for selecting a external figure (only for multi-step figure: line_pp, rect_pp, circle, arc)
  - `fptr` -- figure pointer

+ `void ch_set_state(int new_state);` -- function that set creation step for multi-step figures (see ch_set_external_figure)
  - `new_state` -- new value

##### get
+ `int ch_get_draw_mode();` -- function that returns draw mode

##### add
For all following functions:
> `draw_area` -- drawing area
> `lptr` -- figure list
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `void ch_add_point(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new point by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_line_pp(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new line_pp by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_line_la(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new line_la by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_rect_pp(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new rect_pp by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_rect_wh(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new rect_wh by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_circle(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new circle by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_arc(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new arc by x, y in figure list lptr, and draw it in draw_area
+ `void ch_add_circle_prm(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new circle by x and y, and draw it in draw_area
+ `void ch_add_arc_prm(GtkWidget *draw_area, list *lptr, double x, double y);` -- creates new arc by x and y, and draw it in draw_area

##### click_cursor
For all following functions:
> `draw_area` -- drawing area
> `lptr` -- figure list
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `void ch_click_cursor_select(GtkWidget *draw_area, list *lptr, double x, double y);` -- handle a click, when select Cursor mode
+ `void ch_click_cursor_unselect_all(GtkWidget *draw_area, list *lptr, double x, double y);` -- unselect all selected

---

For all following functions:
> `lptr` -- figure list
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `list *ch_click_cursor_select_text(list *lptr, double x, double y);` -- function for text selecting
+ `list *ch_click_cursor_select_figure(list *lptr, double x, double y);` -- function for figure selecting

##### additional
For all following functions:
> `draw_area` -- drawing area
> `lptr` -- figure list
> `x` -- the x coordinate of the cursor
> `y` -- the y coordinate of the cursor

+ `void ch_move(GtkWidget *draw_area, list *lptr, double x, double y);` -- figure moving
+ `void ch_cp(GtkWidget *draw_area, list *lptr, double x, double y);` -- figure copy-paste
+ `void ch_rotate(GtkWidget *draw_area, list *lptr, double x, double y);` -- figure rotation
+ `void ch_text(GtkWidget *draw_area, list *lptr, double x, double y);` -- add text

---

For all following functions:
> `lptr` -- figure list

+ `void ch_text_move(list *lptr);` -- text move
+ `void ch_copy_text(list *lptr);` -- text copy-paste

##### misc
For all following functions:
> `lptr` -- figure list

+ `void ch_fugure_move(list *lptr);` -- service function for moving all selected figures
+ `void ch_copy_offset(list *lptr);` -- service function for copy-paste all selected figures with moving
+ `void ch_rot(list *lptr);` -- service function for rotation all selected figuresaround base point
+ `void ch_proc_text(list *lptr);` -- sets text coordinates
+ `void ch_click_cursor_unselect_text(list *lptr);` -- unselect all selected text

---

+ `void ch_unselect_last();` -- function that unselect last selected

<a name="clh"></a>
# color.h
thats header contain enum and functions for colorisation

#### enums
+ `DEF_COLORS` -- thats enum contain color set for FlatDraw
```c
enum DEF_COLORS {
	CL_DEF_CURS_COLOR = 0,
	CL_DEF_DRAW_COLOR,
	CL_DEF_PREVIEW_COLOR,
	CL_DEF_PROJECTION_COLOR,
	CL_DEF_SELECTED_COLOR
};
```

#### functions
##### set
+ `void cl_set_color(cairo_t *cr, int color_set);` -- set drawing color for cairo context cr, form color_set (see `DEF_COLORS`)
  - `cr` -- cairo context
  - `color_set` -- color mode (see `DEF_COLORS`)

+ `void cl_set_color_fg(cairo_t *cr, int fg_color_set);` -- set drawing color for cairo context cr, from VM colorset (see figure.h)
  - `cr` -- cairo context
  - `fg_color_set` -- figure color mode (see `VISIBLE_MODES` in figure.h)

<a name="dh"></a>
# draw.h
that header defines drawing functions

#### functions
##### draw
For all following functions:
> `fptr` -- figure pointer

+ `void dl_draw_figure(figure *fptr);` -- common function for drawing figure thats call rest functions
+ `void dl_draw_figure_list(list *lptr);` -- similar to `dl_draw_figure`, but takes a list of figures
+ `void dl_draw_point(figure *fptr);` -- draw point
+ `void dl_draw_line_pp(figure *fptr);` -- draw line_pp
+ `void dl_draw_rect_pp(figure *fptr);` -- draw rect_pp
+ `void dl_draw_circle(figure *fptr);` -- draw circle
+ `void dl_draw_arc(figure *fptr);` -- draw arc
+ `void dl_draw_preview(figure *fptr);` -- draw preview figure (is a milti-step figure, thats not finished, see click_handle.h)

---

+ `void dl_draw_text(text *tptr);` -- draw text
  - `tptr` -- text pointer

##### set
+ `void dl_set_cairo_context(cairo_t *cr);` -- set a cairo context
  - `cr` -- cairo context

+ `void dl_set_preview_coords(double px, double py);` -- set a curcor position, for drawing a preview figure
  - `px` -- preview x coordinate
  - `py` -- preview y coordinate

+ `void dl_set_show_preview(int val);` -- enable or disable drawing preview figure
  - `val` -- new value

##### is
+ `int dl_is_need_draw(figure *fptr);` -- returns 1 if need draw, 0 otherwise
  - `fptr` -- figure pointer

##### switch
+ `void dl_switch_display_all_lays();` -- switch displaying all lays
+ `void dl_switch_show_preview();` -- switch showing preview figure

##### send
+ `void dl_send_preview_figure(figure *fptr);` -- send to draw system a preview figure
  - `fptr` -- figure pointer

##### get
+ `int dl_get_preview();` -- returns whether or not a thumbnail is shown

<a name="ffh"></a>
# fd_format.h
this header defines functions for working with .fd files

#### symbols
+ `FD_FORMAT_END` -- this symbol declarates end of figure defenition

#### functions
+ `void fdl_target_file(char *name);` -- specifies the name of the target file
  - `name` -- name of target file

##### write
+ `void fdl_write_object_stream(FILE *stream, list *lptr);` -- writes lptr to stream
  - `stream` -- file stream, wich object will be written
  - `lptr` -- figure list

+ `void fdl_write_from_list(list *lptr);` -- writes figures from list to target file
  - `lptr` -- figure list

##### read
+ `void fdl_read_string(figure *fptr, char *str);` -- reads string from .fd file
  - `fptr` -- figure pointer
  - `str` -- buffer

+ `void fdl_read_file(list *lptr);` -- read figures from target file to list
  - `lptr` -- figure list

##### pars
For all following functions:
> `stream` -- the file stream from which the shape will be read
> `lptr` -- figure list

+ `void fdl_pars_figure(FILE *stream, list *lptr);` -- extract figure from file
+ `void fdl_pars_text(FILE *stream, list *lptr);` -- extract text from file

<a name="fh"></a>
# figure.h
this header contains functions for working with figures

#### structs
+ `figure` -- this struct defines a base figure
```c
typedef struct {
	double x; // base x coord
	double y; // base y coord

	// additionals
	double a1;
	double a2;
	double a3;

	int type; // figure type (see FG_TYPES)

	int visible; // figure visible mode (see VISIBLE_MODES)

	int lay; // layer of the figure
} figure;
```

#### enums
+ `FG_TYPES` -- this enum defines figure types
```c
enum FG_TYPES {
	FG_TYPE_POINT = 0,
	FG_TYPE_LINE_PP,
	FG_TYPE_LINE_LA,
	FG_TYPE_RECT_PP,
	FG_TYPE_RECT_WH,
	FG_TYPE_CIRCLE,
	FG_TYPE_ARC,
	FG_TYPE_CIRCLE_PRM,
	FG_TYPE_ARC_PRM,
	FG_TYPE_NONE // cursor
};
```

+ `VISIBLE_MODES` -- this enum defines visible modes of figure
```c
enum VISIBLE_MODES {
	VM_HIDE = 0,
	VM_SHOW,
	VM_PREVIEW,
	VM_PROJECTION,
	VM_SELECTED,
	VM_NOT_FINISHED
};
```

#### functions
##### new
+ `figure *figure_new(double type, double x, double y, double a1, double a2, double a3);` -- this function creates a new figure by given parametrs, default visibility - VM_SHOW
  - `type` -- type of the figure
  - `x` -- x coordinate
  - `y` -- y coordinate
  - `a1` -- additional argument 1
  - `a1` -- additional argument 2
  - `a1` -- additional argument 3

+ `figure *figure_new_point(double x, double y);` -- creates new point by x, y
  - `x` -- x coordinate
  - `y` -- y coordinate

+ `figure *figure_new_line_pp(double x1, double y1, double x2, double y2);` -- creates new line_pp by x1, y1 and x2, y2
  - `x1` & `y1` -- start point
  - `x2` & `y2` -- end point

+ `figure *figure_new_rect_pp(double x, double y, double w, double h);` -- creates new rect_pp by x1, y1, and x2, y2
  - `x` -- x coordinate
  - `y` -- y coordinate
  - `w` -- weight
  - `h` -- height

+ `figure *figure_new_circle(double x, double y, double r);` -- creates new circle by x, y with R radii
  - `x` -- x coordinate
  - `y` -- y coordinate
  - `r` -- radii

+ `figure *figure_new_arc(double xc, double yc, double r, double angle1, double angle2);` -- creates new arc by xc, yc, starts in angle1 and end in angle2 (__doesn't work__)

##### free
+ `void figure_free(figure *fptr);` -- free allocated figure
  - `fptr` -- figure pointer

+ `void figure_free_list(list *lptr);` -- free allocated figure from list
  - `lptr` -- figure list

##### get
+ `int figure_get_current_lay();` -- returns current working layer

+ `figure *figure_get_from_node(list *lptr);` -- extract figure from node of list
  - `lptr` -- figure list

##### set
+ `void figure_set_visible_by_lay_list(list *lptr, int lay, int vm_mode);` -- set visible mode for figures on given layer
  - `lptr` -- figure list
  - `lay` -- layer number
  - `vm_mode` -- visible mode

+ `void figure_set_current_lay(int new_lay);` -- set currunt working layer
  - `new_lay` -- layer number

##### is
+ `int figure_is_line(figure *fptr);` -- returns 1 if it is line, 0 otherwise
  - `fptr` -- figure pointer

+ `int figure_is_projection_lay_list(list *lptr, int lay);` -- returns 1 if lay is a projection lay, 0 otherwise
  - `lptr` -- figure list
  - `lay` -- layer number

##### rotate
For all following functions:
> `lptr` -- figure list
> `base (p, l, r)` -- figure pointer
> `angle` -- the angle at which the shape will be rotated

+ `void figure_rotate(list *lptr, figure *base, double angle);` -- rotates the figure contained in the `lptr` around the `base` point by an `angle`
+ `void figure_rotate_point(figure *base, figure *p, double angle);` -- rotates point `p` around `base` by an `angle`
+ `void figure_rotate_line(figure *base, figure *l, double angle);` -- rotates line `l` around `base` by an `angle`
+ `void figure_rotate_rect(figure *base, figure *r, double angle, list *lptr);` -- rotates rectangle `r` around `base` by an `angle`

##### misc
+ `figure *figure_rect_decompose(figure *rect);` -- decomposes rect to 4 lines
  - `rect` -- rect to be decouple

+ `void figure_fill(figure *fptr, double x, double y, double a1, double a2, double type);` -- fill allocated figure given parametrs
  - `fptr` -- figure pointer
  - `x` -- x coordinate
  - `y` -- y coordinate
  - `a1` -- additional argiment 1
  - `a2` -- additional argiment 1
  - `type` -- figure type (see `FIGURE_TYPES`)

+ `void figure_copy(figure *dst, figure *src);` -- copy dst parametrs to src
  - `dst` -- target figure
  - `src` -- source figure

<a name="gh"></a>
# geometry.h
this header contains geometry processing functions

#### symbols
+ `PI` -- it is pi

#### functions
##### calculate
+ `double gel_calculate_lenght(figure *line);` -- calculates line lenght
  - `line` -- the line whose length will be calculated

+ `char *gel_calculate_intersection(figure *line1, figure *line2, figure *p);` -- calculate intersection of 2 lines
  - `line1` -- first line
  - `line2` -- second line
  - `p` -- result point

+ `void gel_calculate_line_la(figure *point, double lenght, double angle);` -- convert line_la to line_pp
  - `point` -- start point of line
  - `lenght` -- line lenght
  - `angle` -- line angle

+ `double gel_calculate_line_angle(figure *l);` -- calculates angle of line
  - `l` -- line

+ `double gel_calculate_heron_formula(double a, double b, double c);` -- calculates square by heron formula
  - `a` -- first side
  - `b` -- second side
  - `c` -- third side

For all following functions:
> `line` -- line by 2 points

+ `double gel_lf_calculate_k(figure *line);` -- calculates *k* from linear function by given line
+ `double gel_lf_calculate_b(figure *line);` -- calculates *b* from linear function by given line

---

+ `int gel_lf_is_parallel(figure *line1, figure *line2);` -- return 1 if lines are parallel
  - `line1` -- first line
  - `line2` -- second line

##### convert
+ `double gel_convert_grades_to_rads(double grades);` -- converts grades to rads
  - `grades` -- value in grades

+ `double gel_convert_rads_to_grades(double rads);` -- converts rads to grades
  - `rads` -- value in rads

+ `double gel_convert_angle_by_crd(figure *l, double a);` -- converts angle `a` by given coordiantes (in view of the coordinate quarter)
  - `l` -- line
  - `a` -- angle for conversion

+ `void gel_convert_crd_by_angle(figure *l, double a, double lx, double ly);` -- converts given coordinates in view of angle `a`
  - `l` -- start point of line
  - `a` -- angle
  - `lx` -- end point of line (x)
  - `ly` -- end point of line (y)

##### get
+ `figure *gel_get_middle_point(figure *line);` -- return middle point of line
  - `line` -- line

+ `double gel_get_line_angle(figure *l);` -- return angle of line
  - `l` -- line

+ `double gel_get_max_lenght(figure *a, figure *b, figure *c);` -- returns the length of the longest line
  - `a` -- first line
  - `b` -- second line
  - `c` -- third line

+ `figure *gel_get_equal_point(figure *line1, figure *line2);` -- returns point where lines begins or ends otherwise NULL
  - `line1` -- first line
  - `line2` -- second line

+ `figure *gel_get_arc_center(figure *l1, figure *l2);` -- returns center of the arc
  - `l1` -- first line
  - `l2` -- second line

##### is_point_in
+ `int gel_is_middle_point_area(figure *line, double x, double y, int area);` -- returns 1 if point by x y is a middle point of line in area
  - `line` -- line
  - `x` -- x coordinate of point
  - `y` -- y coordinate of point
  - `area` -- valid area

For all following functions:
> `l/r/c/p1/area` -- base figure
> `p/p2` -- the figure lying (or not lying) on (inside) the base

+ `int gel_is_point_in_line(figure *l, figure *p);` -- returns 1 if point p lies on the line
+ `int gel_is_point_in_rect(figure *r, figure *p);` -- similar to `gel_is_point_in_line`
+ `int gel_is_point_in_circle(figure *c, figure *p);` -- similar to `gel_is_point_in_line`
+ `int gel_is_point_in_point(figure *p1, figure *p2);` -- similar to `gel_is_point_in_line`
+ `int gel_is_point_in_area(figure *area, figure *p);` -- similar to `gel_is_point_in_line`

##### misc
+ `void gel_rotate_point(double *x, double *y, double angle);` -- rotate point with `x` and `y` coordinates on `angle` around (0; 0)
  - `x` -- x coordinate
  - `y` -- y coordinate
  - `angle` -- rotate angle

<a name="lh"></a>
# list.h
this header contains functions for working with twice-linked lists

#### structs
+ `list` -- structure of 1 element of list
```c
typedef struct __list__ {
	struct __list__ *prev;
	struct __list__ *next;

	void *data;

	OBJ_TYPE dt; // data type
} list;
```

+ `OBJ_TYPE` -- enum of object types
```c
typedef enum {
	OT_FIGURE = 0,
	OT_TEXT
} OBJ_TYPE;
```

#### functions
##### common
+ `list *list_init_node(list *parent);` -- creates new node with parent
  - `parent` -- parent (previous) node

##### free
For all following functions:
> `node` -- figure list

+ `void list_free_node(list *node);` -- free node
+ `void list_free_list(list *node);` -- free all list

##### get
+ `list *list_get_last(list *root);` -- returns last element of the list
  - `root` -- some node (or root) of list

+ `void *list_get_data(list *lptr);` -- returns data field of the list
  - `lptr` -- node pointer

##### misc
+ `void list_add_node(list *root);` -- add new node to end of list
  - `root` -- root of the list

+ `void list_set_data(list *node, void *new_data);` -- set data field of the list
  - `node` -- node pointer
  - `new_data` -- new value

+ `void list_show(list *root);` -- prints list to console
  - `root` -- root of the list

+ `void list_crawl(list *root, void (*func)(list*));` -- crawls list and call func for each node
  - `root` -- foor of the list
  - `func` -- function to be called for each node

<a name="eh"></a>
# error.h
this header contains functions and symbols for error handling

#### enums
+ `ERROR_TYPES` -- this enum stores errors codes
```c
enum ERROR_TYPES {
	ET_WRONG_SELECTING = 0
};
```

#### functions
##### set
+ `void el_set_par_window(GtkWidget *window);` -- sets parent window for error dialog
  - `window` -- parent window widget

##### call
+ `void el_call_error(int err_type);` --  calls dialog by given code
  - `err_type` -- error type (see `ERROR_TYPES`)

+ `void el_call_dialog(char *msg);` -- creates dialog with given text
  - `msg` -- custom message

<a name="hh"></a>
# help.h
this header contains help messages and functions for processing them 

#### enums
+ `HELP_CODES ` -- this enum stores help messages codes
```c
enum HELP_CODES {
	HC_MAIN = 0,
	HC_POINT,
	HC_START_POINT,
	HC_END_POINT,
	HC_CENTER_POINT,
	HC_RADIUS_POINT,
	HC_CUROR,
	HC_INS_POINT,
	HC_BASE_POINT
};
```

#### functions
##### get
+ `char *hl_get_help(int code);` -- returns help message by code
  - `code` -- help code (see `HELP_CODES`)

##### set
+ `void hl_set_widget(GtkWidget *label);` -- sets label for help messages
  - `label` -- label, that will used for hints showing

+ `void hl_set_help(int code);` -- sets help message by code
  - `code` -- help code (see `HELP_CODES`)

<a name="mo"></a>
# multi_obj.h
this header contains functions for combine text and figures

#### symbols
+ `multi_obj` -- this struct implements multi_obj -- common interface for figure and text
```c
typedef struct {
	double x;
	double y;

	int visible;

	int lay;
} multi_obj;
```

#### functions
##### common
For all following functions:
> `lptr` -- list pointer

+ `void mol_free_from_node(list *lptr);` -- common interface for freeing up memory
+ `void mol_draw_obj_from_node(list *lptr);` -- common interface for drawing objects (figure and text)
+ `figure *mol_conv_to_figure(list *lptr);` -- converts multi_object to point

##### extract
For all following functions:
> `lptr` -- list pointer

+ `multi_obj *mol_extract(list *lptr);` -- extracts multi_obj from a list node
+ `multi_obj *mol_extract_from_text(list *lptr);` -- extract multi_obj from text structure
+ `multi_obj *mol_extract_from_figure(list *lptr);` -- extract multi_obj from figure structure

##### apply
For all following functions:
> `lptr` -- list pointer
> `mo` -- multi_obj pointer

+ `void mol_apply(list *lptr, multi_obj *mo);` -- writes multi_obj to list node
+ `void mol_apply_text(list *lptr, multi_obj *mo);` -- writes multi_obj to text
+ `void mol_apply_figure(list *lptr, multi_obj *mo);` -- writes multi_obj to figure

<a name="th"></a>
# text.h
this header contains functions for text processing

#### symbols
+ `text` -- implements text
```c
typedef struct {
	char *buffer;

	char *font;

	int size;

	int color_r;
	int color_g;
	int color_b;

	int visible;

	double x;
	double y;

	int lay;
} text;
```

#### functions
##### free
For all following functions:
> `tptr` -- text pointer

+ `void tl_free(text *tptr);` -- frees text
+ `void tl_free_buffer(text *tptr);` -- free buffer of text

##### misc
+ `text *tl_new(char *font, int size, int r, int g, int b);` -- creates new text
  - `font` -- font
  - `size` -- text size
  - `r` & `g` & `b` -- RGB palette in the range [0; 255]

+ `void tl_add_buffer(text *tptr, char *buf);` -- adds buffer to text
  - `tptr` -- text pointer
  - `buf` -- buffer

<a name="uh"></a>
# util.h
this header contains utility functions

+ `double ul_map(double x, double in_min, double in_max, double out_min, double out_max);` -- converts a value from one system of units to another
  - `x` -- converted value
  - `in_min` & `in_max` -- values of range [in_min; in_max] which contains x
  - `out_min` & `out_max` -- values of range [out_min; out_max]

+ `void ul_get_colors(int *arr, GtkWidget *color_entry);` -- writes colors of color_entry to arr
  - `arr` -- the array to return the palette to
  - `color_entry` -- a field containing the feed text: `R G B`

+ `void ul_pars_font(char *str, char *font_name, char *font_size);` -- parses a string like "font-name font-size", returns the result in arguments
  - `str` -- str cantaining text: "Font-name font-size"
  - `font_name` -- a buffer in which to return the name of the font
  - `font_size` -- a buffer int which to return the size of the font

---

FlatDraw v0.17 17.02.2021 by Centrix14
