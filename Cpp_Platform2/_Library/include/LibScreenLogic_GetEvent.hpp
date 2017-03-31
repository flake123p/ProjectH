

#ifndef _LIB_SCREEN_LOGIC_GET_EVENT_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

#include "My_Basics.hpp"
#include "LibBuffer.hpp"

typedef struct {
  u32   user_event;
  int   kb;
} USER_EVENT_t;

typedef enum {
  SCAN_KEY_TYPE_HEX = 0,
  SCAN_KEY_TYPE_DEC,
  SCAN_KEY_TYPE_YES_NO,
  SCAN_KEY_TYPE_STRING,

  SCAN_KEY_TYPE_INVALID,
} SCAN_KEY_TYPE;

typedef struct {
	IN  SCAN_KEY_TYPE   type;
	LibBufferLog<u8>  buf; //Make sure buf max len is less 1 than REAL BUFFER.
	/*
		IN  T1 *buf;
		IN  u32 bufMaxLen; // 1 bassed. (Means start from 1, not 0)
		OUT u32 curUseLen; // 0 bassed. (Means start from 0)
		OUT u32 size_of_type;
		OUT u32 bufMaxLen_In_Byte;
	*/
	OUT u32             numeric;    //Numeric value from HEX string or DEC string.
	OUT bool            isYes;
} SCAN_KEY_MGR_DB_t; // Scan key manager database

//User event
#define USER_EVT_KB_ENTER         (0x0200000D)
#define USER_EVT_KB_ESC           (0x0200001B)

//Scan event
#define USER_EVT_KB_SCANING       (0x03000000)
#define USER_EVT_KB_SCAN_DONE     (0x03000001)
#define USER_EVT_KB_SCAN_PUSH_NG  (0x03000002)
#define USER_EVT_KB_SCAN_PUSH_OK  (0x03000003)
#define USER_EVT_KB_SCAN_POP_NG   (0x03000004)
#define USER_EVT_KB_SCAN_POP_OK   (0x03000005)
#define USER_EVT_KB_SCAN_YES      (0x03000010)
#define USER_EVT_KB_SCAN_NO       (0x03000011)

u32 LibScreenLogic_CB_GetEvent(OUT void **ScrnEvtHdlPtr);

void LibScreenLogic_GetEvent_DEMO(void);

#define _LIB_SCREEN_LOGIC_GET_EVENT_HPP_INCLUDED_
#endif//_LIB_SCREEN_LOGIC_GET_EVENT_HPP_INCLUDED_

