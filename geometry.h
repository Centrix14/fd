#ifndef __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_GEOMETRY_CACLUCATIONS_LIBRARY_HEADER_FILE_INCLUDED__

#include "figure.h"

double gel_calculate_increase(double lenght, double c1, double c2);
double gel_calculate_lenght(figure *line);
int gel_is_end_coords(figure *line, int x, int y);
void gel_calculate_intersection(figure *line1, figure *line2, figure *point);

#endif
