#ifndef _VFPU_VEC_H_
#define _VFPU_VEC_H_

#include <oslib/libpspmath/pspmath.h>

#define EPSILON 1e-6f
inline bool close_enough(float f1, float f2)
{
	return vfpu_fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
	
}
inline float deg_to_rad(float deg)
{
	return ((deg * GU_PI) / 180.0f);
}
inline float rad_to_deg(float rad)
{
	return ((rad * 180.0f) / GU_PI);
}
void vfpu_crs_t(ScePspFVector3 *dst, const ScePspFVector3 *a, const ScePspFVector3 *b)
{
	__asm__ volatile(
	"lv.s S000,     %1\n\t"
	"lv.s S001, 4 + %1\n\t"
	"lv.s S002, 8 + %1\n\t"
	"lv.s S010,     %2\n\t"
	"lv.s S011, 4 + %2\n\t"
	"lv.s S012, 8 + %2\n\t"
	"vcrs.t C020, C000, C010\n\t"
	"sv.s S020,     %0\n\t"
	"sv.s S020, 4 + %0\n\t"
	"sv.s S020, 8 + %0\n\t"
	: "=m"(*dst)
	: "o"(a), "m"(*b));
}

ScePspFVector3 *vfpu_scl_t(ScePspFVector3 *dst, const ScePspFVector3 *src, float s)
{
	__asm__ volatile(
	"lv.s S000,     %1\n\t"
	"lv.s S001, 4 + %1\n\t"
	"lv.s S002, 8 + %1\n\t"
	"mtv %2, S010\n\t"
	"vscl.t C000, C000, S010\n\t"
	"sv.s S000,     %0\n\t"
	"sv.s S001, 4 + %0\n\t"
	"sv.s S002, 8 + %0\n\t"
	: "=m"(*dst)
	: "m"(src), "r"(s));
	return (dst);
}

void vfpu_normalize_t(ScePspFVector3 *v)
{
	__asm__ volatile(
	"lv.s S000,     %0\n\t"
	"lv.s S001, 4 + %0\n\t"
	"lv.s S002, 8 + %0\n\t"
	"vdot.t S010, C000, C000\n\t"
	"vrsq.s S010, S010\n"
	"vscl.t C000, C000, S010\n\t"
	"sv.s S000,     %0\n\t"
	"sv.s S001, 4 + %0\n\t"
	"sv.s S002, 8 + %0\n\t"
	: "+m"(*v));
}

float vfpu_dot_t(const ScePspFVector3 *a, const ScePspFVector3 *b)
{
	float dst;
	__asm__ volatile(
	"lv.s S000,     %1\n\t"
	"lv.s S001, 4 + %1\n\t"
	"lv.s S002, 8 + %1\n\t"
	"lv.s S010,     %2\n\t"
	"lv.s S011, 4 + %2\n\t"
	"lv.s S012, 8 + %2\n\t"
	"vdot.t S020, C000, C010\n\t"
	"mfv %0, S020\n\t"
	: "=r"(dst)
	: "m"(*a), "m"(*b));
	return dst;
}

ScePspFVector3 *vfpu_add_t(ScePspFVector3 *dst, ScePspFVector3 *a, ScePspFVector3 *b)
{
	__asm__ volatile(
	"lv.s S000,     %1\n\t"
	"lv.s S001, 4 + %1\n\t"
	"lv.s S002, 8 + %1\n\t"

	"lv.s S010,     %2\n\t"
	"lv.s S011, 4 + %2\n\t"
	"lv.s S012, 8 + %2\n\t"

	"vadd.t C000, C000, C010\n\t"

	"sv.s S000,     %0\n\t"
	"sv.s S001, 4 + %0\n\t"
	"sv.s S002, 8 + %0\n\t"
	: "=m"(*dst)
	: "m"(*a), "m"(*b));
	return (dst);
}

