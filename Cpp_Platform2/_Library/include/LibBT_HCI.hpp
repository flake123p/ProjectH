


#ifndef _LIB_BT_HCI_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
	HCI_Command_Packet,
	HCI_ACL_Packet,
	HCI_SCO_Packet,
	HCI_Event_Packet,
} HCI_PACKET_TYPE_t;

//UART HCI packet types
#define UART_HEADER_HCI_COMMAND (0x01)
#define UART_HEADER_HCI_ACL     (0x02)
#define UART_HEADER_HCI_SCO     (0x03)
#define UART_HEADER_HCI_EVENT   (0x04)
typedef struct {
	u8 uartHeader;
	u8 data[15];
} HCI_UART_HEADER_t;
#define HCI_UART_HEADER_SIZE (sizeof(HCI_UART_HEADER_t)-15)
#define HCI_UART_HEADER_SIZE_FOR_CHECK (1)

#if 0
typedef struct {
	u8 reserved[3];
	u8 packetType;
} UART_HCI_HEADER_t;
#define UART_HCI_HEADER_SIZE (sizeof(UART_HCI_HEADER_t)-3)
#define UART_HCI_HEADER_SIZE_FOR_CHECK (1)
#endif

typedef struct {
	u8 opcode_L;
	u8 opcode_H;
	u8 parameterLen;
	u8 parameter[5];
} HCI_COMMAND_PACKET_t;
#define HCI_COMMAND_PACKET_HEADER_SIZE (sizeof(HCI_COMMAND_PACKET_t)-5)
#define HCI_COMMAND_PACKET_HEADER_SIZE_FOR_CHECK (3)

typedef struct {
	u8 handel_L;
	u8 handel_H;
	u8 dataLen_L;
	u8 dataLen_H;
	u8 ACL_data[4];
} HCI_ACL_PACKET_t;
#define HCI_ACL_PACKET_HEADER_SIZE (sizeof(HCI_ACL_PACKET_t)-4)
#define HCI_ACL_PACKET_HEADER_SIZE_FOR_CHECK (4)

typedef struct {
	u8 handel_L;
	u8 handel_H;
	u8 dataLen;
	u8 SCO_data[5];
} HCI_SCO_PACKET_t;
#define HCI_SCO_PACKET_HEADER_SIZE (sizeof(HCI_SCO_PACKET_t)-5)
#define HCI_SCO_PACKET_HEADER_SIZE_FOR_CHECK (3)

typedef struct {
	u8 eventCode;
	u8 parameterLen;
	u8 parameter[6];
} HCI_EVENT_PACKET_t;
#define HCI_EVENT_PACKET_HEADER_SIZE (sizeof(HCI_EVENT_PACKET_t)-6)
#define HCI_EVENT_PACKET_HEADER_SIZE_FOR_CHECK (2)

// ============================== Library: Cross-Platform (Manager) ==============================
void LibBT_HCI_Diagnose(void);
// 1. UART packet
u32  LibBT_HCI_GetUartHeaderSize(void);
int  LibBT_HCI_PrepareUartHeader(HCI_PACKET_TYPE_t hci_packet_type, u32 start_offset, OUT u8 *buf);
// 2. HCI packet
u32  LibBT_HCI_GetHciHeaderSize(HCI_PACKET_TYPE_t hci_packet_type);
int  LibBT_HCI_PrepareHciHeader(HCI_PACKET_TYPE_t hci_packet_type, u32 start_offset, u32 header, u32 dataLen, OUT u8 *buf);

void LibBT_HCI_Demo(void);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================

#define _LIB_BT_HCI_HPP_INCLUDED_
#endif//_LIB_BT_HCI_HPP_INCLUDED_



