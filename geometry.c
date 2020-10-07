#include <stdio.h>
#include <math.h>

#include "geometry.h"
#include "figure.h"

double gel_calculate_lenght(figure *line) {
	int lx, ly; // lenght-x, lenght-y
	double lenght;

	lx = line->a1 - line->x;
	ly = line->a2 - line->y;

	lenght = sqrt(pow(lx, 2) + pow(ly, 2));

	return lenght;
}

double gel_lf_calculate_k(figure *line) {
	int b = 0;

	if (line->x - line->a1 == 0) {
		b = gel_lf_calculate_b(line);

		return (line->a2 - b) / line->a1;
	}

	return (line->y - line->a2) / (line->x - line->a1);
}

double gel_lf_calculate_b(figure *line) {
	double k = 0;

	if (line->x - line->a1 == 0) {
		return (line->a2 - line->y * line->a1) / (1 - line->a1);
	}

	k = gel_lf_calculate_k(line);

	return line->a2 - k * line->a1;
}

int gel_lf_is_parallel(figure *line1, figure *line2) {
	double k1, k2, b1, b2;

	k1 = gel_lf_calculate_k(line1);
	k2 = gel_lf_calculate_k(line2);

	b1 = gel_lf_calculate_b(line1);
	b2 = gel_lf_calculate_b(line2);

	if (k1 == k2 && b1 != b2)
		return 1;
	return 0;
}

char *gel_calculate_intersection(figure *line1, figure *line2, figure *p) {
	double k1, k2, b1, b2;	
	double x = 0, y = 0;
	int is_parallel = 0;
	figure *equal_point;

	equal_point = gel_get_equal_point(line1, line2);
	if (equal_point) {
		p->x = equal_point->x;
		p->y = equal_point->y;

		return "";
	}

	is_parallel = gel_lf_is_parallel(line1, line2);
	if (is_parallel)
		return NULL;

	k1 = gel_lf_calculate_k(line1);
	k2 = gel_lf_calculate_k(line2);

	b1 = gel_lf_calculate_b(line1);
	b2 = gel_lf_calculate_b(line2);

	x = (b2 - b1) / (k1 - k2);
	y = k1 * x + b1;

	if (isnan(x) || isnan(y))
		return NULL;

	p->x = x;
	p->y = y;

	return "";
}

void gel_calculate_line_la(figure *point, double lenght, double angle) {
	double lx, ly;

	ly = sin(gel_convert_grades_to_rads(angle)) * lenght;
	lx = sqrt(pow(lenght, 2) - pow(ly, 2));

	point->a1 = lx + point->x;
	point->a2 = ly + point->y;
}

double gel_convert_grades_to_rads(double grades) {
	return grades * (PI / 180);
}

figure *gel_get_middle_point(figure *line) {
	static figure p;
	double dx, dy, px, py;

	dx = line->a1 - line->x;
	dy = line->a2 - line->y;

	px = dx / 2 + line->x;
	py = dy / 2 + line->y;

	figure_fill(&p, px, py, 0, 0, FG_TYPE_POINT);

	return &p;
}

int gel_is_middle_point_area(figure *line, double x, double y, int area) {
	figure *point;

	point = gel_get_middle_point(line);

	if (fabs(point->x - x) <= area && fabs(point->y - y) <= area)
		return 1;
	return 0;
}

figure *gel_get_equal_point(figure *line1, figure *line2) {
	static figure point;

	if (line1->x == line2->x && line1->y == line2->y) {
		point.x = line1->x;
		point.y = line2->y;

		return &point;
	}
	else if (line1->a1 == line2->a1 && line1->a2 == line2->a2) {
		point.x = line1->a1;
		point.y = line1->a2;

		return &point;
	}

	return NULL;
}

double gel_calculate_arc_lenght_g(double L, double l) {
	return 2 * l + (2 * l - L) / 3;
}

double gel_calculate_arc_angle_g(double lenght, double R) {
	return (180 * lenght) / (M_PI * R);
}