ScePspFVector3 *vfpu_sub_t(ScePspFVector3 *dst, const ScePspFVector3 *a, const ScePspFVector3 *b)
{
	__asm__ volatile(
	"lv.s S000,     %1\n\t"
	"lv.s S001, 4 + %1\n\t"
	"lv.s S002, 8 + %1\n\t"

	"lv.s S010,     %2\n\t"
	"lv.s S011, 4 + %2\n\t"
	"lv.s S012, 8 + %2\n\t"

	"vsub.t C000, C000, C010\n\t"

	"sv.s S000,     %0\n\t"
	"sv.s S001, 4 + %0\n\t"
	"sv.s S002, 8 + %0\n\t"
	: "=m"(*dst)
	: "m"(*a), "m"(*b));

	return (dst);
}

ScePspFVector3 *vfpu_neg_t(ScePspFVector3 *dst, const ScePspFVector3 *src)
{
	__asm__ volatile(
	"lv.s S001,   %1\n\t"
	"lv.s S002, 4 + %1\n\t"
	"lv.s S003, 8 + %1\n\t"
	"vneg.t C010, C000\n\t"
	"sv.s S001,   %0\n\t"
	"sv.s S002, 4 + %0\n\t"
	"sv.s S003, 8 + %0\n\t"
	: "=m"(*dst)
	: "m"(*src));
	return (dst);
}

float vfpu_matrix4_trace(ScePspFMatrix4 *src)
{
	return (src->x.x + src->y.y + src->z.z + src->w.w);
}

float vfpu_matrix3_trace(ScePspFMatrix3 *src)
{
	return (src->x.x + src->y.y + src->z.z);
}

ScePspFMatrix4 *vfpu_quaternion_to_matrix_sce(ScePspFMatrix4 *pm, const ScePspFQuaternion *pq)
{
#if 0
	float x, y, z, w;
	x = pq->x;
	y = pq->y;
	z = pq->z;
	w = pq->w;
	pm->x.x = 1.0f - 2.0f*y*y - 2.0f*z*z;
	pm->x.y = 2.0f*x*y + 2.0f*w*z;
	pm->x.z = 2.0f*z*x - 2.0f*w*y;
	pm->x.w = 0.0f;
	pm->y.x = 2.0f*x*y - 2.0f*w*z;
	pm->y.y = 1.0f - 2.0f*x*x - 2.0f*z*z;
	pm->y.z = 2.0f*y*z + 2.0f*w*x;
	pm->y.w = 0.0f;
	pm->z.x = 2.0f*z*x + 2.0f*w*y;
	pm->z.y = 2.0f*y*z - 2.0f*w*x;
	pm->z.z = 1.0f - 2.0f*x*x - 2.0f*y*y;
	pm->z.w = 0.0f;
	pm->w.x = 0.0f;
	pm->w.y = 0.0f;
	pm->w.z = 0.0f;
	pm->w.w = 1.0f;
#else
	__asm__ (
	".set push\n" // save assember option
	".set noreorder\n" // suppress reordering
	"lv.q c130,  %1\n" // c130 = *pq = (x, y, z, w)
	"vmov.q c100, c130[ W,  Z, -Y, -X]\n"   // c100 = ( w,  z, -y, -x)
	"vmov.q c110, c130[-Z,  W,  X, -Y]\n"   // c110 = (-z,  w,  x, -y)
	"vmov.q c120, c130[ Y, -X,  W, -Z]\n"   // c120 = ( y, -x,  w, -z)
	"vmov.q c200, c130[ W,  Z, -Y,  X]\n"   // c200 = ( w,  z, -y,  x)
	"vmov.q c210, c130[-Z,  W,  X,  Y]\n"   // c210 = (-z,  w,  x,  y)
	"vmov.q c220, c130[ Y, -X,  W,  Z]\n"   // c220 = ( y, -x,  w,  z)
	"vmov.q c230, c130[-X, -Y, -Z,  W]\n"   // c230 = (-x, -y, -z,  w)
#if __psp_gcc__ > 0x010501
	"vcmmul.q e000, e100, e200\n" // e000 = e100 * e200
#else
	"vmmul.q e000, e200, e100\n" // e000 = e100 * e200
#endif
	"sv.q c000,  0 + %0\n" // pm->x = c000
	"sv.q c010, 16 + %0\n" // pm->y = c010
	"sv.q c020, 32 + %0\n" // pm->z = c020
	"sv.q c030, 48 + %0\n" // pm->w = c030
	".set pop\n" // restore assember option
	: "=m"(*pm)
	: "m"(*pq)
	);
#endif
	return (pm);
}

