#ifndef __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__

#include "figure.h"

#define PI 3.14159265359

double gel_calculate_lenght(figure *line);

void gel_calculate_intersection(figure *line1, figure *line2, figure *point);

double gel_lf_calculate_k(figure *line);
double gel_lf_calculate_b(figure *line);
int gel_lf_is_parallel(figure *line1, figure *line2);

void gel_calculate_line_la(figure *point, double lenght, double angle);

double gel_convert_grades_to_rads(double grades);

#endif
