
#include "Everything_App.hpp"
#include "unix_sys_queue.h"

#if 0
void UartRx(void)
{
	LibUart_SniffSetting(1);
	
	LibUartClass uart(NULL, 8000);
	uart.comPortName = "COM26";
	uart.baudRate = 115200;
	uart.InitComPort(3);
	//
	//uart.ReceiveWithLength(2, 2000);
	//
    u8 sendBuf[] = {0x01, 0x03, 0x0C, 0x00};
	uart.Send(sendBuf, sizeof(sendBuf));
	uart.ReceiveWithLength(2, 2000);
	uart.UninitComPort();
}
#endif

DLList_Head_t gHead1 = DLLIST_HEAD_INIT(&gHead1);
SLList_Head_t gHead2;

typedef struct {
    DLList_Entry_t dlist_common_header;
    SLList_Entry_t slist_common_header;
    u32 i; 
} Abcc_t;

void Chimera_Test(void);

int main(int argc, char *argv[])
{
	Abcc_t a,b,c;
	a.i = 9;
	b.i = 11;
    c.i = 13;

    DLLIST_HEAD_RESET(&gHead1);
    SLLIST_HEAD_RESET(&gHead2);

    DLLIST_INSERT_LAST(&gHead1, &c);
	DLLIST_INSERT_LAST(&gHead1, &b);
	//DLLIST_INSERT_AFTER(&gHead1, &gHead1, &a)
    DLLIST_INSERT_LAST(&gHead1, &a);

    SLLIST_INSERT_LAST(&gHead2, &a.slist_common_header);
    SLLIST_INSERT_LAST(&gHead2, &b.slist_common_header);
    SLLIST_INSERT_AFTER(&gHead2, &a.slist_common_header, &c.slist_common_header);

    Abcc_t *p;
    DLLIST_FOREACH(&gHead1, p, Abcc_t)
    {
        printf("p->i=%d\n", p->i);
        if (p->i==11)
        {
            DLLIST_REMOVE_NODE(&gHead1, p);
        }
    }
    //DLLIST_REMOVE_FIRST(&gHead1);
    //DLLIST_REMOVE_LAST(&gHead1);
    //DLLIST_REMOVE_LAST(&gHead1);

    //SLLIST_REMOVE_FIRST_SAFELY(&gHead2);
    //SLLIST_REMOVE_NEXT_SAFELY(&gHead2, &c.slist_common_header);

    void *vp;
    SLLIST_FOREACH(&gHead2, vp, void)
    {
        printf("vp->i=%d\n", STRUCT_ENTRY2(vp, Abcc_t, slist_common_header)->i);
    }
/*
    Abcc_t *p = (Abcc_t *)DLLIST_FIRST(&gHead1);
    printf("p->i=%d\n", p->i);

    p = (Abcc_t *)DLLIST_FIRST(&gHead1);
    printf("p->i=%d\n", p->i);

    DLLIST_REMOVE_FIRST(&gHead1);

    p = (Abcc_t *)DLLIST_FIRST(&gHead1);
    printf("p->i=%d\n", p->i);
*/
    Chimera_Test();
	return 0;
}


#define Bt_Dev_Info_t void
#define bbm_heap_malloc malloc
#define bbm_heap_free   free
#define bbm_cmn_malloc  malloc
#define bbm_cmn_free    free
#define LM_DBG_ASSERT BASIC_ASSERT


typedef enum {
    LM_PACKET_LMP,
    LM_PACKET_ACL,
} LM_PACKET_TYPE_t;

typedef struct {
    SLList_Entry_t sll_entry;
    DLList_Entry_t rx_occupid_entry;
    u8 type; //LM_PACKET_TYPE_t
    u8 is_released;
    u16 buf_len;
    u8 *buf;
} Lm_Rx_Desc_t;
typedef struct {
    Lm_Rx_Desc_t rx_desc;
    u8 l_ch;
    u16 conn_hdl;
    Bt_Dev_Info_t *dev; //need this?
} Lm_Rx_Acl_Desc_t; //LM_PACKET_ACL
typedef struct {
    Lm_Rx_Desc_t rx_desc;
    Bt_Dev_Info_t *dev;
} Lm_Rx_Lmp_Desc_t; //LM_PACKET_LMP

#define __________INTERFACE_TO_RX_BUF__________
#define __________INTERFACE_TO_RX_BUF_________
void TBD_RxOccupationRelease(Lm_Rx_Desc_t *rx_desc)
{
    //update BP
    //BP-> CP+len-1
}

void TBD_RxOccupationReverse(u32 reverse_len)
{
    //update CP
}

#define ____________NEW_CODE______________________
#define ____________NEW_CODE_____________________

#define LM_SINGLE_ACL_TX_BUF_SIZE (340)
#define LM_TOTAL_ACL_TX_BUF       (12)
#define LM_SINGLE_LMP_TX_BUF_SIZE (27)
#define LM_TOTAL_LMP_TX_BUF       (4)
#define LM_SINGLE_ACL_TX_BUF_SIZE_ALIGNED ALIGN_SIZE(LM_SINGLE_ACL_TX_BUF_SIZE, 4) /*align to 4*/
#define LM_SINGLE_LMP_TX_BUF_SIZE_ALIGNED ALIGN_SIZE(LM_SINGLE_LMP_TX_BUF_SIZE, 4) /*align to 4*/

#define LM_TOTAL_ACL_TX_DESC  (LM_TOTAL_ACL_TX_BUF)
#define LM_TOTAL_LMP_TX_DESC  (LM_TOTAL_LMP_TX_BUF)

#define LM_TOTAL_ACL_RX_DESC  (12)
#define LM_TOTAL_LMP_RX_DESC  (4)
#define LM_TOTAL_LMP_INSTANT_DESC (4)
typedef struct {
    SLList_Entry_t sll_entry;
    u8 *cmn_mem;
} Lm_Tx_Acl_Buf_t;

