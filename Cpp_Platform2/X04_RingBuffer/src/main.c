
#include "Everything_App.hpp"

#define bbm_cmn_malloc malloc
#define LM_DBG_ASSERT BASIC_ASSERT

#define TRUN_AROUND_MINUS(a,b,end)        (a>=b)?a-b:(end+a)-b
#define TRUN_AROUND_ADD_TEST(a,b,end)     (a+b>=end)
#define TRUN_AROUND_ADD_NORMAL(a,b)       (a+b)
#define TRUN_AROUND_ADD_OVERFLOW(a,b,end) ((a+b)-end)
#define TRUN_AROUND_ADD(a,b,end)          (TRUN_AROUND_ADD_TEST(a,b,end))?TRUN_AROUND_ADD_OVERFLOW(a,b,end):(TRUN_AROUND_ADD_NORMAL(a,b))

typedef struct {
    u8 *start_ptr;   //LC
    u32 free_index;  //LC, used in cp
    u32 ep_index;    //LC
    u32 bp_index;    //BBM
    u32 remain_size; //LC

    u8 *ble_dummy_buf; //size = 255
} Lm_Rx_Buf_Info_t;

#if 0
#define LM_RX_BUF_MAX_LEN 4096
#define LM_RX_BUF_BLE_DUMMY_BUF_LEN 255
#else
#define LM_RX_BUF_MAX_LEN 32
#define LM_RX_BUF_BLE_DUMMY_BUF_LEN 5
#endif

Lm_Rx_Buf_Info_t g_lm_rx_buf_info;
#define LM_RX_FREE_INDEX  g_lm_rx_buf_info.free_index
#define LM_RX_EP_INDEX    g_lm_rx_buf_info.ep_index
#define LM_RX_BP_INDEX    g_lm_rx_buf_info.bp_index
#define LM_RX_REMAIN_SIZE g_lm_rx_buf_info.remain_size
#define LM_RX_USED_SIZE   (LM_RX_BUF_MAX_LEN-1-LM_RX_REMAIN_SIZE)
#define LM_RX_USED_INDEX  (TRUN_AROUND_ADD(LM_RX_BP_INDEX,1,LM_RX_BUF_MAX_LEN))
#define LM_RX_SP          (g_lm_rx_buf_info.start_ptr)
#define LM_RX_FREE_PTR    (LM_RX_SP+LM_RX_FREE_INDEX)
#define LM_RX_CP          LM_RX_FREE_PTR
#define LM_RX_EP          (LM_RX_SP+LM_RX_EP_INDEX)
#define LM_RX_BP          (LM_RX_SP+LM_RX_BP_INDEX)

void lm_rx_buf_init(void)
{
    g_lm_rx_buf_info.start_ptr = (u8 *)bbm_cmn_malloc(LM_RX_BUF_MAX_LEN);

    LM_RX_FREE_INDEX = 0;
    LM_RX_EP_INDEX = LM_RX_BUF_MAX_LEN-1;
    LM_RX_BP_INDEX = g_lm_rx_buf_info.ep_index;
    LM_RX_REMAIN_SIZE = TRUN_AROUND_MINUS(LM_RX_BP_INDEX, LM_RX_FREE_INDEX, LM_RX_BUF_MAX_LEN);

    g_lm_rx_buf_info.ble_dummy_buf = (u8 *)bbm_cmn_malloc(LM_RX_BUF_BLE_DUMMY_BUF_LEN);
}

void lm_rx_buf_curr_info_update(void)
{
    LM_RX_REMAIN_SIZE = TRUN_AROUND_MINUS(LM_RX_BP_INDEX, LM_RX_FREE_INDEX, LM_RX_BUF_MAX_LEN);
}

Lm_Rx_Buf_Info_t *lm_rx_buf_curr_info_get(void)
{
    lm_rx_buf_curr_info_update();
    return &g_lm_rx_buf_info;
}

u8 *lm_rx_buf_curr_ptr_get(void)
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
    if (TRUN_AROUND_ADD_TEST(LM_RX_FREE_INDEX, rx_len, LM_RX_BUF_MAX_LEN))
    {
        LM_RX_FREE_INDEX = TRUN_AROUND_ADD_OVERFLOW(LM_RX_FREE_INDEX, rx_len, LM_RX_BUF_MAX_LEN);
        is_trun_around = 1;
    }
    else
    {
        LM_RX_FREE_INDEX = TRUN_AROUND_ADD_NORMAL(LM_RX_FREE_INDEX, rx_len);
    }

    return is_trun_around;
}

