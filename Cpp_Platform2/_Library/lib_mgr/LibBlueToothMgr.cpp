
#include "Everything_Lib_Mgr.hpp"

void LibBT_Diagnose(void)
{
	BASIC_ASSERT(UART_HCI_HEADER_SIZE == UART_HCI_HEADER_SIZE_FOR_CHECK);
	
	BASIC_ASSERT(HCI_COMMAND_PACKET_HEADER_SIZE == HCI_COMMAND_PACKET_HEADER_SIZE_FOR_CHECK);
	BASIC_ASSERT(HCI_ACL_PACKET_HEADER_SIZE     == HCI_ACL_PACKET_HEADER_SIZE_FOR_CHECK);
	BASIC_ASSERT(HCI_SCO_PACKET_HEADER_SIZE     == HCI_SCO_PACKET_HEADER_SIZE_FOR_CHECK);
	BASIC_ASSERT(HCI_EVENT_PACKET_HEADER_SIZE   == HCI_EVENT_PACKET_HEADER_SIZE_FOR_CHECK);
}

u32 LibBT_GetHciPduOffset(UART_HCI_PACKET_TYPE_t uartHciType, u32 precedentLen /* = 4 */)
{
	switch (uartHciType) {
		case UART_HCI_COMMAND:
			return precedentLen + HCI_COMMAND_PACKET_HEADER_SIZE;
		case UART_HCI_ACL:
			return precedentLen + HCI_ACL_PACKET_HEADER_SIZE;
		case UART_HCI_SCO:
			return precedentLen + HCI_SCO_PACKET_HEADER_SIZE;
		case UART_HCI_EVENT:
			return precedentLen + HCI_EVENT_PACKET_HEADER_SIZE;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	return 0;
}

int LibBT_MakeHciPacketHeader(UART_HCI_PACKET_TYPE_t uartHciType, u32 header, u32 dataLen, OUT u8 *buf, u32 precedentLen /* = 4 */)
{
	switch (uartHciType) {
		case UART_HCI_COMMAND: {
			BASIC_ASSERT(header < 0x10000);
			BASIC_ASSERT(dataLen < 0x100);
			
			HCI_COMMAND_PACKET_t *cmdPtr = (HCI_COMMAND_PACKET_t *)&buf[precedentLen];
			cmdPtr->opcode_L = (u8)header;
			cmdPtr->opcode_H = (u8)(header>>8);
			cmdPtr->parameterLen = (u8)dataLen;
		} break;

		case UART_HCI_ACL: {
			BASIC_ASSERT(header < 0x10000);
			BASIC_ASSERT(dataLen < 0x10000);

			HCI_ACL_PACKET_t *cmdPtr = (HCI_ACL_PACKET_t *)&buf[precedentLen];
			cmdPtr->handel_L = (u8)header;
			cmdPtr->handel_H = (u8)(header>>8);
			cmdPtr->dataLen_L = (u8)dataLen;
			cmdPtr->dataLen_H = (u8)(dataLen>>8);
		} break;

		case UART_HCI_SCO: {
			BASIC_ASSERT(header < 0x10000);
			BASIC_ASSERT(dataLen < 0x100);

			HCI_SCO_PACKET_t *cmdPtr = (HCI_SCO_PACKET_t *)&buf[precedentLen];
			cmdPtr->handel_L = (u8)header;
			cmdPtr->handel_H = (u8)(header>>8);
			cmdPtr->dataLen = (u8)dataLen;
		} break;

		case UART_HCI_EVENT: {
			BASIC_ASSERT(header < 0x100);
			BASIC_ASSERT(dataLen < 0x100);
			
			HCI_EVENT_PACKET_t *cmdPtr = (HCI_EVENT_PACKET_t *)&buf[precedentLen];
			cmdPtr->eventCode= (u8)header;
			cmdPtr->parameterLen = (u8)dataLen;
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	return 0;
}

int LibBT_MakeUartHciHeader(UART_HCI_PACKET_TYPE_t uartHciType, OUT u8 *buf)
{
	UART_HCI_HEADER_t *uartHeader = (UART_HCI_HEADER_t *)buf;
	uartHeader->reserved[0] = 0;
	uartHeader->reserved[1] = 0;
	uartHeader->reserved[2] = 0;
	switch (uartHciType) {
		case UART_HCI_COMMAND: {
			uartHeader->packetType = 1;
		} break;

		case UART_HCI_ACL: {
			uartHeader->packetType = 2;
		} break;

		case UART_HCI_SCO: {
			uartHeader->packetType = 3;
		} break;

		case UART_HCI_EVENT: {
			uartHeader->packetType = 4;
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	return 0;
}

void LibBT_DumpUartHciPacket(u8 *buf, u32 bufLen)
{
	for (u32 i = 0; i < bufLen; i++) {
		printf("%02X ", buf[i]);
	}
	printf("\n");
}

void LibBT_Demo(void)
{
	LibBT_Diagnose();
	
	PRINT_FUNC(1);
	u8 *buf = (u8 *)malloc(600);
	u32 bufLen = 16;

	u8 pdu[] = {0x00, 0x20, 0x01, 0x02, 0xAA, 0xAA, 0xBB, 0xBB};

	u32 offset = LibBT_GetHciPduOffset(UART_HCI_COMMAND);
	memcpy(&buf[offset], pdu, sizeof(pdu));
	bufLen = offset + sizeof(pdu);
	LibBT_MakeHciPacketHeader(UART_HCI_COMMAND, 0xFCA1, sizeof(pdu), buf);
	LibBT_MakeUartHciHeader(UART_HCI_COMMAND, buf);
	LibBT_DumpUartHciPacket(buf, bufLen);
	DUMPD(LibBT_GetHciPduOffset(UART_HCI_COMMAND));

	offset = LibBT_GetHciPduOffset(UART_HCI_ACL);
	memcpy(&buf[offset], pdu, sizeof(pdu));
	bufLen = offset + sizeof(pdu);
	LibBT_MakeHciPacketHeader(UART_HCI_ACL, 0x0FFF, sizeof(pdu), buf);
	LibBT_MakeUartHciHeader(UART_HCI_ACL, buf);
	LibBT_DumpUartHciPacket(buf, bufLen);
	DUMPD(LibBT_GetHciPduOffset(UART_HCI_ACL));

	free(buf);
}