typedef struct {
    SLList_Entry_t sll_entry;
    u8 *cmn_mem;
    u8 is_dynamic;
} Lm_Tx_Lmp_Buf_t;

typedef struct {
    SLList_Entry_t sll_entry;
} Lm_Tx_Acl_Desc_t;

typedef struct {
    SLList_Entry_t sll_entry;
    u8 is_dynamic;
} Lm_Tx_Lmp_Desc_t;

typedef struct {
    SLList_Entry_t sll_entry;
    u8 is_active;
    u8 is_dynamic;
    u8 is_need_to_free_data; //Default is 0, do nothing. Setting to 1 will use bbm_heap_free() to free it.
    u8 opCode;
    u32 instants;
    u8 *data;
} Lmp_Instant_Desc_t;


/*
typedef enum {
    //TX
    LM_HEAD_ACL_TX_BUF,        //Lm_Tx_Acl_Buf_t,  LM_TOTAL_ACL_TX_BUF
    LM_HEAD_ACL_TX_BUF_USING,  //Lm_Tx_Acl_Buf_t,  0
    LM_HEAD_ACL_TX_DESC,       //Lm_Tx_Acl_Desc_t, LM_TOTAL_ACL_TX_DESC
    LM_HEAD_LMP_TX_BUF,        //Lm_Tx_Lmp_Buf_t,  LM_TOTAL_LMP_TX_BUF
    LM_HEAD_LMP_TX_BUF_USING,  //Lm_Tx_Lmp_Buf_t,  0
    LM_HEAD_LMP_TX_DESC,       //Lm_Tx_Lmp_Desc_t, LM_TOTAL_LMP_TX_DESC

    //RX
    LM_HEAD_ACL_RX_DESC,       //Lm_Rx_Acl_Desc_t, LM_TOTAL_ACL_RX_DESC
    LM_HEAD_LMP_RX_DESC,       //Lm_Rx_Lmp_Desc_t, LM_TOTAL_LMP_RX_DESC
    LM_HEAD_ACL_FROM_LC_DESC,  //Lm_Rx_Acl_Desc_t, 0
    LM_HEAD_LMP_FROM_LC_DESC,  //Lm_Rx_Lmp_Desc_t, 0
    LM_HEAD_RX_OCCUPID_DESC,   //doubly linked list, Lm_Rx_Desc_t, 0

    LM_HEAD_LMP_INSTANT_DESC,  //Lmp_Instant_Desc_t, LM_TOTAL_LMP_INSTANT_DESC

    LM_HEAD_TOTAL, // for array init
    
    LM_HEAD_INVALID = 255, //Make sure LM_HEAD_ID_t is fit to uint8_t
} LM_HEAD_ID_t;
*/

extern SLList_Head_t g_lm_head_acl_tx_buf;
extern SLList_Head_t g_lm_head_acl_tx_buf_using;
extern SLList_Head_t g_lm_head_acl_tx_desc;
extern SLList_Head_t g_lm_head_lmp_tx_buf;
extern SLList_Head_t g_lm_head_lmp_tx_buf_using;
extern SLList_Head_t g_lm_head_lmp_tx_desc;
extern SLList_Head_t g_lm_head_acl_rx_desc;
extern SLList_Head_t g_lm_head_lmp_rx_desc;
extern SLList_Head_t g_lm_head_acl_from_lc_desc;
extern SLList_Head_t g_lm_head_lmp_from_lc_desc;
extern DLList_Head_t g_lm_head_rx_occupied; //doubly linked list
extern SLList_Head_t g_lm_head_lmp_instant_desc;
#define HEAD_ACL_TX_BUF       &g_lm_head_acl_tx_buf
#define HEAD_ACL_TX_BUF_USING &g_lm_head_acl_tx_buf_using
#define HEAD_ACL_TX_DESC      &g_lm_head_acl_tx_desc
#define HEAD_LMP_TX_BUF       &g_lm_head_lmp_tx_buf
#define HEAD_LMP_TX_BUF_USING &g_lm_head_lmp_tx_buf_using
#define HEAD_LMP_TX_DESC      &g_lm_head_lmp_tx_desc
#define HEAD_ACL_RX_DESC      &g_lm_head_acl_rx_desc
#define HEAD_LMP_RX_DESC      &g_lm_head_lmp_rx_desc
#define HEAD_ACL_FROM_LC_DESC &g_lm_head_acl_from_lc_desc
#define HEAD_LMP_FROM_LC_DESC &g_lm_head_lmp_from_lc_desc
#define HEAD_RX_OCCUPIED      &g_lm_head_rx_occupied
#define HEAD_LMP_INSTANT_DESC &g_lm_head_lmp_instant_desc
void BT_LM_HeadsInit(void)
{
    UTL_SListHeadInit(HEAD_ACL_TX_BUF      );
    UTL_SListHeadInit(HEAD_ACL_TX_BUF_USING);
    UTL_SListHeadInit(HEAD_ACL_TX_DESC     );
    UTL_SListHeadInit(HEAD_LMP_TX_BUF      );
    UTL_SListHeadInit(HEAD_LMP_TX_BUF_USING);
    UTL_SListHeadInit(HEAD_LMP_TX_DESC     );
    UTL_SListHeadInit(HEAD_ACL_RX_DESC     );
    UTL_SListHeadInit(HEAD_LMP_RX_DESC     );
    UTL_SListHeadInit(HEAD_ACL_FROM_LC_DESC);
    UTL_SListHeadInit(HEAD_LMP_FROM_LC_DESC);
    UTL_DListHeadInit(HEAD_RX_OCCUPIED     );
    UTL_SListHeadInit(HEAD_LMP_INSTANT_DESC);
}

SLList_Head_t g_lm_head_acl_tx_buf;
SLList_Head_t g_lm_head_acl_tx_buf_using;
SLList_Head_t g_lm_head_acl_tx_desc;

