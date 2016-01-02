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
