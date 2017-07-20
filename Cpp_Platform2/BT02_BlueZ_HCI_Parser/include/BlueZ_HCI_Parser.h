

#ifndef _BLUEZ_HCI_PARSER_HPP_INCLUDED_

#include "BZ_bluetooth.h"
#include "BZ_util.h"
#include <winsock2.h>
#include "hci.h"

struct frame {
	void		*data;
	uint32_t	data_len;
	uint8_t		*ptr;
	uint32_t	len;

	uint16_t	dev_id;
	uint8_t		in;
	uint8_t		master;
	uint16_t	handle;
	uint16_t	cid;
	uint16_t	num;
	uint8_t		dlci;
	uint8_t		channel;
	unsigned long	flags;
//	struct timeval	ts;
	int		pppdump_fd;
	int		audio_fd;

};
#include "BZ_porting.h"

#define HCI_COMMAND_PKT		0x01
#define HCI_ACLDATA_PKT		0x02
#define HCI_SCODATA_PKT		0x03
#define HCI_EVENT_PKT		0x04
#define HCI_VENDOR_PKT		0xff

/* Byte order conversions */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define htobs(d)  (d)
#define htobl(d)  (d)
#define htobll(d) (d)
#define btohs(d)  (d)
#define btohl(d)  (d)
#define btohll(d) (d)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define htobs(d)  bswap_16(d)
#define htobl(d)  bswap_32(d)
#define htobll(d) bswap_64(d)
#define btohs(d)  bswap_16(d)
#define btohl(d)  bswap_32(d)
#define btohll(d) bswap_64(d)
#else
#error "Unknown byte order"
#endif

void hci_dump(int level, struct frame *frm);

int BlueZ_HCI_Parser(void);


#define _BLUEZ_HCI_PARSER_HPP_INCLUDED_
#endif//_BLUEZ_HCI_PARSER_HPP_INCLUDED_