SLList_Head_t g_lm_head_lmp_tx_buf;
SLList_Head_t g_lm_head_lmp_tx_buf_using;
SLList_Head_t g_lm_head_lmp_tx_desc;

SLList_Head_t g_lm_head_acl_rx_desc;
SLList_Head_t g_lm_head_lmp_rx_desc;
SLList_Head_t g_lm_head_acl_from_lc_desc;
SLList_Head_t g_lm_head_lmp_from_lc_desc;
DLList_Head_t g_lm_head_rx_occupied;

SLList_Head_t g_lm_head_lmp_instant_desc;

u8 *gLm_Tx_Acl_Buf; //LM_SINGLE_ACL_TX_BUF_SIZE x LM_TOTAL_ACL_TX_BUF
u8 *gLm_Tx_Lmp_Buf; //LM_SINGLE_LMP_TX_BUF_SIZE x LM_TOTAL_LMP_TX_BUF
/*
Lm_Tx_Acl_Buf_t  g_Lm_Tx_Acl_Buf[LM_TOTAL_ACL_TX_BUF];
Lm_Tx_Acl_Desc_t g_Lm_Tx_Acl_Desc[LM_TOTAL_ACL_TX_DESC];
Lm_Tx_Lmp_Buf_t  g_Lm_Tx_Lmp_Buf[LM_TOTAL_LMP_TX_BUF];
Lm_Tx_Lmp_Desc_t g_Lm_Tx_Lmp_Desc[LM_TOTAL_LMP_TX_DESC];
Lm_Rx_Acl_Desc_t g_Lm_Rx_Acl_Desc[LM_TOTAL_ACL_RX_DESC];
Lm_Rx_Lmp_Desc_t g_Lm_Rx_Lmp_Desc[LM_TOTAL_LMP_RX_DESC];
Lmp_Instant_Desc_t g_Lm_Lmp_Instant_Desc[LM_TOTAL_LMP_INSTANT_DESC];
*/

//Dynmaic for now
#define GET_TX_ACL_BUF_HANDLE(index)  (Lm_Tx_Acl_Buf_t *)bbm_heap_malloc(sizeof(Lm_Tx_Acl_Buf_t))
#define GET_TX_ACL_DESC_HANDLE(index) (Lm_Tx_Acl_Desc_t *)bbm_heap_malloc(sizeof(Lm_Tx_Acl_Desc_t))
#define GET_TX_LMP_BUF_HANDLE(index)  (Lm_Tx_Lmp_Buf_t *)bbm_heap_malloc(sizeof(Lm_Tx_Lmp_Buf_t))
#define GET_TX_LMP_DESC_HANDLE(index) (Lm_Tx_Lmp_Desc_t *)bbm_heap_malloc(sizeof(Lm_Tx_Lmp_Desc_t))

#define GET_RX_ACL_DESC_HANDLE(index) (Lm_Rx_Acl_Desc_t *)bbm_heap_malloc(sizeof(Lm_Rx_Acl_Desc_t))
#define GET_RX_LMP_DESC_HANDLE(index) (Lm_Rx_Lmp_Desc_t *)bbm_heap_malloc(sizeof(Lm_Rx_Lmp_Desc_t))

#define GET_LMP_INSTANT_DESC_HANDLE(index) (Lmp_Instant_Desc_t *)bbm_heap_malloc(sizeof(Lmp_Instant_Desc_t))

void BT_LM_TxBufAndDescInit(void)
{
    int i;

    gLm_Tx_Acl_Buf = (u8 *)bbm_cmn_malloc(LM_SINGLE_ACL_TX_BUF_SIZE_ALIGNED * LM_TOTAL_ACL_TX_BUF);
    gLm_Tx_Lmp_Buf = (u8 *)bbm_cmn_malloc(LM_SINGLE_LMP_TX_BUF_SIZE_ALIGNED * LM_TOTAL_LMP_TX_BUF);

    //ACL
    for (i=0; i<LM_TOTAL_ACL_TX_BUF; i++) {
        Lm_Tx_Acl_Buf_t *p_lm_tx_acl_buf = GET_TX_ACL_BUF_HANDLE(i);
        p_lm_tx_acl_buf->cmn_mem = gLm_Tx_Acl_Buf + (LM_SINGLE_ACL_TX_BUF_SIZE * i);
        UTL_SListInsertLast(HEAD_ACL_TX_BUF, (void *)p_lm_tx_acl_buf);
    }
    for (i=0; i<LM_TOTAL_ACL_TX_DESC; i++) {
        Lm_Tx_Acl_Desc_t *p_lm_tx_acl_desc = GET_TX_ACL_DESC_HANDLE(i);
        UTL_SListInsertLast(HEAD_ACL_TX_DESC, (void *)p_lm_tx_acl_desc);
    }

    //LMP
    for (i=0; i<LM_TOTAL_LMP_TX_BUF; i++) {
        Lm_Tx_Lmp_Buf_t *p_lm_tx_lmp_buf = GET_TX_LMP_BUF_HANDLE(i);
        p_lm_tx_lmp_buf->cmn_mem = gLm_Tx_Lmp_Buf + (LM_SINGLE_LMP_TX_BUF_SIZE * i);
        p_lm_tx_lmp_buf->is_dynamic = 0;
        UTL_SListInsertLast(HEAD_LMP_TX_BUF, (void *)p_lm_tx_lmp_buf);
    }
    for (i=0; i<LM_TOTAL_LMP_TX_DESC; i++) {
        Lm_Tx_Lmp_Desc_t *p_lm_tx_lmp_desc = GET_TX_LMP_DESC_HANDLE(i);
        p_lm_tx_lmp_desc->is_dynamic = 0;
        UTL_SListInsertLast(HEAD_LMP_TX_DESC, (void *)p_lm_tx_lmp_desc);
    }
}

