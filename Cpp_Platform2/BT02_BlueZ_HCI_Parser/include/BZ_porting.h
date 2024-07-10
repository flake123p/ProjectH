
#ifndef __PORTING_H
#define __PORTING_H

struct parser_t {
	unsigned long flags;
	unsigned long filter;
	unsigned short defpsm;
	unsigned short defcompid;
	int state;
	int pppdump_fd;
	int audio_fd;
};

#define DUMP_NOVENDOR (0)
#define DUMP_VERBOSE (0)

#define p_filter(a) (0)
#define get_manufacturer(...) (-1)
#define ericsson_dump(...)
#define csr_dump(...)

#define l2cap_dump(...)
#define l2cap_clear(...)

void raw_dump(int level, struct frame *frm);

#endif //__PORTING_H
