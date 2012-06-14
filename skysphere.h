#ifndef _SKYSPHERE_H_
#define _SKYSPHERE_H_

#include <oslib/oslib.h>

typedef struct sphere_vertex_
{
	float u,v;
	u32 color;
	float nx,ny,nz;
	float x,y,z;
} sphere_vertex_t;

typedef struct sphere_face_
{
	int rows;
	int cols;
	sphere_vertex_t* vertices;
} sphere_face_t;

typedef struct skysphere_
{
	OSL_IMAGE* images[6];
	sphere_face_t faces[6];

} skysphere_t;

skysphere_t* skysphere_create(int rows, int cols);
void skysphere_draw(skysphere_t* s);
void skysphere_destroy(skysphere_t* s);
#endif
