#ifndef __FLAT_DRAW_CAD_C_MODEL_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CAD_C_MODEL_LIBRARY_HEADER_FILE_INCLUDED__

// side functions
void init(list *msg_list);
void model();

// core functions
void point(double x, double y);
void line_xy(double x1, double y1, double x2, double y2);
void line_ar(double x, double y, double angle, double r);
void rect_xy(double x1, double y1, double x2, double y2);
void rect_wh(double x1, double y1, double w, double h);
void circle(double x, double y, double r);
void label(double crd[2], int rgb[3], unsigned int size, char *font, char *str);

// util functions
void info();

// service functions
void set_msg_list(list *msg_list);

#endif
