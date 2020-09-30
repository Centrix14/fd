#include <stdio.h>

#include "geometry.h"
#include "figure.h"

void analise_input(figure *fptr) {
	switch (fptr->type) {
		case FG_TYPE_POINT:
			analise_input_point(fptr);
		break;

		case FG_TYPE_LINE_PP:
			analise_input_line(fptr);
		break;
	}
}
