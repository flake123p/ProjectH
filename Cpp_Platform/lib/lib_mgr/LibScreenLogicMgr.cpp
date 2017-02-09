
#include <stdio.h>

#include "My_Basics.hpp"

#include "LibEvent.hpp" // LibEvent_GetOneKeyBoard()
#include "LibScreenLogic.hpp"

SCREEN_GROUP_CB_LIST_t GroupCallbackList_DUMMY[] = {
	{0, NO_ON_ENTRY, NO_ON_DRAW, NO_ON_EVENT, NO_ON_LEAVE},
};

SCREEN_CB_LIST_t *ScreenCallbackList = NULL;
SCREEN_GROUP_CB_LIST_t *GroupCallbackList = GroupCallbackList_DUMMY;

u32         gCurrentScrnIndex = 0xFFFFFFFF;
ON_ENTRY_t  gCurrentScrnOnEntry = NULL;
ON_DRAW_t   gCurrentScrnOnDraw  = NULL;
ON_EVENT_t  gCurrentScrnOnEvent = NULL;
ON_LEAVE_t  gCurrentScrnOnLeave = NULL;

u32         gCurrentGroupIndex = 0xFFFFFFFF;
ON_ENTRY_t  gCurrentGroupOnEntry = NULL;
ON_DRAW_t   gCurrentGroupOnDraw  = NULL;
ON_EVENT_t  gCurrentGroupOnEvent = NULL;
ON_LEAVE_t  gCurrentGroupOnLeave = NULL;

GET_EVENT_t gGetEvent = NULL;

u32 gPriorEvent  = DSPTCHR_EVT_NON;

void LibScreenLogic_SetPriorEvent(IN u32 event)
{
	gPriorEvent = event;
}

int 
LibScreenLogic_MainDispatcher (
	IN  u32 RootScreenIndex,
	IN  u32 ScrnEvtId,
	IN  void *ScrnEvtHdl)
{
	int retVal;
	u32 screen_event_id;
	void *screen_event_handle;

	LibScreenLogic_GotoNewScreenEx(RootScreenIndex, ScrnEvtId, ScrnEvtHdl);

	BASIC_ASSERT(gGetEvent != NULL);
	while(true) {
		if (gPriorEvent == DSPTCHR_EVT_NON) {
			screen_event_id = (*gGetEvent)(&screen_event_handle);
		}else {
			screen_event_id = gPriorEvent;
			gPriorEvent = DSPTCHR_EVT_NON;
		}
		
		switch (screen_event_id) {
			case DSPTCHR_EVT_KEYBOARD: {
				if(gCurrentScrnOnEvent != NULL) {
					retVal = (*gCurrentScrnOnEvent) (screen_event_id, screen_event_handle);
				}
			} break;
			
			case DSPTCHR_EVT_EXIT: {
				goto QUIT_ALL;
			} break;

			case DSPTCHR_EVT_UPDATE_SCREEN: {
				if(gCurrentScrnOnDraw != NULL) {
					(*gCurrentScrnOnDraw) (NULL_ARGU, NULL_ARGU);
				}
			} break;

			default: {
			} break;
		}
	}
QUIT_ALL:
	return retVal;
}

int 
LibScreenLogic_GotoNewScreen (
	IN  u32  NextScreenIndex)
{
	return LibScreenLogic_GotoNewScreenEx(NextScreenIndex, NULL_ARGU, NULL);
}