void BT_LM_RxDescInit(void)
{
    int i;

    for (i=0; i<LM_TOTAL_ACL_RX_DESC; i++) {
        Lm_Rx_Acl_Desc_t *p_lm_rx_acl_desc = GET_RX_ACL_DESC_HANDLE(i);
        p_lm_rx_acl_desc->rx_desc.type = LM_PACKET_ACL;
        p_lm_rx_acl_desc->rx_desc.is_released = 0;
        UTL_SListInsertLast(HEAD_ACL_RX_DESC, (void *)p_lm_rx_acl_desc);
    }
    for (i=0; i<LM_TOTAL_LMP_RX_DESC; i++) {
        Lm_Rx_Lmp_Desc_t *p_lm_rx_lmp_desc = GET_RX_LMP_DESC_HANDLE(i);
        p_lm_rx_lmp_desc->rx_desc.type = LM_PACKET_LMP;
        p_lm_rx_lmp_desc->rx_desc.is_released = 0;
        UTL_SListInsertLast(HEAD_LMP_RX_DESC, (void *)p_lm_rx_lmp_desc);
    }
}

void BT_LM_LmpInstantDescInit(void)
{
    int i;

    for (i=0; i<LM_TOTAL_LMP_INSTANT_DESC; i++) {
        Lmp_Instant_Desc_t *p_lmp_instant_desc = GET_LMP_INSTANT_DESC_HANDLE(i);
        p_lmp_instant_desc->is_dynamic = 0;
        UTL_SListInsertLast(HEAD_LMP_INSTANT_DESC, (void *)p_lmp_instant_desc);
    }
}

void BT_LM_TotalBufInit(void)
{
    BT_LM_TxBufAndDescInit();
    BT_LM_RxDescInit();
    BT_LM_LmpInstantDescInit();
}


#define ____________TEST_CODE_X______________________
#define ____________TEST_CODE_X_____________________
#define ____________TEST_CODE_X____________________

void Dump_5_ListsX(void)
{
#define LIST_NAME "%12s : "
#define PRINT_LIST_INFO  printf("%sr%d - ", cp->type?"ACL":"LMP", cp->is_released)
#define PRINT_LIST_INFO2 printf("%sr%d - ",\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->type?"ACL":"LMP",\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->is_released)
/*
#define PRINT_LIST_INFO  printf("t%dd%dr%d - ", cp->type, cp->is_dynamic_alloc, cp->is_released)
#define PRINT_LIST_INFO2 printf("t%dd%dr%d - ",\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->type,\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->is_dynamic_alloc,\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->is_released)
*/

    Lm_Rx_Desc_t *cp;

    printf("\n" LIST_NAME, "ACL_RX_DESC");
    SLLIST_FOREACH(HEAD_ACL_RX_DESC, cp, Lm_Rx_Desc_t)
    {
        PRINT_LIST_INFO;
    }
    printf("\n");

    printf(LIST_NAME, "LMP_RX_DESC");
    SLLIST_FOREACH(HEAD_LMP_RX_DESC, cp, Lm_Rx_Desc_t)
    {
        PRINT_LIST_INFO;
    }
    printf("\n");

    printf(LIST_NAME, "ACL_FROM_LC");
    SLLIST_FOREACH(HEAD_ACL_FROM_LC_DESC, cp, Lm_Rx_Desc_t)
    {
        PRINT_LIST_INFO;
    }
    printf("\n");

    printf(LIST_NAME, "LMP_FROM_LC");
    SLLIST_FOREACH(HEAD_LMP_FROM_LC_DESC, cp, Lm_Rx_Desc_t)
    {
        PRINT_LIST_INFO;
    }
    printf("\n");

    printf(LIST_NAME, "RX_OCCUPIED");
    DLLIST_FOREACH(HEAD_RX_OCCUPIED, cp, Lm_Rx_Desc_t)
    {
        PRINT_LIST_INFO2;
    }
    printf("\n");
#undef LIST_NAME
#undef PRINT_LIST_INFO
#undef PRINT_LIST_INFO2
}

void BT_LM_RxLcPassAcl(Lm_Rx_Acl_Desc_t *rx_descriptor)
{
    UTL_SListInsertLastSafely(HEAD_ACL_FROM_LC_DESC, (void *)rx_descriptor);
    UTL_DListInsertLastSafely(HEAD_RX_OCCUPIED, &rx_descriptor->rx_desc.rx_occupid_entry);
}

void BT_LM_RxLcPassLmp(Lm_Rx_Lmp_Desc_t *rx_descriptor)
{
    UTL_SListInsertLastSafely(HEAD_LMP_FROM_LC_DESC, (void *)rx_descriptor);
    UTL_DListInsertLastSafely(HEAD_RX_OCCUPIED, &rx_descriptor->rx_desc.rx_occupid_entry);
}

Lm_Rx_Acl_Desc_t * BT_LM_RxLcGetAclDesc(void)
{
    return (Lm_Rx_Acl_Desc_t *)UTL_SListPopFirstSafely(HEAD_ACL_RX_DESC);
}

Lm_Rx_Lmp_Desc_t * BT_LM_RxLcGetLmpDesc(void)
{
    return (Lm_Rx_Lmp_Desc_t *)UTL_SListPopFirstSafely(HEAD_LMP_RX_DESC);
}

Lm_Rx_Acl_Desc_t *BT_LM_RxAclFromLcDescReadFirst(void)
{
    return (Lm_Rx_Acl_Desc_t *)SLLIST_FIRST(HEAD_ACL_FROM_LC_DESC);
}

Lm_Rx_Lmp_Desc_t *BT_LM_RxLmpFromLcDescReadFirst(void)
{
    return (Lm_Rx_Lmp_Desc_t *)SLLIST_FIRST(HEAD_LMP_FROM_LC_DESC);
}

