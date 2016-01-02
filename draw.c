#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <inkview.h>
#include "draw.h"

#undef DEBUG
#include "debug.h"

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define NLEVELS		(1 << 8)

static icanvas *screen;
static int ori;
pthread_t t;

static int inkview_handler(int type, int par1, int par2)
{

	switch (type) {	
	case EVT_SHOW:
		FullUpdate();
		break;
	case EVT_POINTERDOWN:
		dmsg("POINTERDOWN %d %d", par1, par2);
		break;
	case EVT_KEYPRESS:
		SetOrientation(ori);
		CloseApp();
		break;
	case EVT_ORIENTATION:
		break;
	}
	return 0;

}

void *tfunc(void)
{
	InkViewMain(inkview_handler);
}

void fb_cmap(void)
{
	return;
}

unsigned fb_mode(void)
{
	return 0;
}

int fb_init(char *dev)
{
	OpenScreen();
	dmsg("hello");
	ori = GetOrientation();
	/* SetOrientation(ROTATE90); */
	screen = GetCanvas();
	dmsg("canvas: w=%d h=%d d=%d",
	     screen->width,
	     screen->height,
	     screen->depth);
	pthread_create(&t, NULL, tfunc, NULL);
	return 0;
}

void fb_free(void)
{
	return;
}

int fb_rows(void)
{
	return screen->height;
}

int fb_cols(void)
{
	return screen->width;
}

void *fb_mem(int r)
{
	return screen->addr + r * screen->width;
}

unsigned fb_val(int r, int g, int b)
{
	return (r + g + b) / 3;
}
