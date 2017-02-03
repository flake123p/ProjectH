

#ifndef _LIB_SCREEN_LOGIC_HPP_INCLUDED_

#include "My_Basics.hpp"

// ============================== Manager ==============================
typedef int (* ON_ENTRY_t) (IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);
typedef int (* ON_DRAW_t)  (IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);
typedef int (* ON_EVENT_t) (IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);
typedef int (* ON_LEAVE_t) (IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);
typedef struct {
  u32         Index;
  u32         Group;
  ON_ENTRY_t  OnEntry;
  ON_DRAW_t   OnDraw;
  ON_EVENT_t  OnEvent;
  ON_LEAVE_t  OnLeave;
} SCREEN_CB_LIST_t;
typedef struct {
  u32         Group;
  ON_ENTRY_t  OnEntry;
  ON_DRAW_t   OnDraw;
  ON_EVENT_t  OnEvent;
  ON_LEAVE_t  OnLeave;
} SCREEN_GROUP_CB_LIST_t;

#define NO_ON_ENTRY NULL
#define NO_ON_DRAW  NULL
#define NO_ON_EVENT NULL
#define NO_ON_LEAVE NULL
#define NULL_ARGU   (0)

typedef u32 (* GET_EVENT_t) (void);
// ============================== Library: Cross-Platform ==============================
// For minimizing the global variable uses

// ============================== Library: Platform Dependant ==============================

int 
LibScreenLogic_MainDispatcher (
	IN u32 RootScreenIndex, 
	IN u32 ScrnEvtId, 
	IN void *ScrnEvtHdl);

int 
LibScreenLogic_GotoNewScreen (
	IN  u32  NextScreenIndex);

int 
LibScreenLogic_GotoNewScreenEx (
	IN  u32  NextScreenIndex,
	IN  u32  ScrnEvtId,
	IN  void *ScrnEvtHdl);

void
LibScreenLogic_SetCB_GetEvent(
	IN  GET_EVENT_t cb);

void
LibScreenLogic_SetCB_ScreenList(
	IN  SCREEN_CB_LIST_t *list);

void
LibScreenLogic_SetCB_GroupList(
	IN  SCREEN_GROUP_CB_LIST_t *list);

void LibScreenLogic_Demo(void);

#define _LIB_SCREEN_LOGIC_HPP_INCLUDED_
#endif//_LIB_SCREEN_LOGIC_HPP_INCLUDED_

