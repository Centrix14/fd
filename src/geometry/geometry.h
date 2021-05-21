#ifndef __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__

#include "../figure/figure.h"

#define PI 3.14159265358979323846

double gel_calculate_lenght(figure *line);
char *gel_calculate_intersection(figure *line1, figure *line2, figure *p);
void gel_calculate_line_la(figure *point, double lenght, double angle);
double gel_calculate_line_angle(figure *l);
double gel_calculate_heron_formula(double a, double b, double c);

double gel_lf_calculate_k(figure *line);
double gel_lf_calculate_b(figure *line);
int gel_lf_is_parallel(figure *line1, figure *line2);

double gel_convert_grades_to_rads(double grades);
double gel_convert_rads_to_grades(double rads);
double gel_convert_angle_by_crd(figure *l, double a);
void gel_convert_crd_by_angle(figure *l, double a, double lx, double ly);

figure *gel_get_middle_point(figure *line);
double gel_get_line_angle(figure *l);
double gel_get_max_lenght(figure *a, figure *b, figure *c);
figure *gel_get_equal_point(figure *line1, figure *line2);
figure *gel_get_arc_center(figure *l1, figure *l2);

int gel_is_middle_point_area(figure *line, double x, double y, int area);
int gel_is_point_in_line(figure *l, figure *p);
int gel_is_point_in_rect(figure *r, figure *p);
int gel_is_point_in_circle(figure *c, figure *p);
int gel_is_point_in_point(figure *p1, figure *p2);
int gel_is_point_in_area(figure *area, figure *p);

void gel_rotate_point(double *x, double *y, double angle);

double gel_min(double v1, double v2);
double gel_max(double v1, double v2);

double gel_reflect_coord(double cb, double ct);

#endif
