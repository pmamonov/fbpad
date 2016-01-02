#ifndef _KFIFO_H_
#define _KFIFO_H_

#include <pthread.h>

struct kfifo {
	int *buf;
	int in;
	int out;
	int len;
	int size;
	pthread_mutex_t lock;
};

void kfifo_init(struct kfifo *kfifo, int size);
void kfifo_putchar(struct kfifo *kfifo, int c);
void kfifo_puts(struct kfifo *kfifo, char *s);
int kfifo_getchar(struct kfifo *kfifo);
int kfifo_poll(struct kfifo *kfifo);
void kfifo_free(struct kfifo *kfifo);
#endif /*_KFIFO_H_ */
