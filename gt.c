#include <stdio.h>

#include "figure.h"
#include "geometry.h"

int main() {
	figure *l1;

	l1 = figure_new_line_pp(5, 5, 0, 0);

	gel_calculate_point(l1, 5, 45);

	printf("%f %f\n", l1->a1, l1->a2);

	return 0;
}
