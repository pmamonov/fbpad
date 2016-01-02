#include <inkview.h>
/* fbpad's framebuffer interface */
/* fb_mode() interpretation */
#define FBM_BPP(m)	1	/* bytes per pixel (4 bits) */

/* main functions */
int fb_init(char *dev);
void fb_free(void);
unsigned fb_mode(void);
void *fb_mem(int r);
int fb_rows(void);
int fb_cols(void);
void fb_cmap(void);
unsigned fb_val(int r, int g, int b);
