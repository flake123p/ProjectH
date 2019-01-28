#ifdef DFS_SIM_ON
#include "Everything_App.hpp"
#define OSAL_Malloc malloc
#else //#ifdef DFS_SIM_ON

#endif //#ifdef DFS_SIM_ON

Request_ADDR_List_t * Scheduler_APIGetRequestAddressList()
{
    Request_ADDR_List_t * request_address_list;
    request_address_list = (Request_ADDR_List_t *)OSAL_Malloc(sizeof (Request_ADDR_List_t));
    memset(request_address_list, 0, sizeof(Request_ADDR_List_t));
    TAILQ_INIT(request_address_list);
    
    return request_address_list;
}

Request_ADDR_T * Scheduler_APIGetRequestAddress()
{
    Request_ADDR_T* request_address;
    request_address = (Request_ADDR_T *)OSAL_Malloc(sizeof(Request_ADDR_T));
    memset(request_address, 0, sizeof(Request_ADDR_T));
    return request_address;
}

Scheduler_Request_T * Scheduler_APIGetRequest()
{
    Scheduler_Request_T * request;
    request = (Scheduler_Request_T *)OSAL_Malloc(sizeof(Scheduler_Request_T));
    memset(request, 0, sizeof(Scheduler_Request_T));
    return request;
}