void lm_rx_buf_occupation_release_from_upper(u32 release_len)
{
    LM_DBG_ASSERT(release_len <= LM_RX_USED_SIZE);

    LM_RX_BP_INDEX = TRUN_AROUND_ADD(LM_RX_BP_INDEX, release_len, LM_RX_BUF_MAX_LEN);
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

    if (TRUN_AROUND_ADD_TEST(rx_buf_index, increment, LM_RX_BUF_MAX_LEN))
    {
        new_index = TRUN_AROUND_ADD_OVERFLOW(rx_buf_index, increment, LM_RX_BUF_MAX_LEN);
        len_to_turn_around = LM_RX_BUF_MAX_LEN - rx_buf_index;
    }
    else
    {
        new_index = TRUN_AROUND_ADD_NORMAL(rx_buf_index, increment);
    }

    if (p_out_len_to_turn_around != NULL)
    {
        *p_out_len_to_turn_around = len_to_turn_around;
    }

    return new_index;
}

void Test_Init_Lm_Rx(void)
{
    u32 i;
    for (i = 0; i < LM_RX_BUF_MAX_LEN; i++) {
        LM_RX_SP[i] = i%8;
    }
    for (i = 0; i < LM_RX_BUF_BLE_DUMMY_BUF_LEN; i++) {
        g_lm_rx_buf_info.ble_dummy_buf[i] = 0xDD;
    }
}

void Test_Rx_BREDR(u8 *input_buf, u32 input_len)
{
    u32 i;
    u32 index;
    Lm_Rx_Buf_Info_t *rx_info = lm_rx_buf_curr_info_get();

    printf("%s START, len=%d\n", __func__, input_len);
    if (rx_info->remain_size > 0)
    {
        if(input_len <= rx_info->remain_size)
        {
            for (i=0; i<input_len; i++)
            {
                index = TRUN_AROUND_ADD(rx_info->free_index, i, LM_RX_BUF_MAX_LEN);
                rx_info->start_ptr[index] = input_buf[i];
            }
            lm_rx_buf_occupation_set(input_len);
            printf("%s SUCCESS!!!\n\n", __func__);
        }
        else
        {
            printf("%s FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! rx len too big(%d)\n\n", __func__, input_len);
        }
    }
    else
    {
        printf("%s FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! no remain size\n\n", __func__);
    }
}

void Test_Rx_BLE(u8 *input_buf, u32 input_len)
{
    u32 i;
    u32 index;
    Lm_Rx_Buf_Info_t *rx_info = lm_rx_buf_curr_info_get();

    printf("%s START, len=%d\n", __func__, input_len);
    BASIC_ASSERT(input_len<LM_RX_BUF_BLE_DUMMY_BUF_LEN);
    if (rx_info->remain_size >= LM_RX_BUF_BLE_DUMMY_BUF_LEN)
    {
        for (i=0; i<input_len; i++)
        {
            index = TRUN_AROUND_ADD(rx_info->free_index, i, LM_RX_BUF_MAX_LEN);
            rx_info->start_ptr[index] = input_buf[i];
        }
        lm_rx_buf_occupation_set(input_len);
        printf("%s SUCCESS!!!\n\n", __func__);
    }
    else
    {
        for (i=0; i<input_len; i++)
        {
            rx_info->ble_dummy_buf[i] = input_buf[i];
        }
        printf("%s FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! remain size not enough\n\n", __func__);
    }
}


