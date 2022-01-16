#ifndef __FLAT_DRAW_CAD_C_MODEL_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CAD_C_MODEL_LIBRARY_HEADER_FILE_INCLUDED__

// side functions
void init(list *msg_list);
void model();

// core functions
figure *point(double x, double y);
figure *line_xy(double x1, double y1, double x2, double y2);
figure *line_ar(double x, double y, double angle, double r);
figure *rect_xy(double x1, double y1, double x2, double y2);
figure *rect_wh(double x1, double y1, double w, double h);
figure *circle(double x, double y, double r);
text *label(double crd[2], int rgb[3], unsigned int size, char *font, char *str);

// util functions
void info();
void erase(void *ptr, OBJ_TYPE ot);
char *get_param(char *msg);

// service functions
void set_msg_list(list *msg_list);

#endif