/**
    return: true for these is any release happened
*/
int BT_LM_RxOccupidReleaseCheck(void)
{
    int ret = 0;
    void *curr_dnode;
    Lm_Rx_Desc_t *rx_descriptor;

    while (1)
    {
        curr_dnode = UTL_DListPopFirstSafely(HEAD_RX_OCCUPIED);
        if (curr_dnode == NULL) {
            break;
        }

        rx_descriptor = STRUCT_ENTRY(curr_dnode, Lm_Rx_Desc_t, rx_occupid_entry);
        
        if (rx_descriptor->is_released == 0) {
            //insert back
            UTL_DListInsertFirstSafely(HEAD_RX_OCCUPIED, curr_dnode);
            break;
        }

        //TBD: update BP
        //TBD_RxOccupationRelease(rx_descriptor);
        
        //start release proc
        ret = 1;
        LM_DBG_ASSERT(rx_descriptor->is_released == 1);
        rx_descriptor->is_released = 0;
        if (rx_descriptor->type == LM_PACKET_ACL) {
            UTL_SListInsertLastSafely(HEAD_ACL_RX_DESC, (void *)rx_descriptor);
        } else {
            UTL_SListInsertLastSafely(HEAD_LMP_RX_DESC, (void *)rx_descriptor);
        }
    }

    return ret;
}

void BT_LM_RxAclFromLcDescReleaseFirst(Lm_Rx_Acl_Desc_t *desc_to_release)
{
    Lm_Rx_Acl_Desc_t *first_desc = (Lm_Rx_Acl_Desc_t *)UTL_SListPopFirstSafely(HEAD_ACL_FROM_LC_DESC);

    //check the input descriptor is first one
    LM_DBG_ASSERT(first_desc != NULL);
    LM_DBG_ASSERT(first_desc->rx_desc.is_released == 0);
    desc_to_release->rx_desc.is_released = 1;
    LM_DBG_ASSERT(first_desc->rx_desc.is_released == 1);

    BT_LM_RxOccupidReleaseCheck();
}

void BT_LM_RxLmpFromLcDescReleaseFirst(Lm_Rx_Lmp_Desc_t *desc_to_release)
{
    Lm_Rx_Lmp_Desc_t *first_desc = (Lm_Rx_Lmp_Desc_t *)UTL_SListPopFirstSafely(HEAD_LMP_FROM_LC_DESC);

    //check the input descriptor is first one
    LM_DBG_ASSERT(first_desc != NULL);
    LM_DBG_ASSERT(first_desc->rx_desc.is_released == 0);
    desc_to_release->rx_desc.is_released = 1;
    LM_DBG_ASSERT(first_desc->rx_desc.is_released == 1);

    BT_LM_RxOccupidReleaseCheck();
}

int BT_LM_RxRecycleLmpDesc(void) //return 1 if any recycle happened
{
    void *curr_dnode;
    Lm_Rx_Desc_t *rx_descriptor;

    if (SLLIST_IS_NOT_EMPTY(HEAD_LMP_RX_DESC))
        return 0;

    // Recycled one won't be the last, no need to worry about thread safe issue even if LC is trying to insert last or to revert last.
    DLLIST_FOREACH(HEAD_RX_OCCUPIED, curr_dnode, void)
    {
        rx_descriptor = STRUCT_ENTRY(curr_dnode, Lm_Rx_Desc_t, rx_occupid_entry);
        if (rx_descriptor->type == LM_PACKET_LMP && rx_descriptor->is_released == 1)
        {
            DLLIST_REMOVE_NODE(HEAD_RX_OCCUPIED, curr_dnode);
            rx_descriptor->is_released = 0; //reset it
            UTL_SListInsertLastSafely(HEAD_LMP_RX_DESC, (void *)rx_descriptor);
            return 1;
        }
    }
    return 0;
}

int BT_LM_RxOccupidRevertLastLmpDesc(void)
{
    Lm_Rx_Desc_t *rx_descriptor;

    if (SLLIST_IS_NOT_EMPTY(HEAD_LMP_RX_DESC))
        return 0;

    //use read-approach, this is more complicated!!!
    if (DLLIST_IS_NOT_EMPTY(HEAD_RX_OCCUPIED))
    {
        rx_descriptor = (Lm_Rx_Desc_t *)DLLIST_TAIL(HEAD_RX_OCCUPIED);
        rx_descriptor = STRUCT_ENTRY(rx_descriptor, Lm_Rx_Desc_t, rx_occupid_entry);

        if (rx_descriptor->is_released)
        {
            if (rx_descriptor->type == LM_PACKET_LMP)
            {
                //own this descriptor and release it
                rx_descriptor = (Lm_Rx_Desc_t *)UTL_DListPopLastSafely(HEAD_RX_OCCUPIED);
                rx_descriptor = STRUCT_ENTRY(rx_descriptor, Lm_Rx_Desc_t, rx_occupid_entry);
                //make sure popped desc and  read desc are the same
                LM_DBG_ASSERT(rx_descriptor != NULL);
                LM_DBG_ASSERT(rx_descriptor->is_released);
                LM_DBG_ASSERT(rx_descriptor->type == LM_PACKET_LMP);
                TBD_RxOccupationReverse(rx_descriptor->buf_len);
                rx_descriptor->is_released = 0;
                UTL_SListInsertLastSafely(HEAD_LMP_RX_DESC, (void *)rx_descriptor);
                return 1;
            }
        }
    }

    return 0;
}

void LC_Send_ACLx(void)
{
    Lm_Rx_Acl_Desc_t *cp = BT_LM_RxLcGetAclDesc();
    BT_LM_RxLcPassAcl(cp);
}

void LC_Send_LMPx(void)
{
    Lm_Rx_Lmp_Desc_t *cp = BT_LM_RxLcGetLmpDesc();
    BT_LM_RxLcPassLmp(cp);
}

