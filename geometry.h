#ifndef __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__

#include "figure.h"

#define PI 3.14159265359

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

figure *gel_get_middle_point(figure *line);
double gel_get_line_angle(figure *l);
double gel_get_max_lenght(figure *a, figure *b, figure *c);
figure gel_get_triangle_center(figure *a, figure *b, figure *c);
figure *gel_get_middle_point(figure *line);
figure *gel_get_equal_point(figure *line1, figure *line2);
figure *gel_get_center_point_by_hr(double xh, double yh, double R);
figure *gel_get_arc_center(figure *l1, figure *l2);

int gel_is_middle_point_area(figure *line, double x, double y, int area);
int gel_is_point_in_line(figure *l, figure *p);
int gel_is_point_in_rect(figure *r, figure *p);
int gel_is_point_in_circle(figure *c, figure *p);
int gel_is_point_in_point(figure *p1, figure *p2);

#endif
