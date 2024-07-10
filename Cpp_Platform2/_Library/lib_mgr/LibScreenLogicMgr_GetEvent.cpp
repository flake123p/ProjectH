
#include <stdio.h>

#include "My_Basics.hpp"

#include "_LibError.hpp"
#include "_LibString.hpp"
#include "LibBuffer.hpp"
#include "LibEvent.hpp"        // LibEvent_GetOneKeyBoard()
#include "LibScreenLogic.hpp"  // return DSPTCHR_EVT_KEYBOARD
#include "LibScreenLogic_GetEvent.hpp"

static USER_EVENT_t gUserEvent;

SCAN_KEY_MGR_DB_t *gScanDB = NULL;
void LibScreenLogic_SetScanKeyDatabase(IN SCAN_KEY_MGR_DB_t *db)
{
	gScanDB = db;
	gScanDB->buf.Init();
}

u32 LibScreenLogic_ScanMgr_Hex(int ch)
{
	if(LibString_IsCharHexadecimal(ch)) {
		bool isPushSuccess = gScanDB->buf.Push((u8)ch);

		if (isPushSuccess) {
			return USER_EVT_KB_SCAN_PUSH_OK;
		}
	} else if (ch == KEYBOARD_ENTER) {
		// Case "Enter":
		if (gScanDB->buf.curUseLen == 0) {
			gScanDB->numeric = 0;
		} else {
			// Make a string for sscanf
			gScanDB->buf.buf[gScanDB->buf.curUseLen] = 0;
			if (1 != sscanf((const char*)gScanDB->buf.buf, "%x", &(gScanDB->numeric))) {
				BASIC_ASSERT(0);
			}
		}
		return USER_EVT_KB_SCAN_DONE;
	} else if (ch == KEYBOARD_BACKSPACE) {
		u8 temp;
		bool isPopSuccess = gScanDB->buf.Pop(&temp);

		if (isPopSuccess) {
			return USER_EVT_KB_SCAN_POP_OK;
		} else {
			return USER_EVT_KB_SCAN_POP_NG;
		}
	}

	return USER_EVT_KB_SCAN_PUSH_NG;
}

// Return user event
u32 LibScreenLogic_ScanMgr(int ch)
{
	switch (gScanDB->type) {
		case SCAN_KEY_TYPE_HEX: {
			return LibScreenLogic_ScanMgr_Hex(ch);
		} break;

		case SCAN_KEY_TYPE_DEC: {
		} break;

		case SCAN_KEY_TYPE_YES_NO: {
		} break;

		case SCAN_KEY_TYPE_STRING: {
		} break;

		default: {
			BASIC_ASSERT(0);
		} break;
	}
	return 0;
}


u32 LibScreenLogic_CB_GetEvent(OUT void **ScrnEvtHdlPtr)
{
	int ch = LibEvent_GetOneKeyBoard();
	*ScrnEvtHdlPtr = &gUserEvent;

	gUserEvent.kb = ch;

	if (gScanDB != NULL) {
		gUserEvent.user_event = LibScreenLogic_ScanMgr(ch);
		return DSPTCHR_EVT_KEYBOARD;
	}

	switch (ch) {
		case KEYBOARD_ESC:
			gUserEvent.user_event = USER_EVT_KB_ESC;
		break;

		default:
			gUserEvent.user_event = DSPTCHR_EVT_KEYBOARD + (u32)ch;
		break;
	}
	
	return DSPTCHR_EVT_KEYBOARD;
}

static u8 gBuffer[300];
//LibBufferLog<u8> gBuf = LibBufferLog<u8>(gBuffer, 300);
SCAN_KEY_MGR_DB_t gMyScanDB = {
	SCAN_KEY_TYPE_HEX,
	LibBufferLog<u8>(gBuffer, 4),
};
void LibScreenLogic_GetEvent_DEMO(void)
{
	PRINT_FUNC;
	
	USER_EVENT_t *hdl;
	u32 retVal;

	do {
		retVal = LibScreenLogic_CB_GetEvent((void **)(&hdl));
		DUMPX(retVal);
		DUMPX(hdl->kb);
		DUMPC(hdl->kb);
		DUMPX(hdl->user_event);
	} while (hdl->user_event != USER_EVT_KB_ESC);

	PRINT_LINE;
	LibScreenLogic_SetScanKeyDatabase(&gMyScanDB);
	do {
		retVal = LibScreenLogic_CB_GetEvent((void **)(&hdl));
		if (hdl->user_event == USER_EVT_KB_SCAN_PUSH_OK) {
			printf("%c", hdl->kb);
		}
	} while (hdl->user_event != USER_EVT_KB_SCAN_DONE);
	printf("\nstring is %s\n", gMyScanDB.buf.buf);
	gMyScanDB.buf.Dump();
	DUMPD(gMyScanDB.numeric);
	DUMPX(gMyScanDB.numeric);
}
