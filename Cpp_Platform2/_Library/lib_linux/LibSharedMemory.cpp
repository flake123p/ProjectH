
#include "Everything_Lib_Linux.hpp"

// ====== Platform Library ======
#include "LibSharedMemory.hpp"

/*
 *   Reference: http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html
 *
 *   Command Line:
 *     ipcs: Show IPC information
 *     ipcrm -M <key>: Remove shared memory
 */

/*
 * We'll name our shared memory segment
 * "0x20170227".
 */
key_t gShmKey = 0x20170227;
int   gShmId  = 0;
char *gShmPtr = NULL;

bool LibSharedMemory_Init(u32 buf_size, SHM_SC_TYPE_t sc_type)
{
	int shmflg;

	switch (sc_type) {
		case SHM_SERVER:
			shmflg = IPC_CREAT | 0666;
			break;

		case SHM_CLIENT:
			shmflg = 0666;
			break;

		default:
			BASIC_ASSERT(0);
			break;
	}

	/*
	 * Create the segment.
	 */
	if ((gShmId = shmget(gShmKey, (size_t)0x100000, shmflg)) < 0) {
		perror("shmget");
		return false;
	}

	/*
	 * Now we attach the segment to our data space.
	 */
	if ((gShmPtr = (char *)shmat(gShmId, NULL, 0)) == (char *) -1) {
		perror("shmat");
		return false;
	}

	return true;
}

void LibSharedMemory_Uninit(SHM_SC_TYPE_t sc_type)
{
	char cmd[60];

	switch (sc_type) {
		case SHM_SERVER:
			sprintf(cmd, "ipcrm --shmem-key 0x%08x\n", gShmKey);
			system(cmd);
			break;

		case SHM_CLIENT:
			break;

		default:
			BASIC_ASSERT(0);
			break;
	}
}

char *LibSharedMemory_GetBuffer()
{
	return gShmPtr;
}
