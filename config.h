/* framebuffer device */
#define FBDEV		"/dev/fb0"

/* list of tags */
#define TAGS		"xnlhtr01uiva-"
#define TAGS_SAVED	""

/* programs mapped to m-c, m-m, m-e */
#define SHELL		{"sh"}
#define EDITOR		{"vi"}
#define MAIL		{"mailx", "-f", "+inbox"}

/* fbval_t should match framebuffer depth */
typedef unsigned int fbval_t;

/* tinyfont files for regular, italic, and bold fonts */
#define FR		"/path/to/font.tf"
#define FI		NULL
#define FB		NULL

/* alternative fonts */
#define FR0		"/path/to/font.tf"
#define FI0		NULL
#define FB0		NULL

/* foreground and background colors */
#define FGCOLOR		0
#define BGCOLOR		15

/* where to write the screen shot */
#define SCRSHOT		"/tmp/scr"

/* lock command password; NULL disables locking */
#define PASS		NULL

/* optimized version of fb_val() */
#define FB_VAL(r, g, b)	fb_val((r), (g), (b))

/* black */
#define COLOR0		0x000000
#define COLOR8		0x555555
/* red */
#define COLOR1		0xaa0000
#define COLOR9		0xff5555
/* green */
#define COLOR2		0x00aa00
#define COLOR10		0x55ff55
/* yellow */
#define COLOR3		0xaa5500
#define COLOR11		0xffff55
/* blue */
#define COLOR4		0x0000aa
#define COLOR12		0x5555ff
/* magenta */
#define COLOR5		0xaa00aa
#define COLOR13		0xff55ff
/* cyan */
#define COLOR6		0x00aaaa
#define COLOR14		0x55ffff
/* white */
#define COLOR7		0xaaaaaa
#define COLOR15		0xffffff
