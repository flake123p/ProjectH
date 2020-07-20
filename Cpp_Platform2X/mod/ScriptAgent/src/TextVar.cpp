#include "Everything_ThisMod.hpp"

void BLE_LM_CmnGenAccessAddr()
{
    int     pass_check;
    uint8_t  breakCnt = 0;
    uint8_t  randidx;
    uint32_t accessAddr;
    uint32_t noMoreThanSixZerosInRowMsk = 0x02040810;//0b00000010 00000100 00001000 00010000
    uint32_t noMoreThanSixOnessInRowMsk = 0xFBF7EFDF;//0b11111011 11110111 11101111 11011111
    uint32_t msbSixBitsAllZerosMsk = 0x03FFFFFF;//0b00000011 11111111 11111111 11111111
    uint8_t msbSixBitsTwoTransitionMsk[16] = {0xC0, 0x60, 0x30, 0x18, 0x0C, 0xBC, 0xDC, 0xEC, 0xF4, 0xF8, 0xE0, 0x70, 0x38, 0x1C, 0xF0, 0x78};

    LibUtil_InitRand();
    do
    {
        pass_check = 1;

        accessAddr = (uint32_t)LibUtil_GetRand32();
        //BT_SYS_RandGet((uint8_t*)&accessAddr, 4);
        randidx = (uint8_t)(accessAddr & 0xF);

        //Rule1: It shall have no more than six consecutive zeros or ones
        accessAddr |= noMoreThanSixZerosInRowMsk;//To satisfy no more than six consecutive zeros
        accessAddr &= noMoreThanSixOnessInRowMsk;//To satisfy no more than six consecutive ones

        //=====Up to here, the accessAddr must be 0bxxxxx0 1x  xxxx01xx xxx01xxx xx01xxxx======

        //Rule2: It shall not have all four octets equal
            //Naturally fit

        //On an implementation that also supports the LE Coded PHY (see Section 2.2),
        //the Access Address shall also meet the following requirements:

        //Rule3: It shall have at least three ones in the least significant 8 bits.
        //Rule4: It shall have no more than eleven transitions in the least significant 16 bits
        accessAddr |= 0xF;//to satisfy Rule3 and Rule4


        accessAddr &= msbSixBitsAllZerosMsk;//To clear the msb six bits for later use
        //=====Up to here, the accessAddr must be 0b000000 1x  xxxx01xx xxx01xxx xx011111

        //and there are at most 18 transitions in the first 26 bits. Therefore we only need to make sure that
        //there are no more than 6 transitions and a minimum two transitions in the msb six bits.
        //The first criterion naturally fit since only 6 bits can never result in 6 transitions.

        //Rule5: It shall have no more than 24 transitions
            //Naturally fit
        //Rule6: It shall have a minimum of two transitions in the most significant six bits
        accessAddr |= (msbSixBitsTwoTransitionMsk[randidx] << 24);

        //=====Up to here, the accessAddr must be 0b100001 1x  xxxx01xx xxx01xxx xx011111

        //Rule7: It shall not be the advertising channel packet Access Address(0x8E89BED6)
            //Naturally fit
        //Rule8: It shall not be a sequence that differs from the advertising physical channel
        //packet Access Address by only one bit
            //Naturally fit

        if(++breakCnt > 10)
        {
            break;
        }

        //It shall not be the Access Address for any existing ACL connection or CIS on
        //this device
            //TBD
        //It shall not be the Access Address for any enabled periodic advertising train
        //if(s_bleLmCmnCheckPeriAdvAAExistCb != NULL)
        {
            //pass_check = !s_bleLmCmnCheckPeriAdvAAExistCb(accessAddr);
            //if(pass_check == FALSE)
            {
                //continue;
            }
        }
    }while(pass_check == 0);

    if(breakCnt > 10)
    {
        accessAddr = 0xC2174DDF;//just set a random number which meets Rule1~8
    }

    DUMPNX(accessAddr);
    LibUtil_PrintBinary((u8 *)&accessAddr, 4);
}


class TO  {
public:
    TO(){PRINT_FUNC;};
    ~TO(){PRINT_FUNC;};
};

typedef struct TT_t {
    TO *to;
} TT_t;


void TextVar_Demo(void)
{
    PRINT_FUNC;
    PRINT_FUNC;

    std::string str = "123";
    TO obj;
    TT_t stt;
    stt.to = &obj;
    {
        std::map<std::string, TT_t> varMap;
        
        PRINT_LINE;
        varMap.insert ( std::pair<std::string,TT_t>(str, stt) );
        PRINT_LINE;
    }
    PRINT_LINE;
    //BLE_LM_CmnGenAccessAddr();
    //BLE_LM_CmnGenAccessAddr();
    //BLE_LM_CmnGenAccessAddr();
    //BLE_LM_CmnGenAccessAddr();
    LibUtil_TestRand();
}

