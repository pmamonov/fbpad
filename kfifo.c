#include <stdlib.h>
#include <string.h>
#include "kfifo.h"

void kfifo_init(struct kfifo *kfifo, int size)
{
	kfifo->in = 0;
	kfifo->out = 0;
	kfifo->len = 0;
	kfifo->size = size;
	kfifo->buf = malloc(size * sizeof(int));
	pthread_mutex_init(&kfifo->lock, 0);
}

void kfifo_putchar(struct kfifo *kfifo, int c)
{
	pthread_mutex_lock(&kfifo->lock);
	if (kfifo->len >= kfifo->size)
		return;
	kfifo->buf[kfifo->in] = c;
	kfifo->in = (kfifo->in + 1) % kfifo->size;
	kfifo->len++;
	pthread_mutex_unlock(&kfifo->lock);
}

int kfifo_getchar(struct kfifo *kfifo)
{
	int ret;

	pthread_mutex_lock(&kfifo->lock);
	if (kfifo->len <= 0)
		return -1;
	ret = kfifo->buf[kfifo->out];
	kfifo->out = (kfifo->out + 1) % kfifo->size;
	kfifo->len--;
	pthread_mutex_unlock(&kfifo->lock);

	return ret;
}

int kfifo_poll(struct kfifo *kfifo)
{
	int ret;

	pthread_mutex_lock(&kfifo->lock);
	ret = kfifo->len;
	pthread_mutex_unlock(&kfifo->lock);

	return ret;
}

void kfifo_puts(struct kfifo *kfifo, char *s)
{
	int i, l;

	if (!s)
		return;
	pthread_mutex_lock(&kfifo->lock);
	l = strlen(s);
	for (i = 0; i < l; i++) {
		if (kfifo->len >= kfifo->size)
			break;
		kfifo->buf[kfifo->in] = s[i];
		kfifo->in = (kfifo->in + 1) % kfifo->size;
		kfifo->len++;
	}
	pthread_mutex_unlock(&kfifo->lock);
}

void kfifo_free(struct kfifo *kfifo)
{
	free(kfifo->buf);
}