void LC_Ask_LM_Recycle_LMP_Desc(void)
{
    BT_LM_RxRecycleLmpDesc();
}


void HCI_Execute_ACL_FROM_LC(void)
{
    Lm_Rx_Acl_Desc_t *cp = BT_LM_RxAclFromLcDescReadFirst();
    if (cp != NULL)
    {
        //...
        BT_LM_RxAclFromLcDescReleaseFirst(cp);
    }
}

void LMP_Execute_LMP_FROM_LC(void)
{
    Lm_Rx_Lmp_Desc_t *cp = BT_LM_RxLmpFromLcDescReadFirst();
    if (cp != NULL)
    {
        //...
        BT_LM_RxLmpFromLcDescReleaseFirst(cp);
    }
}

Lmp_Instant_Desc_t *BT_LM_InstantDescGet(void)
{
    Lmp_Instant_Desc_t *p_lmp_instant_desc = (Lmp_Instant_Desc_t *)UTL_SListPopFirstSafely(HEAD_LMP_INSTANT_DESC);
    if (p_lmp_instant_desc == NULL)
    {
        p_lmp_instant_desc = (Lmp_Instant_Desc_t *)bbm_heap_malloc(sizeof(Lmp_Instant_Desc_t));
        if (p_lmp_instant_desc == NULL)
            return NULL;
        p_lmp_instant_desc->is_dynamic = 1;
        p_lmp_instant_desc->data = NULL;
    }
    return p_lmp_instant_desc;
}

void BT_LM_InstantDescRelease(Lmp_Instant_Desc_t *p_lmp_instant_desc)
{
    if (p_lmp_instant_desc->is_need_to_free_data)
    {
        if (p_lmp_instant_desc->data != NULL)
        {
            bbm_heap_free(p_lmp_instant_desc->data);
        }
    }

    if (p_lmp_instant_desc->is_dynamic == 1)
    {
        bbm_heap_free(p_lmp_instant_desc);
    }
    else
    {
        UTL_SListInsertLastSafely(HEAD_LMP_INSTANT_DESC, (void *)p_lmp_instant_desc);
    }
}

void BT_LM_InstantDescInsertLast(SLList_Head_t *p_head, Lmp_Instant_Desc_t *p_lmp_instant_desc)
{
    UTL_SListInsertLastSafely(p_head, (void *)p_lmp_instant_desc);
}

//TBD
void BT_LM_InstantDescInsertLastByDev(void *dev, Lmp_Instant_Desc_t *p_lmp_instant_desc)
{
    UTL_SListInsertLastSafely((SLList_Head_t *)dev, (void *)p_lmp_instant_desc);
}

//Only been used in BBM, no thread safe protection
u8 *BT_LM_AclTxBufferGet(void)
{
    Lm_Tx_Acl_Buf_t *buf_desc = (Lm_Tx_Acl_Buf_t *)UTL_SListPopFirst(HEAD_ACL_TX_BUF);

    if (buf_desc == NULL)
        return NULL;

    UTL_SListInsertLast(HEAD_ACL_TX_BUF_USING, (void *)buf_desc);
    return buf_desc->cmn_mem;
}

//Only been used in BBM, no thread safe protection
int BT_LM_AclTxBufferRelease(u8 *buf)
{
    Lm_Tx_Acl_Buf_t *curr, *prev;

    prev = (Lm_Tx_Acl_Buf_t *)HEAD_ACL_TX_BUF_USING;
    SLLIST_FOREACH(HEAD_ACL_TX_BUF_USING, curr, Lm_Tx_Acl_Buf_t)
    {
        if (curr->cmn_mem == buf)
        {
            SLLIST_REMOVE_NEXT(HEAD_ACL_TX_BUF_USING, prev);
            UTL_SListInsertLast(HEAD_ACL_TX_BUF, (void *)curr);
            return 0;
        }
    }

    return 1;
}

//Only been used in BBM, no thread safe protection
u8 *BT_LM_LmpTxBufferGet(u32 size_if_need_malloc)
{
    Lm_Tx_Lmp_Buf_t *buf_desc = (Lm_Tx_Lmp_Buf_t *)UTL_SListPopFirst(HEAD_LMP_TX_BUF);

    if (buf_desc == NULL)
    {
        return (u8 *)bbm_cmn_malloc(size_if_need_malloc);
    }

    UTL_SListInsertLast(HEAD_LMP_TX_BUF_USING, (void *)buf_desc);
    return buf_desc->cmn_mem;
}

//Only been used in BBM, no thread safe protection
int BT_LM_LmpTxBufferRelease(u8 *buf)
{
    Lm_Tx_Lmp_Buf_t *curr, *prev;

    prev = (Lm_Tx_Lmp_Buf_t *)HEAD_LMP_TX_BUF_USING;
    SLLIST_FOREACH(HEAD_LMP_TX_BUF_USING, curr, Lm_Tx_Lmp_Buf_t)
    {
        if (curr->cmn_mem == buf)
        {
            SLLIST_REMOVE_NEXT(HEAD_LMP_TX_BUF_USING, prev);
            UTL_SListInsertLast(HEAD_LMP_TX_BUF, (void *)curr);
            return 0;
        }
    }

    bbm_cmn_free(buf);
    return 1;
}

