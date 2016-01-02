#ifndef _DEBUG_H_
#define _DEBUG_H_
#ifdef DEBUG
#define MSGBUFLEN 1024
static void dmsg(char *fmt, ...)
{
	char msgbuf[MSGBUFLEN];
	va_list ap;

	va_start(ap, fmt);
    	vsnprintf(msgbuf, MSGBUFLEN, fmt, ap);
	va_end(ap);
    	Message(0, "debug", msgbuf, 1000);
}
#else /* DEBUG */
#define dmsg(ftm, ...)
#endif /* DEBUG */
#endif /* _DEBUG_H_*/
