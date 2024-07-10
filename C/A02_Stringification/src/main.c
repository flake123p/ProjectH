/*
	Purpose: 
		Usage of ## and #.
*/
#include <stdio.h>

#define MSG_QUEUEFULL	0x01
#define MSG_MULTIKEY	0x02
#define MSG_INVALIDKEY	0x03

#define DBGMSG(x) \
		printf("get msg id(%d), name(%s)\n", MSG_##x, #x);

int main()
{
    char input = '!';

    while(input != '7'){
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
				break;
			case MSG_MULTIKEY:
				DBGMSG(MULTIKEY)
				break;
			case MSG_INVALIDKEY:
				DBGMSG(INVALIDKEY)
				break;
			default:
				printf("invalid nember\n");
				continue;
		}
    }
	
    return 0;
}