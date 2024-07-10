#ifndef __STUB_FOR_SIM_H__
#define __STUB_FOR_SIM_H__

typedef enum BT_DEV_TYPE{
    DEVICE_TYPE_BREDR,
    DEVICE_TYPE_BLE
}BT_DEV_TYPE_t;
typedef enum BT_DEV_ROLE
{
    BREDR_ACCESS, //for page, page scan, inquiry, inquiry scan
    BREDR_MASTER,
    BREDR_SLAVE,
    BREDR_STANDBY,//for bredr test command or AFH background RSSI
    LE_ADVERTISER,
    LE_SCANNER,
    LE_INITIATOR,
    LE_MASTER,
    LE_SLAVE,
    LE_CORE  //for ltt,lrt command
    //...left to be added
}BT_DEV_ROLE_t;
typedef struct Bt_Dev_Info_t {
    //Be careful! always put infrastructure at first place, so that contain_of can trace back to the head of each device
    void *infrastructure;
    BT_DEV_TYPE_t Type;
    BT_DEV_ROLE_t Role;

    u16 connection_handle;
}Bt_Dev_Info_t;
typedef struct
{
    u8 AA[4];
    u8 CRCInit[3];
    u8 WinSize;     //unit 1.25ms
    u16 WinOffset;  //unit 1.25ms
    u16 Interval;   //unit 1.25ms
    u16 Latency;    //no unit
    u16 Timeout;    //unit 10ms
    u8 ChM[5];
    u8 Hop: 5;
    u8 SCA: 3;
}Adv_LL_Data_t;
typedef struct
{
    u8 InitA[6];
    u8 AdvA[6];
    Adv_LL_Data_t LLData;
}Adv_Connect_Ind_Payload_t;

typedef enum HCI_ERROR_CODE_T
{
    ERROR_CODES_Memory_Capacity_Exceeded                          = 0x07,
}HCI_ERROR_CODE_T;

Bt_Dev_Info_t *dev_init(BT_DEV_TYPE_t type, BT_DEV_ROLE_t role, u16 size_of_infrastructure);

void Dump_Conn_Info(Bt_Dev_Info_t *dev);

#define ASSERT_Reboot BASIC_ASSERT

#define LC_TASK_QUEUE           0
#define RC_TASK_QUEUE           1
#define BBM_TASK_QUEUE          2
#define MAX_TASK_QUEUE          3
#define INVALID_TASK_QUEUE      0xF


#define INTRA_TASK_BBM          0
#define INTRA_TASK_ASSISTANT    1
#define INTRA_TASK_MAX          2

#define BB_MANAGER_MSG_GROUP_DM                 0x10000
#define BB_MANAGER_MSG_GROUP_LMP                0x20000
#define BB_MANAGER_MSG_GROUP_HCI		        0x70000
#define BB_MANAGER_MSG_GROUP_TIMEOUT            0x90000
#define BB_MANAGER_RESOURCE_CTRL                0xA0000

typedef enum {
    TASK_SUSPEND = 1,
    TASK_READY,
    TASK_BLOCK,
}TASK_STATE;

typedef enum
{
    LE_LM_ADVERTISER,
    LE_LM_SCANNER,
    LE_LM_INITIATOR,
    LE_LM_SLAVE,
    LE_LM_MASTER,
    LE_LM_CORE,
}LE_LM_MODULE_T;


#define LC_GROUP_EVENT           0x10000

enum
{
    MESSAGE_SUBGROUP_BREDR,
    MESSAGE_SUBGROUP_LE
};

#define SUBGROUP_OFFSET         14
#define LE_MODULE_OFFSET        11

typedef enum
{
    LE_LC_ADVERTISER,
    LE_LC_SCANNER,
    LE_LC_INITIATOR,
    LE_LC_SLAVE,
    LE_LC_MASTER,
    LE_LC_CORE,
}LE_LC_MODULE_T;

#define TAILQ_ENTRY(type)						\
struct {                                \
    struct type *tqe_next;  /* next element */          \
    struct type **tqe_prev; /* address of previous next element */  \
}

#define TAILQ_HEAD(name, type)						\
struct name {                               \
    struct type *tqh_first; /* first element */         \
    struct type **tqh_last; /* addr of last next element */     \
}

#define TAILQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.tqe_next = NULL;					\
	(elm)->field.tqe_prev = (head)->tqh_last;			\
	*(head)->tqh_last = (elm);					\
	(head)->tqh_last = &(elm)->field.tqe_next;			\
} while (0)

#define	TAILQ_INIT(head) do {						\
	(head)->tqh_first = NULL;					\
	(head)->tqh_last = &(head)->tqh_first;				\
} while (0)

u32 scheduler_simulate_delay_in_us(void);

#endif //#define __STUB_FOR_SIM_H__


