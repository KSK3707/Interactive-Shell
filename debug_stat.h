#ifdef KDEBUG
#define myprintf(a) printf(a)
#else
#define myprintf
#endif

#ifndef __TRACER__
#define __TRACER__

#define PLACEMENT 1
#define TRAVERSE 2
#define SUCCESS 4

#define DEBUG_LEVEL (PLACEMENT)

#define kprintf(dbglvl, a) if(dbglvl&DEBUG_LEVEL) printf(a)

#endif
