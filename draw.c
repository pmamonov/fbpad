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
#include "kbd.h"

#undef DEBUG
#include "debug.h"

#define KBD_HEIGHT 200

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define NLEVELS		(1 << 8)

static icanvas *screen;
static pthread_t t;
static int ul, ur, ut, ub;

void fb_update_start()
{
	ul = screen->width;
	ur = 0;
	ut = screen->height;
	ub = 0;
}

void fb_update(int row, int col, int len)
{
	int t = row;
	int b = t + (col + len) / screen->width;
	int l = col + len < screen->width ? col : 0;
	int r = col + len < screen->width ? col + len : screen->width;

	if (l < ul)
		ul = l;
	if (r > ur)
		ur = r;
	if (t < ut)
		ut = t;
	if (b > ub)
		ub = b;
}

void fb_update_finish()
{
	if (ul >= ur ||
	    ut >= ub)
	    return;
	PartialUpdateBW(ul, ut, ur - ul, ub - ut);
}

static int inkview_handler(int type, int par1, int par2)
{
	switch (type) {	
	case EVT_SHOW:
		kbd_draw();
		FullUpdate();
		break;
	case EVT_POINTERDOWN:
		kbd_push(par1, par2);
		kbd_draw();
		SoftUpdate();
		break;
	case EVT_KEYPRESS:
		dmsg("keypress 0x%x", par1);
		switch (par1) {
		case KEY_MENU:
			kfifo_putchar(&kfifo, '\r');
			break;
		case KEY_PREV:
			kfifo_puts(&kfifo, "\x1b[A");
			break;
		case KEY_NEXT:
			kfifo_puts(&kfifo, "\x1b[B");
			break;
		default:
			break;
		}
		break;
	case EVT_ORIENTATION:
		break;
	}
	return 0;
}

void *tfunc(void *arg)
{
	InkViewMain(inkview_handler);
	return NULL;
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
	screen = GetCanvas();
	kbd_init(0, screen->height - KBD_HEIGHT, screen->width, screen->height);
	pthread_create(&t, NULL, tfunc, NULL);
	return 0;
}

void fb_free(void)
{
	return;
}

int fb_rows(void)
{
	return screen->height - KBD_HEIGHT;
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
