// file: NFSPActivePeer.h
// NFSP: New Funshion Streaming Protocol

#ifndef __NFSPACTIVEPEER_H__
#define __NFSPACTIVEPEER_H__

#include "BaseServer/OSHeaders.h"
#include "BaseServer/StringFormatter.h"
#include "BaseServer/Task.h"
#include "BaseServer/TCPSocket.h"
#include "BaseServer/TimeoutTask.h"

class NFSPActivePeer : public Task
{
public:
	NFSPActivePeer();
	virtual ~NFSPActivePeer();
	virtual     SInt64      Run();
	
protected:
	int			SendMsg();
	int			RecvMsg();
	
protected:
	typedef enum state_t
	{
		kInit			= 0,
		kConnecting 	= 1,
		kConnected		= 2,
		kConnectFailed  = 3,
		kDisconnect		= 4, 
	} STATE_T;
	
	typedef enum stage_t
	{
		kStageInit			= 0,
		kStageHandShake		= 1, 
		kStageBitfield		= 2, 
		kStageDownloading	= 3, 
		kStageDownloadDone	= 4,
	} STAGE_T;
	
	STATE_T			fState;
	STAGE_T			fStage;	
	TCPSocket		fSocket;
	TimeoutTask		fTimeoutTask;
	
};

#endif

