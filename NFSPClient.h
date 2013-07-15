// file: NFSPClient.h
// NFSP: New Funshion Streaming Protocol, which is samiliar to BT protocol.

// BT protocol 
// 1. download the torrent file from HttpServer;
// 2. get the peer list from Tracker;
// 3. connect with peer, download piece (combined of slices), piece=256KB, slice=16KB;
// till end. All the pieces downloaded.

// NFSP protocol 
// 0. hash.dat, 
// 1. connect with tracker,  get the peer list.
// 2. connect with peer, download the dat header, retrieve the torrent file.
// 3. connect with peer, download the real files.
// till end. All the pieces downloaded.


#ifndef __NFSPCLIENT_H__
#define __NFSPCLIENT_H__

#include "NFSPActivePeer.h"

#define PEER_MAX_NUM	10

class NFSPClient : public Task
{
public:
	NFSPClient();
	virtual ~NFSPClient();
	virtual SInt64 	Run();
	
protected:
	// peer list
	NFSPActivePeer* fPeerList[PEER_MAX_NUM];
	TimeoutTask		fTimeoutTask;

};

#endif

