#include <stdio.h>
#include <math.h>

#include "geometry.h"
#include "figure.h"
#include "binding.h"

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

	point->a1 = lx + point->x;
	point->a2 = ly + point->y;
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
	double a = 0, b = 0, c = 0, S = 0, hp = 0, h = 0;
	figure b_line, c_line;

	figure_fill(&b_line, p->x, p->y, l->a1, l->a2, FG_TYPE_LINE_PP);
	figure_fill(&c_line, p->x, p->y, l->x, l->y, FG_TYPE_LINE_PP);

	// sides of triangle
	a = gel_calculate_lenght(l);
	b = gel_calculate_lenght(&b_line);
	c = gel_calculate_lenght(&c_line);

	// helfperometr
	hp = (a + b + c) / 2;

	// square
	S = sqrt(hp * (hp - a)*(hp - b)*(hp - c));

	// height
	h = (2 * S) / a;
	printf("h = %g\tis_point_in_area = %d\n", h, gel_is_point_in_area(l, p));

	return (h <= BINDING_AREA) && (gel_is_point_in_area(l, p));
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
	if (fabs(p1->x - p2->x) <= BINDING_AREA && fabs(p1->y - p2->y <= BINDING_AREA))
		return 1;
	return 0;
}

double gel_calculate_line_angle(figure *l) {
	double dx, dy, k;

	dx = l->a1 - l->x;
	dy = l->a2 - l->y;

	if (!dx) {
		return (dy > 0) ? (90) : (-90);	
	}
	if (!dy && dx < 0) {
		return 180;	
	}

	k = dy / dx;

	printf("\tang = %g\n", atan(k));

	return gel_convert_rads_to_grades(atan(k));
}

double gel_calculate_heron_formula(double a, double b, double c) {
	double S, p;

	p = (a + b + c) / 2;
	S = sqrt(p * (p - a)*(p - b)*(p - b));

	return S;
}

figure *gel_get_center_point_by_hr(double xh, double yh, double R) {
	static figure cp;	

	if (xh - R > 0)
		cp.x = xh - R;
	else
		cp.x = xh + R;

	if (yh - R > 0)
		cp.y = yh - R;
	else
		cp.y = yh + R;

	cp.a1 = cp.a2 = 0;

	return &cp;
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

	printf("mb expr %g\nmb expr %g\n", (y3 - y2), (x3 - x2));

	xc = (ma * mb * (y1 - y3) + mb * (x1 + x2) - ma * (x2 + x3)) / (2 * (mb - ma));
	yc = -(1 / ma) * (xc - ((x1 + x2) / 2)) + ((y1 + y2 ) / 2);

	printf("C(%g %g)\n", xc, yc);
	printf("ma = %g\nmb = %g\n", ma, mb);

	figure_fill(&cp, xc, yc, 0, 0, FG_TYPE_POINT);

	return &cp;
}

int min(double v1, double v2) {
	return (v1 < v2) ? v1 : v2;
}

int max(double v1, double v2) {
	return (v1 > v2) ? v1 : v2;
}

int gel_is_point_in_area(figure *area, figure *p) {
	double x1, y1, x2, y2;

	x1 = area->x;
	y1 = area->y;

	x2 = area->a1;
	y2 = area->a2;

	if ((p->x >= min(x1, x2)) && (p->a1 <= max(x1, x2)))
		if ((p->y >= min(y1, y2)) && (p->a2 <= max(y1, y2)))
			return 1;
	return 0;
}
