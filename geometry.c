#include <stdio.h>
#include <math.h>

#include "geometry.h"
#include "figure.h"

double gel_calculate_increase(double lenght, double c1, double c2) {
	return (c2 - c1) / (lenght - 1);
}

double gel_calculate_lenght(figure *line) {
	int lx, ly; // lenght-x, lenght-y
	double lenght;

	lx = line->a1 - line->x;
	ly = line->a2 - line->y;

	lenght = sqrt(pow(lx, 2) + pow(ly, 2));

	return lenght;
}

int gel_is_end_coords(figure *line, int x, int y) {
	if (line->a1 == x && line->a2 == y)
		return 1;
	return 0;
}

void gel_calculate_intersection(figure *line1, figure *line2, figure *p) {
	int l1x, l1y, l2x, l2y;
	int dx1, dx2, dy1, dy2;

	dx1 = gel_calculate_increase(gel_calculate_lenght(line1), line1->x, line1->a1);
	dx2 = gel_calculate_increase(gel_calculate_lenght(line2), line2->x, line2->a1);

	dy1 = gel_calculate_increase(gel_calculate_lenght(line1), line1->y, line1->a2);
	dy2 = gel_calculate_increase(gel_calculate_lenght(line2), line2->y, line2->a2);

	l1x = line1->x;
	l1y = line1->y;

	l2x = line2->x;
	l2y = line2->y;

	while ((line1->a1 != l1x && line1->a2 != l1y) && (line2->a1 != l2x && line2->a2 != l2y)) {
		if ((l1x == l2x) && (l1y == l2y)) {
			p->x = l1x;
			p->y = l1y;
		}

		l1x += dx1; l1y += dy1;
		l2x += dx2; l1y += dy2;
	}
}
