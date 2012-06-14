#include <oslib/oslib.h>
#include <psprtc.h>
#include <stdbool.h>
#include "skysphere.h"

#define DEFAULT_FOVX 90.0f
#define DEFAULT_ZFAR 1000.0f
#define DEFAULT_ZNEAR 0.1f
#define DEFAULT_SCREEN_WIDTH 480
#define DEFAULT_SCREEN_HEIGHT 272

PSP_MODULE_INFO("Skysphere", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12*1024);
u32 fps;
char fps_str[100];
float elapsed_sec;

SceKernelUtilsMt19937Context ctx;
ScePspFVector3 eye = { 0.0f, 0.0f, 5.0f };
ScePspFVector3 neg_eye = { 0.0f, 0.0f, -5.0f };

ScePspFVector3 origin = { 0.0f, 0.0f, 0.0f };
ScePspFVector3 up = { 0.0f, 1.0f, 0.0f };
ScePspFVector3 dir = { 0.0f, 0.0f, -1.0f };
ScePspFVector3 skybox_pos = { 0.0f, 0.0f, 0.0f };
ScePspFVector3 extents = { 4.0f, 4.0f, 4.0f };
ScePspFVector3 light_pos = { 0.0f, 2.0, 0.0f };
ScePspFMatrix4 proj;
ScePspFMatrix4 view;
ScePspFMatrix4 model;

float camera_rot_speed = 0.1f;

float rot = 0.01f;

int exit_callback(int arg1, int arg2, void *commmon)
{
	osl_quit = 1;
	return 0;
}

int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0 ,0);
	return thid;
}

int initOSLib()
{
	oslInit(0);
	oslInitGfx(OSL_PF_8888, 1);
	oslInitAudio();
	oslSetQuitOnLoadFailure(1);
	oslSetKeyAutorepeatInit(40);
	oslSetKeyAutorepeatInterval(10);
	sceKernelUtilsMt19937Init(&ctx, time(NULL));
	return 1;
}

void update_fps(float elapsed_sec)
{
	static float accum_sec = 0.0f;
	static int frames = 0;

	accum_sec += elapsed_sec;

	if(accum_sec >= 1.0f)
	{
		fps = frames;
		sprintf(fps_str, "FPS: %d", frames);
		frames = 0;
		accum_sec = 0.0f;
	}
	else
		++frames;
}

float delta_t()
{
	static u64 t_prev = 0;
	static u64 t_now = 0;
	static u32 t_res = 0;
	static float t_scale = 0.0f;
	float dt = 0.0f;
	static bool initialized = false;

	if(!initialized)
	{
		initialized = true;
		t_res = sceRtcGetTickResolution();
		sceRtcGetCurrentTick(&t_prev);
		t_scale = 1.0f / (float)t_res;
	}
	
	sceRtcGetCurrentTick(&t_now);
	dt = (t_now - t_prev) * t_scale;
	t_prev = t_now;
	return dt;
}

int rand_int(int low, int high)
{
	u32 rand_val = sceKernelUtilsMt19937UInt(&ctx);
	rand_val = low + rand_val % (high-low);
	return (int)rand_val;
}

void print_matrix4(int x, int y, ScePspFMatrix4 *m)
{
	oslDrawStringf(x, y, "%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
	m->x.x, m->x.y, m->x.z, m->x.w,
	m->y.x, m->y.y, m->y.z, m->y.w,
	m->z.x, m->z.y, m->z.z, m->z.w,
	m->w.x, m->w.y, m->w.z, m->w.w);
}

void gu_start(void)
{
	gumInit();
	sceGuDepthRange(65535,0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CCW);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuColorMaterial(GU_AMBIENT);
	sceGuEnable(GU_LIGHTING);
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(90.0f, (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT, 0.1f, 1000.0f);
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGuLight(0, GU_AMBIENT, GU_AMBIENT_AND_DIFFUSE, &light_pos);
	sceGuEnable(GU_LIGHT0);
	sceGuAmbient(0xffffffff);
	return;
}
void update_input(OSL_CONTROLLER *k)
{
		if(k->pressed.start)
			osl_quit = 1;
}

int main(void)
{
	int skip = 0;
	SetupCallbacks();
	initOSLib();


	oslIntraFontInit(INTRAFONT_CACHE_MED);
	OSL_FONT *font = oslLoadFontFile("flash0:/font/ltn0.pgf");
	oslSetFont(font);

	OSL_SOUND *mod = oslLoadSoundFileMOD("music/GSLINGER.MOD", OSL_FMT_NONE);
	skysphere_t *sky = skysphere_create(16, 16);

	gu_start();
	oslPlaySound(mod,1);
	OSL_CONTROLLER *k;
	oslSetFrameskip(1);
	oslSetMaxFrameskip(4);
	oslSetFramerate(60);

	while(!osl_quit)
	{
		if(!skip)
		{
			rot += 0.01f;
			elapsed_sec = delta_t();
			update_fps(elapsed_sec);
			k = oslReadKeys();
			update_input(k);
			
			oslStartDrawing();
			sceGuClearColor(0xFF000000);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();
			sceGumLookAt(&eye, &dir, &up);
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();


			sceGumRotateY(rot);
			sceGumScale(&extents);
			skysphere_draw(sky);

			oslDrawString(0, 3, fps_str);
			oslEndDrawing();
		}

		oslEndFrame();
		skip = oslSyncFrame();
	}

	skysphere_destroy(sky);
	oslDeleteSound(mod);
	oslDeleteFont(font);
	oslIntraFontShutdown();
	oslDeinitAudio();
	oslEndGfx();
	oslQuit();
	sceKernelExitGame();
	return 0;
}
