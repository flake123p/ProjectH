
#include "Everything_Lib_Mgr.hpp"

void LibBT_HCI_Diagnose(void)
{
	BASIC_ASSERT(HCI_COMMAND_PACKET_HEADER_SIZE == HCI_COMMAND_PACKET_HEADER_SIZE_FOR_CHECK);
	BASIC_ASSERT(HCI_ACL_PACKET_HEADER_SIZE     == HCI_ACL_PACKET_HEADER_SIZE_FOR_CHECK);
	BASIC_ASSERT(HCI_SCO_PACKET_HEADER_SIZE     == HCI_SCO_PACKET_HEADER_SIZE_FOR_CHECK);
	BASIC_ASSERT(HCI_EVENT_PACKET_HEADER_SIZE   == HCI_EVENT_PACKET_HEADER_SIZE_FOR_CHECK);
}

u32  LibBT_HCI_GetUartHeaderSize(void)
{
	return 1;
}

int  LibBT_HCI_PrepareUartHeader(HCI_PACKET_TYPE_t hci_packet_type, u32 start_offset, OUT u8 *buf)
{
	switch (hci_packet_type) {
		case HCI_Command_Packet: {
			buf[start_offset] = UART_HEADER_HCI_COMMAND;
		} break;
		
		case HCI_ACL_Packet: {
			buf[start_offset] = UART_HEADER_HCI_ACL;
		} break;
		
		case HCI_SCO_Packet: {
			buf[start_offset] = UART_HEADER_HCI_SCO;
		} break;
		
		case HCI_Event_Packet: {
			buf[start_offset] = UART_HEADER_HCI_EVENT;
		} break;
			
		default:
			BASIC_ASSERT(0);
			break;
	}
	return 0;
}

u32  LibBT_HCI_GetHciHeaderSize(HCI_PACKET_TYPE_t hci_packet_type)
{
	switch (hci_packet_type) {
		case HCI_Command_Packet: {
			return HCI_COMMAND_PACKET_HEADER_SIZE;
		} break;
		
		case HCI_ACL_Packet: {
			return HCI_ACL_PACKET_HEADER_SIZE;
		} break;
		
		case HCI_SCO_Packet: {
			return HCI_SCO_PACKET_HEADER_SIZE;
		} break;
		
		case HCI_Event_Packet: {
			return HCI_EVENT_PACKET_HEADER_SIZE;
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	return 0;
}

int  LibBT_HCI_PrepareHciHeader(HCI_PACKET_TYPE_t hci_packet_type, u32 start_offset, u32 header, u32 dataLen, OUT u8 *buf)
{
	switch (hci_packet_type) {
		case HCI_Command_Packet: {
			BASIC_ASSERT(header < 0x10000);
			BASIC_ASSERT(dataLen < 0x100);
			
			HCI_COMMAND_PACKET_t *ptr = (HCI_COMMAND_PACKET_t *)&buf[start_offset];
			ptr->opcode_L = (u8)header;
			ptr->opcode_H = (u8)(header>>8);
			ptr->parameterLen = (u8)dataLen;
		} break;

		case HCI_ACL_Packet: {
			BASIC_ASSERT(header < 0x10000);
			BASIC_ASSERT(dataLen < 0x10000);

			HCI_ACL_PACKET_t *ptr = (HCI_ACL_PACKET_t *)&buf[start_offset];
			ptr->handel_L = (u8)header;
			ptr->handel_H = (u8)(header>>8);
			ptr->dataLen_L = (u8)dataLen;
			ptr->dataLen_H = (u8)(dataLen>>8);
		} break;

		case HCI_SCO_Packet: {
			BASIC_ASSERT(header < 0x10000);
			BASIC_ASSERT(dataLen < 0x100);

			HCI_SCO_PACKET_t *ptr = (HCI_SCO_PACKET_t *)&buf[start_offset];
			ptr->handel_L = (u8)header;
			ptr->handel_H = (u8)(header>>8);
			ptr->dataLen = (u8)dataLen;
		} break;

		case HCI_Event_Packet: {
			BASIC_ASSERT(header < 0x100);
			BASIC_ASSERT(dataLen < 0x100);
			
			HCI_EVENT_PACKET_t *ptr = (HCI_EVENT_PACKET_t *)&buf[start_offset];
			ptr->eventCode= (u8)header;
			ptr->parameterLen = (u8)dataLen;
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	return 0;
}

void LibBT_HCI_Demo(void)
{
	LibBT_HCI_Diagnose();
	
	PRINT_FUNC(1);
	u8 *buf = (u8 *)malloc(600);
	u8 pdu[] = {0x99, 0x88, 0x77, 0x66, 0xAA, 0xAA, 0xBB, 0xBB};

	u32 offset_1_UartHeader;
	u32 offset_2_HciHeader;
	u32 offset_3_HciPayload;
	u32 hci_payload_size;
	HCI_PACKET_TYPE_t packetType;

	packetType = HCI_ACL_Packet;
	offset_1_UartHeader = 0;
	offset_2_HciHeader = offset_1_UartHeader + LibBT_HCI_GetUartHeaderSize();
	offset_3_HciPayload = offset_2_HciHeader + LibBT_HCI_GetHciHeaderSize(packetType);
	hci_payload_size = sizeof(pdu);
	memcpy(&buf[offset_3_HciPayload], pdu, hci_payload_size);
	LibBT_HCI_PrepareHciHeader(packetType, offset_2_HciHeader, 0x0FFF, sizeof(pdu), buf);
	LibBT_HCI_PrepareUartHeader(packetType, offset_1_UartHeader, buf);
	ARRAYDUMPX2(buf, offset_3_HciPayload+hci_payload_size);

	
	packetType = HCI_Command_Packet;
	offset_1_UartHeader = 0;
	offset_2_HciHeader = offset_1_UartHeader + LibBT_HCI_GetUartHeaderSize();
	offset_3_HciPayload = offset_2_HciHeader + LibBT_HCI_GetHciHeaderSize(packetType);
	hci_payload_size = sizeof(pdu);
	memcpy(&buf[offset_3_HciPayload], pdu, hci_payload_size);
	LibBT_HCI_PrepareHciHeader(packetType, offset_2_HciHeader, 0xFCA1, sizeof(pdu), buf);
	LibBT_HCI_PrepareUartHeader(packetType, offset_1_UartHeader, buf);
	ARRAYDUMPX2(buf, offset_3_HciPayload+hci_payload_size);
	
	free(buf);
}

