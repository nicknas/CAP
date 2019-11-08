
#include <stdlib.h>
#include "nbody.h"

body* get_memory(int nBodies)
{
	body *p = (body*)malloc(sizeof(body));
	p->m = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	p->x = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	p->y = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	p->z = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	p->vx = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	p->vy = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	p->vz = (float*)_mm_malloc(sizeof(float) * nBodies, 32);
	return p;
}

void free_memory(body *p)
{
	_mm_free(p->m);
	_mm_free(p->x);
	_mm_free(p->y);
	_mm_free(p->z);
	_mm_free(p->vx);
	_mm_free(p->vy);
	_mm_free(p->vz);
	free(p);
}

void randomizeBodies(body *data, int n) {
	int i;
	for (i = 0; i < n; i++) {
		data->m[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;

		data->x[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->y[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->z[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;

		data->vx[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->vy[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->vz[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
	}
}

