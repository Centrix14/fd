#ifndef __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__

#include "figure.h"

#define PI 3.14159265359

double gel_calculate_lenght(figure *line);
char *gel_calculate_intersection(figure *line1, figure *line2, figure *p);
void gel_calculate_line_la(figure *point, double lenght, double angle);

double gel_lf_calculate_k(figure *line);
double gel_lf_calculate_b(figure *line);
int gel_lf_is_parallel(figure *line1, figure *line2);

double gel_convert_grades_to_rads(double grades);

figure *gel_get_middle_point(figure *line);

int gel_is_middle_point_area(figure *line, double x, double y, int area);

figure *gel_get_equal_point(figure *line1, figure *line2);

#endif
