
#include "Everything_App.hpp"

#define DUMP_WIDTH (20)

void hex_dump(int level, struct frame *frm, int num)
{
	unsigned char *buf = frm->ptr;
	register int i, n;

	if ((num < 0) || (num > (int) frm->len))
		num = frm->len;

	for (i = 0, n = 1; i < num; i++, n++) {
		if (n == 1)
			p_indent(level, frm);
		printf("%2.2X ", buf[i]);
		if (n == DUMP_WIDTH) {
			printf("\n");
			n = 0;
		}
	}
	if (i && n != 1)
		printf("\n");
}

void raw_ndump(int level, struct frame *frm, int num)
{
	if (!frm->len)
		return;

	hex_dump(level, frm, num);
}

void raw_dump(int level, struct frame *frm)
{
	return;
	raw_ndump(level, frm, -1);
}

