#include <inkview.h>
#include "kbd.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

enum kmode {
	KMOD_NORM = 0,
	KMOD_SHFT,
	KMOD_CTRL,
	KMOD_CTRL_SHFT,
	KBD_NUM_STATES,
	KMOD_NEW_ROW = -1,
};

struct kfifo kfifo;

static int ktop, kbottom, kleft, kright, kheight, kwidth;
enum kmode kmode;

struct kbutton_state {
	char *sym;
	char *seq;
};

struct kbutton {
	int left, top, right, bottom;
	enum kmode mode;
	struct kbutton_state bs[KBD_NUM_STATES];
};

struct kbutton buttons[] = {
	{.bs = {{"`","`"}, {"~", "~"}}},
	{.bs = {{"1","1"}, {"!", "!"}}},
	{.bs = {{"2","2"}, {"@", "@"}}},
	{.bs = {{"3","3"}, {"#", "#"}}},
	{.bs = {{"4","4"}, {"$", "$"}}},
	{.bs = {{"5","5"}, {"%", "%"}}},
	{.bs = {{"6","6"}, {"^", "^"}}},
	{.bs = {{"7","7"}, {"&", "&"}}},
	{.bs = {{"8","8"}, {"*", "*"}}},
	{.bs = {{"9","9"}, {"(", "("}}},
	{.bs = {{"0","0"}, {")", ")"}}},
	{.bs = {{"-","-"}, {"_", "_"}}},
	{.bs = {{"=","="}, {"+", "+"}}},
	{.bs = {{"BS", "\x8"},}},
	{.bs = {{"ES", "\x1b"},}},
	{.mode = KMOD_NEW_ROW},
	{.bs = {{"q","q"}, {"Q", "Q"}}},
	{.bs = {{"w","w"}, {"W", "W"}, {"^W", "\x17"}}},
	{.bs = {{"e","e"}, {"E", "E"}}},
	{.bs = {{"r","r"}, {"R", "R"}}},
	{.bs = {{"t","t"}, {"T", "T"}}},
	{.bs = {{"y","y"}, {"Y", "Y"}}},
	{.bs = {{"u","u"}, {"U", "U"}}},
	{.bs = {{"i","i"}, {"I", "I"}}},
	{.bs = {{"o","o"}, {"O", "O"}}},
	{.bs = {{"p","p"}, {"P", "P"}}},
	{.bs = {{"[","["}, {"{", "{"}, {"H", "\x1b[1~"}}},
	{.bs = {{"]","]"}, {"}", "}"}, {"E", "\x1b[4~"}}},
	{.bs = {{"RT ","\r"},}},
	{.mode = KMOD_NEW_ROW},
	{.bs = {{"a","a"}, {"A", "A"}}},
	{.bs = {{"s","s"}, {"S", "S"}}},
	{.bs = {{"d","d"}, {"D", "D"}}},
	{.bs = {{"f","f"}, {"F", "F"}}},
	{.bs = {{"g","g"}, {"G", "G"}}},
	{.bs = {{"h","h"}, {"H", "H"}}},
	{.bs = {{"j","j"}, {"J", "J"}}},
	{.bs = {{"k","k"}, {"K", "K"}}},
	{.bs = {{"l","l"}, {"L", "L"}}},
	{.bs = {{";",";"}, {":", ":"}}},
	{.bs = {{"'","'"}, {"\"", "\""}}},
	{.bs = {{"\\","\\"}, {"|", "|"}}},
	{.bs = {{"SH", ""},}, .mode = KMOD_SHFT},
	{.bs = {{"TB", "\t"}}},
	{.mode = KMOD_NEW_ROW},
	{.bs = {{"z","z"}, {"Z", "Z"}, {"^Z", "\x1a"}}},
	{.bs = {{"x","x"}, {"X", "X"}}},
	{.bs = {{"c","c"}, {"C", "C"}, {"^C", "\x03"}}},
	{.bs = {{"v","v"}, {"V", "V"}}},
	{.bs = {{"b","b"}, {"B", "B"}}},
	{.bs = {{"n","n"}, {"N", "N"}}},
	{.bs = {{"m","m"}, {"M", "M"}}},
	{.bs = {{",",","}, {"<", "<"}}},
	{.bs = {{".","."}, {">", ">"}}},
	{.bs = {{"/","/"}, {"?", "?"}}},
	{.bs = {{"SP"," "}}},
	{.bs = {{"CT", ""},}, .mode = KMOD_CTRL},
	{.bs = {{"<","\x1b[D"}}},
	{.bs = {{">","\x1b[C"}}},
	{.bs = {{"^","\x1b[A"}}},
	{.bs = {{"v","\x1b[B"}}},
};

void kbd_init(int l, int t, int r, int b)
{
	ifont *kfont;
	
	kmode = KMOD_NORM;
	kleft = l;
	ktop = t;
	kright = r;
	kbottom = b;
	kheight = b - t;
	kwidth = r - l;

	kfifo_init(&kfifo, 100);

	kfont = OpenFont("mono", KBD_FS, 0);
	SetFont(kfont, BLACK);
}

static struct kbutton_state *get_bs(struct kbutton *b)
{
	if (kmode >= KBD_NUM_STATES)
		return NULL;
	if (b->bs[kmode].sym && b->bs[kmode].seq)
		return &b->bs[kmode];
	if (b->bs[0].sym && b->bs[0].seq)
		return &b->bs[0];
	return NULL;
}

static void draw_button(struct kbutton_state *bs, int x, int y, int w, int h)
{
		DrawRect(x, y, w, h, DGRAY);
		DrawString(x, y, bs->sym);
}

void kbd_draw()
{
	int i, x, y, dx, dy, w, nr, r;
	struct kbutton_state *bs;

	FillArea(kleft, ktop, kwidth, kheight, WHITE);

	nr = 1;
	for (i = 0; i < ARRAY_SIZE(buttons); i++)
		if (buttons[i].mode == KMOD_NEW_ROW)
			nr++;
	dy = kheight / nr;
	
	r = 0;
	w = 0;
	x = kleft;
	y = ktop;
	for (i = 0; i < ARRAY_SIZE(buttons); i++) {
		bs = get_bs(&buttons[i]);
		if (bs)
			w += strlen(bs->sym);
		if (buttons[i].mode == KMOD_NEW_ROW ||
		    i + 1 >= ARRAY_SIZE(buttons)) {
			dx = kwidth / w;
			while (r <= i) {
				bs = get_bs(&buttons[r]);
				if (bs) {
					w = dx * strlen(bs->sym);
					buttons[r].left = x;
					buttons[r].top = y;
					buttons[r].right = x + w;
					buttons[r].bottom = y + dy;
					draw_button(bs, x, y, w, dy);
					x += w;
				}
				r++;
			}
			y += dy;
			w = 0;
			x = kleft;
		}
	}
}

void kbd_push(int x, int y)
{
	int i;
	struct kbutton *b;
	struct kbutton_state *bs;

	for (i = 0; i < ARRAY_SIZE(buttons); i++) {
		b = &buttons[i];
		if (x >= b->left && x < b->right &&
		    y >= b->top && y < b->bottom) {
		    	if (b->mode) {
				if (kmode == b->mode)
					kmode = KMOD_NORM;
				else
					kmode = b->mode;
			} else {
				bs = get_bs(b);
				if (bs)
					kfifo_puts(&kfifo, bs->seq);
			}
			break;
		}
	}
}
