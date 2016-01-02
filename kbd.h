#ifndef _KBD_H_
#define _KBD_H_
#include "kfifo.h"

#define KBD_FS 28

extern struct kfifo kfifo;

void kbd_init(int, int, int, int);
void kbd_draw();
void kbd_push(int, int);
#endif /*_KBD_H_*/
