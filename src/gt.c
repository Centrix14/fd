#include <stdio.h>

#include "geometry.h"
#include "figure.h"

int main() {
	figure *l1;

	l1 = figure_new_line_pp(0, 0, -5, -5);

	printf("%s: %g\n", __func__, gel_calculate_line_angle(l1));
	return 0;
}
