#include <stdio.h>
#include <math.h>

#include "geometry.h"
#include "../figure/figure.h"
#include "../binding/binding.h"
#include "../ccVector/ccVector.h"
#include "../st.h/st.h"

st_debug_start(0);

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

		return "good";
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

	return "good";
}

void gel_calculate_line_la(figure *point, double lenght, double angle) {
	double lx, ly;

	ly = sin(gel_convert_grades_to_rads(angle)) * lenght;
	lx = sqrt(pow(lenght, 2) - pow(ly, 2));

	gel_convert_crd_by_angle(point, gel_convert_grades_to_rads(angle), lx, ly);
}

double gel_convert_grades_to_rads(double grades) {
	return grades * (PI / 180);
}

double gel_convert_rads_to_grades(double rads) {
	return rads * (180 / PI);
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

int gel_is_point_in_line(figure *l, figure *p) {
	double dx1, dy1, dx, dy, S, ab, h;

	dx1 = l->a1 - l->x;
	dy1 = l->a2 - l->y;

	dx = p->x - l->x;
	dy = p->y - l->y;

	S = dx1 * dy - dx * dy1;
	ab = sqrt(pow(dx1, 2) + pow(dy, 2));

	h = S / ab;
	if (fabs(h) < BINDING_AREA / 2)
		if (gel_is_point_in_area(l, p))
			return 1;
	return 0;
}

int gel_is_point_in_rect(figure *r, figure *p) {
	figure *lines;
	int res = 0;

	lines = figure_rect_decompose(r);

	for (int i = 0; i < 4; i++) {
		res += gel_is_point_in_line(&lines[i], p);
	}
	return res;
}

int gel_is_point_in_circle(figure *c, figure *p) {
	double x, y, xo, yo, R;
	int intern_circle = 0, ext_circle = 0;

	x = p->x;
	y = p->y;

	xo = c->x;
	yo = c->y;
	R = c->a1;

	intern_circle = (pow(x - xo, 2) + pow(y - yo, 2) - pow(R - BINDING_AREA, 2) > BINDING_AREA);
	ext_circle = (pow(x - xo, 2) + pow(y - yo, 2) - pow(R + BINDING_AREA, 2) < BINDING_AREA);

	if (intern_circle && ext_circle)
		return 1;
	return 0;
}

int gel_is_point_in_point(figure *p1, figure *p2) {
	if (fabs(p1->x - p2->x) <= BINDING_AREA && fabs(p1->y - p2->y) <= BINDING_AREA)
		return 1;
	return 0;
}

double gel_calculate_line_angle(figure *l) {
	double dy, R, A;

	dy = l->a2 - l->y;
	R = gel_calculate_lenght(l);

	A = asin(fabs(dy) / R);

	A = gel_convert_angle_by_crd(l, A);

	return gel_convert_rads_to_grades(A);
}

double gel_calculate_heron_formula(double a, double b, double c) {
	double S, p;

	p = (a + b + c) / 2;
	S = sqrt(p * (p - a)*(p - b)*(p - b));

	return S;
}

double gel_get_max_lenght(figure *a, figure *b, figure *c) {
	double al, bl, cl;

	al = gel_calculate_lenght(a);
	bl = gel_calculate_lenght(b);
	cl = gel_calculate_lenght(c);

	if (al > bl && al > cl)
		return al;
	else if (bl > al && bl > cl)
		return bl;
	else if (cl > al && cl > bl)
		return cl;
	return al;
}

// x1, y1, x2, y2 -- l1

figure *gel_get_arc_center(figure *l1, figure *l2) {
	double ma = 0, mb = 0, xc, yc;
	double x1, x2, x3, y1, y2, y3;
	static figure cp;

	x1 = l1->x;
	x2 = l1->a1;
	x3 = l2->a1;

	y1 = l1->y;
	y2 = l1->a2;
	y3 = l2->a2;

	ma = (y2 - y1) / (x2 - x1);
	mb = (y3 - y2) / (x3 - x2);

	st_debug{
		printf("mb expr %g\nmb expr %g\n", (y3 - y2), (x3 - x2));
	}

	xc = (ma * mb * (y1 - y3) + mb * (x1 + x2) - ma * (x2 + x3)) / (2 * (mb - ma));
	yc = -(1 / ma) * (xc - ((x1 + x2) / 2)) + ((y1 + y2 ) / 2);

	st_debug {
		printf("C(%g %g)\n", xc, yc);
		printf("ma = %g\nmb = %g\n", ma, mb);
	}

	figure_fill(&cp, xc, yc, 0, 0, FG_TYPE_POINT);

	return &cp;
}

double gel_min(double v1, double v2) {
	return (v1 < v2) ? v1 : v2;
}

double gel_max(double v1, double v2) {
	return (v1 > v2) ? v1 : v2;
}

int gel_is_point_in_area(figure *area, figure *p) {
	double x1, y1, x2, y2;

	x1 = area->x;
	y1 = area->y;

	x2 = area->a1;
	y2 = area->a2;

	if ((p->x >= gel_min(x1, x2)) && (p->a1 <= gel_max(x1, x2)))
		if ((p->y >= gel_min(y1, y2)) && (p->a2 <= gel_max(y1, y2)))
			return 1;
	return 0;
}

void gel_rotate_point(double *x, double *y, double angle) {
	vec2 point = {*x, *y}, result;
	mat2x2 transform;

	mat2x2Identity(transform);
	mat2x2Rotate(transform, angle);
	result = mat2x2MultiplyVector(transform, point);

	*x = result.x;
	*y = result.y;
}

double gel_convert_angle_by_crd(figure *l, double a) {
	double dx = 0, dy = 0;

	dx = l->a1 - l->x;
	dy = l->a2 - l->y;

	if (dx < 0) {
		a += PI / 2;
	}
	if (dy < 0) {
		a = -a;
	}

	st_debug {
		printf("%s: %g\n", __func__, a);
	}

	return a;
}

void gel_convert_crd_by_angle(figure *l, double a, double lx, double ly) {
	if (a <= (PI / 2)) {
		l->a1 = l->x + lx;
		l->a2 = l->y + ly;
	}
	else if (a <= PI) {
		l->a1 = l->x - fabs(lx);
		l->a2 = l->y + ly;
	}
	else if (a <= (3 * PI / 2)) {
		l->a1 = l->x - fabs(lx);
		l->a2 = l->y - fabs(ly);
	}
	else if (a <= (2 * PI)) {
		l->a1 = l->x + lx;
		l->a2 = l->y - fabs(ly);
	}
}
