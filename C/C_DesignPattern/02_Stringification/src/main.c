/*
	OS Define
	
	Web Resource:
	http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*/
#include <stdio.h>

#define MSG_QUEUEFULL	0x01
#define MSG_MULTIKEY	0x02
#define MSG_INVALIDKEY	0x03

#define DBGMSG(x) \
		printf("get msg id(%d), name(%s)\n", MSG_##x, #x);


//#define MSG_QUEUEFULL	0x00
//#define MSG_MULTIKEY	0x01
//#define MSG_INVALIDKEY	0x02
/*
MsgSlot_t MsgArr[] = {
	DBGMSG(MSG_QUEUEFULL)
	DBGMSG(MSG_MULTIKEY)
	DBGMSG(MSG_INVALIDKEY)
	{NULL, NULL},
};
*/
int main()
{
    char input = '!';

    while(input != '7'){
        //system("cls");
        printf("please input a number(1~3), 7 is quit :");
        scanf(" %d", &input);

        switch (input)
		{
			case 7:
				printf("quit the function\n");
				return 0;
				break;
			case MSG_QUEUEFULL:
				DBGMSG(QUEUEFULL)
				//printf("get msg(%d): %s\n", MsgArr.msg, MsgArr.msgName);
				break;
			case MSG_MULTIKEY:
				DBGMSG(MULTIKEY)
				//printf("get msg(%d): %s\n", MsgArr.msg, MsgArr.msgName);
				break;
			case MSG_INVALIDKEY:
				DBGMSG(INVALIDKEY)
				//printf("get msg(%d): %s\n", MsgArr.msg, MsgArr.msgName);
				break;
			default:
				printf("invalid nember\n");
				continue;
		}
        //system("pause");
    }
	
    return 0;
}