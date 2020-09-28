#include <stdio.h>

#include "figure.h"
#include "geometry.h"

int main() {
	figure *l1, *l2, *p;

	l1 = figure_new_line_pp(0, 0, 3, 3);
	l2 = figure_new_line_pp(5, 0, 0, 5);

	p = figure_new_point(0, 0);

	gel_calculate_intersection(l1, l2, p);

	printf("Intr(l1, l2) = (%d, %d)\n", p->x, p->y);

	return 0;
}