void Dump_Tx_Buf_ListsX(void)
{
#define LIST_NAME "%-16s : "
#define PRINT_LIST_INFO  printf("%sr%d - ", cp->type?"ACL":"LMP", cp->is_released)
#define PRINT_LIST_INFO2 printf("%sr%d - ",\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->type?"ACL":"LMP",\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->is_released)
/*
#define PRINT_LIST_INFO  printf("t%dd%dr%d - ", cp->type, cp->is_dynamic_alloc, cp->is_released)
#define PRINT_LIST_INFO2 printf("t%dd%dr%d - ",\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->type,\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->is_dynamic_alloc,\
    STRUCT_ENTRY(cp, Lm_Rx_Desc_t, rx_occupid_entry)->is_released)
*/

    {
        Lm_Tx_Acl_Buf_t *cp;

        printf(LIST_NAME, "ACL TX BUF");
        SLLIST_FOREACH(HEAD_ACL_TX_BUF, cp, Lm_Tx_Acl_Buf_t)
        {
            printf("0x%08X - ", (u32)cp->cmn_mem);
        }
        printf("\n");

        printf(LIST_NAME, "ACL TX BUF_USING");
        SLLIST_FOREACH(HEAD_ACL_TX_BUF_USING, cp, Lm_Tx_Acl_Buf_t)
        {
            printf("0x%08X - ", (u32)cp->cmn_mem);
        }
        printf("\n");
    }

    {
        Lm_Tx_Lmp_Buf_t *cp;

        printf(LIST_NAME, "LMP TX BUF");
        SLLIST_FOREACH(HEAD_LMP_TX_BUF, cp, Lm_Tx_Lmp_Buf_t)
        {
            printf("[%d]0x%08X - ", cp->is_dynamic, (u32)cp->cmn_mem);
        }
        printf("\n");

        printf(LIST_NAME, "LMP TX BUF_USING");
        SLLIST_FOREACH(HEAD_LMP_TX_BUF_USING, cp, Lm_Tx_Lmp_Buf_t)
        {
            printf("[%d]0x%08X - ", cp->is_dynamic, (u32)cp->cmn_mem);
        }
        printf("\n");
    }
}

void Chimera_TestX(void)
{
    BT_LM_HeadsInit();
    BT_LM_TotalBufInit();

    u8 *buf;
    buf = BT_LM_AclTxBufferGet();
    BT_LM_AclTxBufferRelease(buf);
    buf = BT_LM_LmpTxBufferGet(2);
    Dump_Tx_Buf_ListsX();
    return;
    Dump_5_ListsX();

    LC_Send_ACLx();
    LC_Send_LMPx();
    LC_Send_LMPx();
    LC_Send_LMPx();
    LC_Send_ACLx();
    LMP_Execute_LMP_FROM_LC();
    LMP_Execute_LMP_FROM_LC();
    LC_Send_LMPx();
    LMP_Execute_LMP_FROM_LC();
    LMP_Execute_LMP_FROM_LC();
    Dump_5_ListsX();
    
    //LC_Ask_LM_Recycle_LMP_Desc();
    BT_LM_RxOccupidRevertLastLmpDesc();
    Dump_5_ListsX();

    {
        Lmp_Instant_Desc_t *pDesc;

        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        BT_LM_InstantDescRelease(pDesc);
        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        pDesc = BT_LM_InstantDescGet();
        DUMPA(pDesc);DUMPD(pDesc->is_dynamic);
        
    }
}

#define ____________TEST_CODE______________________
#define ____________TEST_CODE_____________________
#define ____________TEST_CODE____________________

void CP_Minus_Test(void);

void Chimera_Test(void)
{
    Chimera_TestX();
    return;
    //LC step 1:   is rx buf enough
    //LC step 2:   is acl?
    //LC step 2-1: is BT_LM_RxEmptyAclDescPopFirst() ok? no = nack
    //LC step 3:   is lmp?
    //LC step 3-1: is BT_LM_RxOccupidRevertReleaseLmpDesc() ok?
    //LC step 3-2: is BT_LM_RxEmptyLmpDescPopFirst() ok?
}


#define ___RX______________________________
#define ___RX_____________________________
#define ___RX____________________________

typedef struct {
    u8 *start_ptr;   //LC
    u8 *end_ptr;     //LC
    u32 free_index;  //LC, used in cp
    u32 bp_index;    //BBM
    u32 remain_size; //LC

    u8 *ble_dummy_buf; //size = 255
} Lm_Rx_Buf_Info_t;

#define TURN_AROUND_MINUS(a,b,end)        (a>=b)?a-b:(end+a)-b
#define TURN_AROUND_ADD_TEST(a,b,end)     (a+b>=end)
#define TURN_AROUND_ADD_NORMAL(a,b)       (a+b)
#define TURN_AROUND_ADD_OVERFLOW(a,b,end) ((a+b)-end)
#define TURN_AROUND_ADD(a,b,end)          (TURN_AROUND_ADD_TEST(a,b,end))?TURN_AROUND_ADD_OVERFLOW(a,b,end):(TURN_AROUND_ADD_NORMAL(a,b))

#if 0
#define LM_RX_BUF_MAX_LEN 4096
#define LM_RX_BUF_BLE_DUMMY_BUF_LEN 255
#else
#define LM_RX_BUF_MAX_LEN 1024
#define LM_RX_BUF_BLE_DUMMY_BUF_LEN 255
#endif

Lm_Rx_Buf_Info_t g_lm_rx_buf_info;
#define LM_RX_FREE_INDEX  g_lm_rx_buf_info.free_index
#define LM_RX_EP_INDEX    (LM_RX_BUF_MAX_LEN-1)
#define LM_RX_BP_INDEX    g_lm_rx_buf_info.bp_index
#define LM_RX_REMAIN_SIZE g_lm_rx_buf_info.remain_size
#define LM_RX_USED_SIZE   (LM_RX_BUF_MAX_LEN-1-LM_RX_REMAIN_SIZE)
#define LM_RX_USED_INDEX  (TURN_AROUND_ADD(LM_RX_BP_INDEX,1,LM_RX_BUF_MAX_LEN))
#define LM_RX_SP          (g_lm_rx_buf_info.start_ptr)
#define LM_RX_EP          (g_lm_rx_buf_info.end_ptr)
#define LM_RX_FREE_PTR    (LM_RX_SP+LM_RX_FREE_INDEX)
#define LM_RX_CP          LM_RX_FREE_PTR
#define LM_RX_BP          (LM_RX_SP+LM_RX_BP_INDEX)

