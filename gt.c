#include <stdio.h>

#include "figure.h"
#include "geometry.h"

int main() {
	figure *l1, *l2, *p;

	l1 = figure_new_line_pp(0, 0, 0, 5);
	l2 = figure_new_line_pp(1, 1, 1, 6);

	p = figure_new_point(0, 0);

	gel_calculate_intersection(l1, l2, p);

	printf("%d\n", gel_lf_is_parallel(l1, l2));

	return 0;
}
