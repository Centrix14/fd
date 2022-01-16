#include <stdio.h>

#include "cmodel/cmodel.c"

void init(list *msg_list) {
	pl_set_msg_list(msg_list);
}

void model() {
	int n = 6, i = 0;
	double angle = 0, r = 100, a = 0;
	double vertexes[] = {0, 0, 0, 0};
	double x = 0, y = 0;
	figure *line = NULL;
	char *prm = NULL;

	prm = get_param("Enter the number of sides and coordinates of the rectangle (n x y)");
	sscanf(prm, "%d %lf %lf", &n, &x, &y);

	angle = 360 / n;
	a = 0;
	for (int i = 0; i < n+1; i++) {
		line = line_ar(x, y, a, r);

		vertexes[0] = vertexes[2];
		vertexes[1] = vertexes[3];
		vertexes[2] = line->a1;
		vertexes[3] = line->a2;

		if (vertexes[0] && vertexes[1] && vertexes[2] && vertexes[3])
			line_xy(vertexes[0], vertexes[1], vertexes[2], vertexes[3]);
		erase(line, OT_FIGURE);

		a += angle;
	}
}
