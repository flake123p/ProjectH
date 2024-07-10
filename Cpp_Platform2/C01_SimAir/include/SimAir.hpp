

#ifndef _SIM_AIR_HPP_INCLUDED_

typedef enum {
	SIM_AIR_SLEEP,
	SIM_AIR_TX,
	SIM_AIR_RX,
}SIM_AIR_EVENT_t;
typedef void (*SIM_AIR_CB_t)(void *msg);

int SimAir_AddEvent(SIM_AIR_EVENT_t event, u32 tick, SIM_AIR_CB_t callback, void *msg);

int SimAir_Init(void);
int SimAir_Run(void);



#define _SIM_AIR_HPP_INCLUDED_
#endif//_SIM_AIR_HPP_INCLUDED_



