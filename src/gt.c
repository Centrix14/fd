#include <stdio.h>

#include "geometry.h"
#include "figure.h"

int main() {
	figure *l1;
	double a = 315;

	l1 = figure_new_line_pp(0, 0, 0, 0);
	gel_calculate_line_la(l1, 5, a);

	printf("%s: %g %g\t%g %g\n", __func__, l1->x, l1->y, l1->a1, l1->a2);
	return 0;
}