void lm_rx_buf_init(void)
{
#if 1
    g_lm_rx_buf_info.start_ptr = (u8 *)0x1000;
#else
    g_lm_rx_buf_info.start_ptr = (u8 *)bbm_cmn_malloc(LM_RX_BUF_MAX_LEN);
#endif
    g_lm_rx_buf_info.end_ptr = g_lm_rx_buf_info.start_ptr + LM_RX_EP_INDEX;

    //DIRECT_RFIELD_PDU_PL_SP = (u32)g_lm_rx_buf_info.start_ptr;
    //DIRECT_RFIELD_PDU_PL_EP = (u32)g_lm_rx_buf_info.end_ptr;

    g_lm_rx_buf_info.free_index = 0;
    g_lm_rx_buf_info.bp_index = LM_RX_EP_INDEX;
    g_lm_rx_buf_info.remain_size = TURN_AROUND_MINUS(LM_RX_BP_INDEX, LM_RX_FREE_INDEX, LM_RX_BUF_MAX_LEN);

    g_lm_rx_buf_info.ble_dummy_buf = (u8 *)bbm_cmn_malloc(LM_RX_BUF_BLE_DUMMY_BUF_LEN);
}

void lm_rx_buf_curr_info_update(void)
{
    LM_RX_REMAIN_SIZE = TURN_AROUND_MINUS(LM_RX_BP_INDEX, LM_RX_FREE_INDEX, LM_RX_BUF_MAX_LEN);
}

Lm_Rx_Buf_Info_t *lm_rx_buf_curr_info_get(void)
{
    return &g_lm_rx_buf_info;
}

u8 *lm_rx_buf_cp_get(void)
{
    return LM_RX_CP;
}

u8 *lm_rx_buf_bp_get(void)
{
    return LM_RX_CP;
}


//Return true for turn around
int lm_rx_buf_occupation_set(u32 rx_len)
{
    int is_trun_around = 0;

    //rx_len must be smaller than remain size
    LM_DBG_ASSERT(rx_len <= LM_RX_REMAIN_SIZE);

    LM_RX_REMAIN_SIZE -= rx_len;

    //update end index
    if (TURN_AROUND_ADD_TEST(LM_RX_FREE_INDEX, rx_len, LM_RX_BUF_MAX_LEN))
    {
        LM_RX_FREE_INDEX = TURN_AROUND_ADD_OVERFLOW(LM_RX_FREE_INDEX, rx_len, LM_RX_BUF_MAX_LEN);
        is_trun_around = 1;
    }
    else
    {
        LM_RX_FREE_INDEX = TURN_AROUND_ADD_NORMAL(LM_RX_FREE_INDEX, rx_len);
    }

    return is_trun_around;
}

void lm_rx_buf_occupation_release_from_upper(u32 release_len)
{
    LM_DBG_ASSERT(release_len <= LM_RX_USED_SIZE);

    LM_RX_BP_INDEX = TURN_AROUND_ADD(LM_RX_BP_INDEX, release_len, LM_RX_BUF_MAX_LEN);
}

void lm_rx_buf_occupation_release_by_old_cp_from_upper(u8 *old_cp, u32 old_len)
{
    u32 old_free_index = old_cp - g_lm_rx_buf_info.start_ptr;
    u32 new_bp = (TURN_AROUND_ADD(old_free_index, old_len, LM_RX_BUF_MAX_LEN)) - 1;

    LM_RX_BP_INDEX = new_bp;
}

void lm_rx_buf_occupation_revert(u32 revert_len)
{
    LM_RX_REMAIN_SIZE += revert_len;

    LM_RX_FREE_INDEX = TURN_AROUND_MINUS(LM_RX_FREE_INDEX, revert_len, LM_RX_BUF_MAX_LEN);
}

/*
    Output: *p_out_len_to_turn_around
                0 : no turn around
                others : length to turn around index
*/
u32 lm_rx_buf_calc_increased_index(u32 rx_buf_index, u32 increment, u32 *p_out_len_to_turn_around)
{
    u32 len_to_turn_around = 0;
    u32 new_index;

    if (TURN_AROUND_ADD_TEST(rx_buf_index, increment, LM_RX_BUF_MAX_LEN))
    {
        new_index = TURN_AROUND_ADD_OVERFLOW(rx_buf_index, increment, LM_RX_BUF_MAX_LEN);
        len_to_turn_around = LM_RX_BUF_MAX_LEN - rx_buf_index;
    }
    else
    {
        new_index = TURN_AROUND_ADD_NORMAL(rx_buf_index, increment);
    }

    if (p_out_len_to_turn_around != NULL)
    {
        *p_out_len_to_turn_around = len_to_turn_around;
    }

    return new_index;
}

void Dump_Rx_Buf_Info(void)
{
    u32 x;
    x = (u32)g_lm_rx_buf_info.start_ptr;
    printf("sp = 0x%X\n", x);
    x = (u32)g_lm_rx_buf_info.end_ptr;
    printf("ep = 0x%X\n", x);

    printf("cp = 0x%X\n", g_lm_rx_buf_info.free_index);

    printf("bp = 0x%X\n", g_lm_rx_buf_info.bp_index);

    printf("remain = 0x%X\n", g_lm_rx_buf_info.remain_size);
}

void CP_Minus_Test(void)
{
    lm_rx_buf_init();
    Dump_Rx_Buf_Info();

    lm_rx_buf_occupation_set(0x40);
    Dump_Rx_Buf_Info();

    lm_rx_buf_occupation_revert(0x1);
    Dump_Rx_Buf_Info();
    return;
    lm_rx_buf_occupation_release_by_old_cp_from_upper((u8 *)0x1010, 0x10);
    lm_rx_buf_curr_info_update();
    Dump_Rx_Buf_Info();
}

