#include <oslib/oslib.h>
#include "skysphere.h"
#include "vfpu_vec.h"

skysphere_t* skysphere_create(int rows, int cols)
{
	int i,r,c;
	skysphere_t* s = malloc(sizeof(skysphere_t));
	if(s == NULL)
		return NULL;
	for(i=0;i<6;i++)
	{
		s->faces[i].rows = rows;
		s->faces[i].cols = cols;
		s->faces[i].vertices = memalign(16, sizeof(sphere_vertex_t) * 6 * rows * cols);
	}

	s->images[0] = oslLoadImageFileJPG("images/pos_x.jpg", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	s->images[1] = oslLoadImageFileJPG("images/neg_x.jpg", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	s->images[2] = oslLoadImageFileJPG("images/pos_y.jpg", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	s->images[3] = oslLoadImageFileJPG("images/neg_y.jpg", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	s->images[4] = oslLoadImageFileJPG("images/pos_z.jpg", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	s->images[5] = oslLoadImageFileJPG("images/neg_z.jpg", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

	// pos_x
	i = 0;
	for(r=0; r < rows; r++)
	{
		float dr = (((float)r)/((float)rows));
		float dbeta = GU_PI/4.0f - (GU_PI/2.0f) * dr;
		float tan_dbeta = vfpu_tanf(dbeta);

		for(c=0;c < cols; c++)
		{
			float dc = (((float)c)/((float)cols));
			float dalpha = -GU_PI/4.0f + (GU_PI/2.0f) * dc;
			float tan_dalpha = vfpu_tanf(dalpha);
			sphere_vertex_t* v;

			// top left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 0]);
			v->color = RGB(255,0,0);
			v->x = 1.0f;
			v->y = tan_dbeta;
			v->z = tan_dalpha;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 1]);
			v->color = RGB(255,0,0);
			v->x = 1.0f;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = tan_dalpha;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 2]);
			v->color = RGB(255,0,0);
			v->x = 1.0f;
			v->y = tan_dbeta;
			v->z = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));


			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// second triangle
			// bottom right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 3]);
			v->color = RGB(255,0,0);
			v->x = 1.0f;;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 4]);
			v->color = RGB(255,0,0);
			v->x = 1.0f;
			v->y = tan_dbeta;
			v->z = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));
			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 5]);
			v->color = RGB(255,0,0);
			v->x = 1.0f;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = tan_dalpha;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);
		}
	}

	// neg_x
	i = 1;
	for(r=0; r < rows; r++)
	{
		float dr = (((float)r)/((float)rows));
		float dbeta = GU_PI/4.0f - (GU_PI/2.0f) * dr;
		float tan_dbeta = vfpu_tanf(dbeta);

		for(c=0;c < cols; c++)
		{
			float dc = (((float)c)/((float)cols));
			float dalpha = GU_PI/4.0f - (GU_PI/2.0f) * dc;
			float tan_dalpha = vfpu_tanf(dalpha);
			sphere_vertex_t* v;

			// top left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 0]);
			v->color = RGB(0,255,0);
			v->x = -1.0f;
			v->y = tan_dbeta;
			v->z = tan_dalpha;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 1]);
			v->color = RGB(0,255,0);
			v->x = -1.0f;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = tan_dalpha;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 2]);
			v->color = RGB(0,255,0);
			v->x = -1.0f;
			v->y = tan_dbeta;
			v->z = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));


			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// second triangle
			// bottom right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 3]);
			v->color = RGB(0,255,0);
			v->x = -1.0f;;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 4]);
			v->color = RGB(0,255,0);
			v->x = -1.0f;
			v->y = tan_dbeta;
			v->z = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));
			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 5]);
			v->color = RGB(0,255,0);
			v->x = -1.0f;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = tan_dalpha;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);
		}
	}

	// pos_y
	i = 2;
	for(r=0; r < rows; r++)
	{
		float dr = (((float)r)/((float)rows));
		float dbeta = -GU_PI/4.0f + (GU_PI/2.0f) * dr;
		float tan_dbeta = vfpu_tanf(dbeta);

		for(c=0;c < cols; c++)
		{
			float dc = (((float)c)/((float)cols));
			float dalpha = GU_PI/4.0f - (GU_PI/2.0f) * dc;
			float tan_dalpha = vfpu_tanf(dalpha);
			sphere_vertex_t* v;

			// top left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 0]);
			v->color = RGB(255,0,255);
			v->x = tan_dalpha;
			v->y = 1.0f;
			v->z = tan_dbeta;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 1]);
			v->color = RGB(255,0,255);
			v->x = tan_dalpha;
			v->y = 1.0f;
			v->z = vfpu_tanf(dbeta + (GU_PI/2.0f)/(float)rows);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 2]);
			v->color = RGB(255,0,255);
			v->x = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			v->y = 1.0f;
			v->z = tan_dbeta;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));


			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// second triangle
			// bottom right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 3]);
			v->color = RGB(255,0,255);
			v->x = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			v->y = 1.0f;
			v->z = vfpu_tanf(dbeta + (GU_PI/2.0f)/(float)rows);

			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 4]);
			v->color = RGB(255,0,255);
			v->x = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			v->y = 1.0f;
			v->z = tan_dbeta;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));
			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 5]);
			v->color = RGB(255,0,255);
			v->x = tan_dalpha;
			v->y = 1.0f;
			v->z = vfpu_tanf(dbeta + (GU_PI/2.0f)/(float)rows);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);
		}
	}

	// neg_y
	i = 3;
	for(r=0; r < rows; r++)
	{
		float dr = (((float)r)/((float)rows));
		float dbeta = -GU_PI/4.0f + (GU_PI/2.0f) * dr;
		float tan_dbeta = vfpu_tanf(dbeta);

		for(c=0;c < cols; c++)
		{
			float dc = (((float)c)/((float)cols));
			float dalpha = -GU_PI/4.0f + (GU_PI/2.0f) * dc;
			float tan_dalpha = vfpu_tanf(dalpha);
			sphere_vertex_t* v;

			// top left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 0]);
			v->color = RGB(255,255,0);
			v->x = tan_dalpha;
			v->y = -1.0f;
			v->z = tan_dbeta;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 1]);
			v->color = RGB(255,255,0);
			v->x = tan_dalpha;
			v->y = -1.0f;
			v->z = vfpu_tanf(dbeta + (GU_PI/2.0f)/(float)rows);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 2]);
			v->color = RGB(255,255,0);
			v->x = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			v->y = -1.0f;
			v->z = tan_dbeta;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));


			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// second triangle
			// bottom right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 3]);
			v->color = RGB(255,255,0);
			v->x = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			v->y = -1.0f;
			v->z = vfpu_tanf(dbeta + (GU_PI/2.0f)/(float)rows);

			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 4]);
			v->color = RGB(255,255,0);
			v->x = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			v->y = -1.0f;
			v->z = tan_dbeta;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));
			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 5]);
			v->color = RGB(255,255,0);
			v->x = tan_dalpha;
			v->y = -1.0f;
			v->z = vfpu_tanf(dbeta + (GU_PI/2.0f)/(float)rows);
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);
		}
	}

	// pos_z
	i = 4;
	for(r=0; r < rows; r++)
	{
		float dr = (((float)r)/((float)rows));
		float dbeta = GU_PI/4.0f - (GU_PI/2.0f) * dr;
		float tan_dbeta = vfpu_tanf(dbeta);

		for(c=0;c < cols; c++)
		{
			float dc = (((float)c)/((float)cols));
			float dalpha = GU_PI/4.0f - (GU_PI/2.0f) * dc;
			float tan_dalpha = vfpu_tanf(dalpha);
			sphere_vertex_t* v;

			// top left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 0]);
			v->color = RGB(0,255,255);
			v->x = tan_dalpha;
			v->y = tan_dbeta;
			v->z = 1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 1]);
			v->color = RGB(0,255,255);
			v->x = tan_dalpha;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = 1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 2]);
			v->color = RGB(0,255,255);
			v->x = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			v->y = tan_dbeta;
			v->z = 1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));


			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// second triangle
			// bottom right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 3]);
			v->color = RGB(0,255,255);
			v->x = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = 1.0f;

			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 4]);
			v->color = RGB(0,255,255);
			v->x = vfpu_tanf(dalpha - (GU_PI/2.0f)/(float)cols);
			v->y = tan_dbeta;
			v->z = 1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));
			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 5]);
			v->color = RGB(0,255,255);
			v->x = tan_dalpha;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = 1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);
		}
	}

	// neg_z
	i = 5;
	for(r=0; r < rows; r++)
	{
		float dr = (((float)r)/((float)rows));
		float dbeta = GU_PI/4.0f - (GU_PI/2.0f) * dr;
		float tan_dbeta = vfpu_tanf(dbeta);

		for(c=0;c < cols; c++)
		{
			float dc = (((float)c)/((float)cols));
			float dalpha = -GU_PI/4.0f + (GU_PI/2.0f) * dc;
			float tan_dalpha = vfpu_tanf(dalpha);
			sphere_vertex_t* v;

			// top left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 0]);
			v->color = RGB(0,0,255);
			v->x = tan_dalpha;
			v->y = tan_dbeta;
			v->z = -1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 1]);
			v->color = RGB(0,0,255);
			v->x = tan_dalpha;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = -1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 2]);
			v->color = RGB(0,0,255);
			v->x = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			v->y = tan_dbeta;
			v->z = -1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));


			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// second triangle
			// bottom right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 3]);
			v->color = RGB(0,0,255);
			v->x = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = -1.0f;

			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// top right
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 4]);
			v->color = RGB(0,0,255);
			v->x = vfpu_tanf(dalpha + (GU_PI/2.0f)/(float)cols);
			v->y = tan_dbeta;
			v->z = -1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));
			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);

			// bottom left
			v = &(s->faces[i].vertices[r * cols * 6 + c * 6 + 5]);
			v->color = RGB(0,0,255);
			v->x = tan_dalpha;
			v->y = vfpu_tanf(dbeta - (GU_PI/2.0f)/(float)rows);
			v->z = -1.0f;
			vfpu_normalize_t((ScePspFVector3*)&(v->x));

			vfpu_neg_t((ScePspFVector3*)&(v->nx), (ScePspFVector3*) &(v->x));
			v->u = atan2f(v->nx, v->nz) / (2.0f * GU_PI) + 0.5f;
			v->v = 1.0f - (vfpu_asinf(v->ny) / GU_PI + 0.5f);
		}
	}
	return s;
}

void skysphere_draw(skysphere_t* s)
{
	int i;

	OSL_IMAGE* img;

	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexMode(GU_PSM_8888, 0, 0, 1);
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	sceGuTexWrap(GU_CLAMP, GU_CLAMP);

	for(i=0;i < 6;i++)
	{
		img = s->images[i];
		sceGuTexImage(0, img->sysSizeX, img->sysSizeY, img->sysSizeX, (void*)img->data);
		int rows = s->faces[i].rows;
		int cols = s->faces[i].cols;
		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 6 * rows * cols, 0, &(s->faces[i].vertices[0]));
	}
}

void skysphere_destroy(skysphere_t* s)
{
	if(s != NULL)
	{
		int i;
		for(i = 0; i < 6; i++)
		{
			if(s->images[i] != NULL)
				oslDeleteImage(s->images[i]);
			if(s->faces[i].vertices != NULL)
				free(s->faces[i].vertices);
		}
		free(s);
	}
}
