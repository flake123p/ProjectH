
int gSimpleEvent = 0;

void LibEvent_SendSimpleEvent(int inputEvent)
{
	gSimpleEvent = inputEvent;
}

int LibEvent_GetSimpleEvent(bool do_clear /* = true */)
{
	int retEvent = gSimpleEvent;

	if (do_clear)
		gSimpleEvent = 0;
	
	return retEvent;
}