int 
LibScreenLogic_GotoNewScreenEx (
	IN  u32  NextScreenIndex,
	IN  u32  ScrnEvtId,
	IN  void *ScrnEvtHdl)
{
	u32 NextGroupIndex = ScreenCallbackList[NextScreenIndex].Group;

	//
	// Old Screen Flow
	//
	if (gCurrentScrnOnLeave != NULL) {
		(*gCurrentScrnOnLeave) (ScrnEvtId, ScrnEvtHdl);
	}
	//
	// Old Group Flow
	//
	if (gCurrentGroupIndex != NextGroupIndex && gCurrentGroupOnLeave != NULL) {
		(*gCurrentGroupOnLeave) (ScrnEvtId, ScrnEvtHdl);
	}

	//
	// New Screen Callbacks
	//
	//ASSERT(NextScreenIndex < SCRN_MAX_INDEX);
	BASIC_ASSERT(NextScreenIndex == ScreenCallbackList[NextScreenIndex].Index);
	gCurrentScrnIndex   = NextScreenIndex;
	gCurrentScrnOnEntry = ScreenCallbackList[NextScreenIndex].OnEntry;
	gCurrentScrnOnDraw  = ScreenCallbackList[NextScreenIndex].OnDraw;
	gCurrentScrnOnEvent = ScreenCallbackList[NextScreenIndex].OnEvent;
	gCurrentScrnOnLeave = ScreenCallbackList[NextScreenIndex].OnLeave;
	//
	// New Group Callbacks
	//
	if (gCurrentGroupIndex != NextGroupIndex) {
		//ASSERT(NextGroupIndex < SCRN_GROUP_MAX_INDEX);
		BASIC_ASSERT(NextGroupIndex == GroupCallbackList[NextGroupIndex].Group);
		gCurrentGroupIndex = NextGroupIndex;
		gCurrentGroupOnEntry = GroupCallbackList[NextGroupIndex].OnEntry;
		gCurrentGroupOnDraw  = GroupCallbackList[NextGroupIndex].OnDraw;
		gCurrentGroupOnEvent = GroupCallbackList[NextGroupIndex].OnEvent;
		gCurrentGroupOnLeave = GroupCallbackList[NextGroupIndex].OnLeave;
		//
		// New Group Flow
		//
		if(gCurrentGroupOnEntry != NULL) {
			(*gCurrentGroupOnEntry) (ScrnEvtId, ScrnEvtHdl);
		}
		if(gCurrentGroupOnDraw != NULL) {
			(*gCurrentGroupOnDraw) (ScrnEvtId, ScrnEvtHdl);
		}
	}

	//
	// New Screen Flow
	//
	if(gCurrentScrnOnEntry != NULL) {
		(*gCurrentScrnOnEntry) (ScrnEvtId, ScrnEvtHdl);
	}
	if(gCurrentScrnOnDraw != NULL) {
		(*gCurrentScrnOnDraw) (ScrnEvtId, ScrnEvtHdl);
	}

	return 0;
}

void
LibScreenLogic_SetCB_GetEvent(
	IN  GET_EVENT_t cb)
{
	gGetEvent = cb;
}

void
LibScreenLogic_SetCB_ScreenList(
	IN  SCREEN_CB_LIST_t *list)
{
	ScreenCallbackList = list;
}

void
LibScreenLogic_SetCB_GroupList(
	IN  SCREEN_GROUP_CB_LIST_t *list)
{
	GroupCallbackList = list;
}


int Scrn00AAA_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Scrn00AAA_OnDraw(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Scrn00AAA_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	LibScreenLogic_GotoNewScreen(1);
	return 0;
}

int Scrn00AAA_OnLeave(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Scrn01BBB_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Scrn01BBB_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	LibScreenLogic_GotoNewScreen(2);
	return 0;
}

int Scrn02CCC_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Scrn02CCC_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	LibScreenLogic_SetPriorEvent(DSPTCHR_EVT_EXIT); //Leave Demo
	return 0;
}

int Group00_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Group00_OnDraw(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Group00_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Group00_OnLeave(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Group01_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

int Group01_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)
{PRINT_FUNCn
	return 0;
}

SCREEN_CB_LIST_t Demo_ScreenCallbackList[] = {
	{0, 0, Scrn00AAA_OnEntry, Scrn00AAA_OnDraw, Scrn00AAA_OnEvent, Scrn00AAA_OnLeave},
	{1, 0, Scrn01BBB_OnEntry, NO_ON_DRAW, Scrn01BBB_OnEvent, NO_ON_LEAVE},
	{2, 1, Scrn02CCC_OnEntry, NO_ON_DRAW, Scrn02CCC_OnEvent, NO_ON_LEAVE},
};

SCREEN_GROUP_CB_LIST_t Demo_GroupCallbackList[] = {
	{0, Group00_OnEntry, Group00_OnDraw, Group00_OnEvent, Group00_OnLeave},
	{1, Group01_OnEntry, NO_ON_DRAW, Group01_OnEvent, NO_ON_LEAVE},
};

static int gKeyBoard;
u32 LibScreenLogic_CB_SimpleGetEvent(OUT void **ScrnEvtHdlPtr)
{
	*ScrnEvtHdlPtr = &gKeyBoard;
	gKeyBoard = LibEvent_GetOneKeyBoard();
	return DSPTCHR_EVT_KEYBOARD;
}

void LibScreenLogic_Demo(void)
{
	LibScreenLogic_SetCB_GetEvent(LibScreenLogic_CB_SimpleGetEvent);
	LibScreenLogic_SetCB_ScreenList(Demo_ScreenCallbackList);
	LibScreenLogic_SetCB_GroupList(Demo_GroupCallbackList);

	LibScreenLogic_MainDispatcher(0, NULL_ARGU, NULL);
}