void Dump_Lm_Rx(void)
{
    u32 i;
    printf("------ ====== DUMP START ====== ------\n");
    printf("fi:%4d, bi:%4d, remain:%4d, used:%4d\n", LM_RX_FREE_INDEX, LM_RX_BP_INDEX, LM_RX_REMAIN_SIZE, LM_RX_USED_SIZE);
    for (i = 0; i < LM_RX_BUF_MAX_LEN; i++) {
        printf("0x%02X ", LM_RX_SP[i]);
        if (i%8==7)
            printf(" ");
        if (i%16==15)
            printf("\n");
    }
    printf("\nFree Buf(size=%d):\n", LM_RX_REMAIN_SIZE);
    if (LM_RX_REMAIN_SIZE != 0)
    {
        u32 real_index;
        for (i = 0; i < LM_RX_REMAIN_SIZE; i++) {
            real_index = TRUN_AROUND_ADD(LM_RX_FREE_INDEX, i, LM_RX_BUF_MAX_LEN);
            printf("0x%02X ", LM_RX_SP[real_index]);
            if (i%8==7)
                printf(" ");
            if (i%16==15)
                printf("\n");
        }
        printf("\n");
    }
    printf("\nUsed Buf(size=%d):\n", LM_RX_USED_SIZE);
    if (LM_RX_USED_SIZE != 0)
    {
        u32 used_index;
        for (i = 0; i < LM_RX_USED_SIZE; i++) {
            used_index = LM_RX_USED_INDEX;
            used_index = TRUN_AROUND_ADD(used_index, i, LM_RX_BUF_MAX_LEN);
            printf("0x%02X ", LM_RX_SP[used_index]);
            if (i%8==7)
                printf(" ");
            if (i%16==15)
                printf("\n");
        }
        printf("\n");
    }
    printf("\nBLE Dummy Buf(size=%d):\n", LM_RX_BUF_BLE_DUMMY_BUF_LEN);
    if (LM_RX_BUF_BLE_DUMMY_BUF_LEN != 0)
    {
        for (i = 0; i < LM_RX_BUF_BLE_DUMMY_BUF_LEN; i++) {
            printf("0x%02X ", g_lm_rx_buf_info.ble_dummy_buf[i]);
            if (i%8==7)
                printf(" ");
            if (i%16==15)
                printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    u32 bi = 4095;
    u32 ci = 0;
    u32 end = 4096;
    u32 remain;
    u32 is_trun_around = 0;


    printf("<<< Turn around test >>>\n");
    remain = TRUN_AROUND_MINUS(bi,ci,end);
    printf("bi=%4d, ci=%4d, remain=%4d\n", bi, ci, remain);

    ci=4094;
    remain = TRUN_AROUND_MINUS(bi,ci,end);
    printf("bi=%4d, ci=%4d, remain=%4d\n", bi, ci, remain);

    ci=4095;
    remain = TRUN_AROUND_MINUS(bi,ci,end);
    printf("bi=%4d, ci=%4d, remain=%4d\n", bi, ci, remain);

    is_trun_around = TRUN_AROUND_ADD_TEST(bi, 1, end);
    bi = TRUN_AROUND_ADD(bi, 1, end);
    printf("bi=%4d,                          is_trun_around = %d\n", bi, is_trun_around);
    remain = TRUN_AROUND_MINUS(bi,ci,end);
    printf("bi=%4d, ci=%4d, remain=%4d\n", bi, ci, remain);

    is_trun_around = TRUN_AROUND_ADD_TEST(bi, 1, end);
    bi = TRUN_AROUND_ADD(bi, 1, end);
    printf("bi=%4d,                          is_trun_around = %d\n", bi, is_trun_around);
    remain = TRUN_AROUND_MINUS(bi,ci,end);
    printf("bi=%4d, ci=%4d, remain=%4d\n", bi, ci, remain);

    is_trun_around = TRUN_AROUND_ADD_TEST(bi, 1, end);
    bi = TRUN_AROUND_ADD(bi, 1, end);
    printf("bi=%4d,                          is_trun_around = %d\n", bi, is_trun_around);
    remain = TRUN_AROUND_MINUS(bi,ci,end);
    printf("bi=%4d, ci=%4d, remain=%4d\n", bi, ci, remain);

    printf("\n\n<<< Rx API test >>>\n");
    lm_rx_buf_init();

    Test_Init_Lm_Rx();
    Dump_Lm_Rx();

    {
        u8 testBuf[] = {0xF1, 0xF2, 0xF3, 0xF4};PRINT_LINE;
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

    {
        u8 testBuf[] = {0xD1, 0xD2, 0xD3, 0xD4, 0xD1, 0xD2, 0xD3, 0xD4};PRINT_LINE;
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

    {
        u8 testBuf[] = {0xC1, 0xC2, 0xC3, 0xC1, 0xC2, 0xC3, 0xC1, 0xC2, 0xC3};PRINT_LINE;
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

    {
        u8 testBuf[] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7};PRINT_LINE;
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

    {
        u8 testBuf[] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7};PRINT_LINE;
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

#if 0
    {
        u8 testBuf[] = {0xA1, 0xA2, 0xA3};
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }
#endif
    {
        u8 testBuf[] = {0xFF, 0xEE, 0xDD, 0xCC};PRINT_LINE;
        Test_Rx_BLE(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }
    {
        u8 testBuf[] = {0x11, 0x22};PRINT_LINE;
        Test_Rx_BLE(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

    PRINT_LINE;
    lm_rx_buf_occupation_release_from_upper(2);
    Dump_Lm_Rx();

    lm_rx_buf_curr_info_update();
    PRINT_LINE;
    Dump_Lm_Rx();
    /*
    {
        u8 testBuf[] = {0x33, 0x44, 0x55};PRINT_LINE;
        Test_Rx_BREDR(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }
    */

    {
        u8 testBuf[] = {0x11, 0x22, 0x33, 0x44};PRINT_LINE;
        Test_Rx_BLE(testBuf, sizeof(testBuf));
        Dump_Lm_Rx();
    }

    u32 abcc;
    u32 ltta;
    abcc = lm_rx_buf_calc_increased_index(30, 1, &ltta);
    printf("abcc=%4d, ltta=%d\n", abcc, ltta);
    abcc = lm_rx_buf_calc_increased_index(31, 1, &ltta);
    printf("abcc=%4d, ltta=%d\n", abcc, ltta);
    abcc = lm_rx_buf_calc_increased_index(31, 2, &ltta);
    printf("abcc=%4d, ltta=%d\n", abcc, ltta);
    abcc = lm_rx_buf_calc_increased_index(31, 3, &ltta);
    printf("abcc=%4d, ltta=%d\n", abcc, ltta);
    
    return 0;
}

