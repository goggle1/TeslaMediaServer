
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "NFSPActivePeer.h"

#define INFO_HASH_LEN	(20)
#define PEER_ID_LEN		(20)

#define MSG_TYPE_HANDSHAKE		0x0601

#define PROTOCOL_VERSION 		0x0002

//#pragma pack(push) 
//#pragma pack(1)
typedef struct msg_handshake_t
{
	/*
 	reserved(4B)：填充随机数
 	length(4B)：0x00000036
 	type(2B)：0x0601
 	version(2B)：0x0002
 	session id（2B）：根据实际定义填充
 	info hash（20B）：交互的任务的Info Hash ID
 	peer id（20B）：发送handshake一方的Peer ID，采用fsp的peer id生成算法
 	*/
 	u_int32_t	reserved;
 	u_int32_t	length;
 	u_int16_t	type;
 	u_int16_t	version;
 	u_int16_t	session_id;	
 	char		info_hash[INFO_HASH_LEN];
 	char		peer_id[PEER_ID_LEN];	
}__attribute__((packed)) MSG_HANDSHAKE_T ;
//typedef struct msg_handshake_t MSG_HANDSHAKE_T;
//#pragma pack(pop)

NFSPActivePeer::NFSPActivePeer():
	fSocket(this, Socket::kNonBlockingSocketType),
	fTimeoutTask(this, 0)
{
	fState = kInit;
	fStage = kStageInit;
	this->Signal(Task::kStartEvent);
}

NFSPActivePeer::~NFSPActivePeer()
{

}

SInt64 NFSPActivePeer::Run()
{
	Task::EventFlags events = this->GetEvents();	
	if(events == 0x00000000)
	{
		// epoll_wait EPOLLERR or EPOLLHUP
    	// man epoll_ctl
		fprintf(stdout, "%s %s[%d][0x%016lX] events=0x%08X\n",
			__FILE__, __PRETTY_FUNCTION__, __LINE__, (long)this, events);
		return -1;
	}
		
	if(events & Task::kKillEvent)
	{
		fprintf(stdout, "%s %s[%d][0x%016lX]: get kKillEvent[0x%08X] \n", 
			__FILE__, __PRETTY_FUNCTION__, __LINE__, (long)this, events);
		return -1;
	}

	int willRequestEvent = 0;

	if(events & Task::kStartEvent)
	{
		fprintf(stdout, "%s %s[%d][0x%016lX]: get kStartEvent[0x%08X] \n", 
			__FILE__, __PRETTY_FUNCTION__, __LINE__, (long)this, events);
		
		// 124.254.47.117  9901    1C9AB5D21D8504833C9AE829FAF95C0FE5A4E249
		char* server_ip_addr = "124.254.47.117";
		UInt32 server_ip = inet_addr(server_ip_addr);
		server_ip = ntohl(server_ip);
		UInt16 server_port = 9901;
		OS_Error err = fSocket.Connect(server_ip, server_port);
		if(err == OS_NoErr)
		{
			fState = kConnected;
			fSocket.RequestEvent(EV_WR);
		}
		else if(err == EINPROGRESS || err == EAGAIN)
		{
			fState = kConnecting;
			fSocket.RequestEvent(EV_WR|EV_RE);
		}
		else
		{
			// todo:
			fState = kConnectFailed;	
			return -1;
		}
		
	}

	if(events & Task::kTimeoutEvent)
	{
		fprintf(stdout, "%s %s[%d][0x%016lX]: get kTimeoutEvent[0x%08X] \n", 
			__FILE__, __PRETTY_FUNCTION__, __LINE__, (long)this, events);
	}	
	
	
	if(events & Task::kWriteEvent)
	{
		if(fState == kConnecting)
		{
			fState = kConnected;
			willRequestEvent = EV_WR;
		}
		else if(fState == kConnected)
		{		
			fStage = kStageHandShake;
			SendMsg();
		}		
	}
	
	if(events & Task::kReadEvent)
	{
		if(fState == kConnecting)
		{
			fState = kConnected;
			willRequestEvent = EV_WR;
		}
		else if(fState == kConnected)
		{		
			fStage = kStageHandShake;
			RecvMsg();
		}	
		
	}
	
	if(willRequestEvent == 0)
	{
		// do nothing.	
	}
	else
	{
		fSocket.RequestEvent(willRequestEvent);
	}
	
	return 0;
}


int NFSPActivePeer::SendMsg()
{
	if(fStage == kStageHandShake)
	{
		MSG_HANDSHAKE_T msg;
	}
	else if(fStage == kStageBitfield)
	{
		
	}
	else if(fStage == kStageDownloading)
	{
		
	}

	return -1;
}

int NFSPActivePeer::RecvMsg()
{
	return 0;
}