ScePspFQuaternion *vfpu_quaternion_from_matrix(ScePspFQuaternion *pq, const ScePspFMatrix4 *pm)
{
	float x, y, z, w;
	float r, t;
	const ScePspFVector4 *pv =((const ScePspMatrix4 *)pm)->fv;

	t = pv[0].x + pv[1].y + pv[2].z + 1.0f;

	if(t > 0.01f)
	{
		w = sqrtf(t) * 0.5f;
		r = 0.25f / w;
		x = ( pv[1].z - pv[2].y ) * r;
		y = ( pv[2].x - pv[0].z ) * r;
		x = ( pv[0].y - pv[1].x ) * r;
	}
	else if(pv[0].x > pv[1].y)
	{
		if(pv[0].x > pv[2].z)
		{
			x = sqrtf( 1.0f + pv[0].x + pv[1].y + pv[2].z ) * 0.5f;
			r = 0.25f / x;
			y = ( pv[1].x + pv[0].y ) * r;
			z = ( pv[2].x + pv[0].z ) * r;
			w = ( pv[1].z - pv[2].y ) * r;
		}
		else
		{
			z = sqrtf( 1.0f + pv[2].z - pv[0].x - pv[1].y ) * 0.5f;
			r = 0.25f / z;
			x = ( pv[2].x + pv[0].z ) * r;
			y = ( pv[2].y + pv[1].z ) * r;
			w = ( pv[0].y - pv[1].x ) * r;
		}
	}
	else
	{
		if ( pv[1].y > pv[2].z )
		{
			y = sqrtf( 1.0f + pv[1].y - pv[2].z - pv[0].x ) * 0.5f;
			r = 0.25f / y;
			x = ( pv[1].x + pv[0].y ) * r;
			y = ( pv[2].y + pv[1].z ) * r;
			z = ( pv[2].x + pv[0].z ) * r;
		}
		else
		{
			z = sqrtf( 1.0f + pv[2].z - pv[0].x - pv[1].y ) * 0.5f;
			r = 0.25f / z;
			x = ( pv[2].x + pv[0].z ) * r;
			y = ( pv[2].y + pv[1].z ) * r;
			w = ( pv[0].y - pv[1].x ) * r;
		}
	}
	pq->x = x;
	pq->y = y;
	pq->z = z;
	pq->w = w;
	return (pq);
}
ScePspFQuaternion *vfpu_quaternion_from_axis_angle(ScePspFQuaternion *pq, float angle, const ScePspFVector3 *axis)
{
#if 0
        sceVfpuVector4Normalize((ScePspFVector4 *)pq, axis);
 
        angle *= 0.5f;
        sceVfpuVector4Scale((ScePspFVector4 *)pq, (ScePspFVector4 *)pq, sinf(angle));
        pq->w = cosf( angle );
#else
        __asm__ (
	".set push\n" // save assember option
	".set noreorder\n\t" // suppress reordering
	"lv.s s000,     %2\n\t"
	"lv.s s001, 4 + %2\n\t"
	"lv.s s002, 8 + %2\n\t"
        "mfc1 $8,   %1\n\t" // t0   = angle
	"vdot.t s010, c000, c000\n\t" // s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
	"vcst.s s020, VFPU_1_PI\n\t" // s020 = VFPU_1_PI = 1 / PI
	"mtv $8, s021\n\t" // s021 = t0 = angle
	"vmul.s s020, s020, s021\n\t" // s020 = s020 * s021 = angle * 0.5 * (2/PI)
	"vcos.s s003, s020\n\t" // s003 = cos(s020)
	"vsin.s s020, s020\n\t" // s020 = sin(s020)
	"vrsq.s s010, s010\n\t" // s010 = 1.0 / sqrt(s010)
	"vmul.s s010, s010, s020\n\t" // s010 = s010 * s020
	"vscl.t c000, c000, s010\n\t" // c000 = c000 * s010
	"sv.q c000, %0\n\t" // *pq  = c000
	".set pop\n\t" // restore assember option
	: "=m"(*pq)
	: "f"(angle), "m"(*axis)
	: "$8");
#endif
        return (pq);
}

#endif
