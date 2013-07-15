#include "NFSPClient.h"

NFSPClient::NFSPClient():fTimeoutTask(this, 60)
{
	memset(fPeerList, 0, sizeof(fPeerList));
	this->Signal(Task::kStartEvent);
}

NFSPClient::~NFSPClient()
{
}

SInt64 NFSPClient::Run()
{
	EventFlags theEvents = this->GetEvents();
    
    if (theEvents & Task::kStartEvent)
    {
       // todo:
       NFSPActivePeer* peerp = new NFSPActivePeer();
       fPeerList[0] = peerp;
    }
         
    if (theEvents & Task::kTimeoutEvent)
    {
		// todo:
    }
        
    if (theEvents & Task::kKillEvent)
    {
		// todo:
        return -1;
    }   
        
    fTimeoutTask.RefreshTimeout();

    return 0;    
}

