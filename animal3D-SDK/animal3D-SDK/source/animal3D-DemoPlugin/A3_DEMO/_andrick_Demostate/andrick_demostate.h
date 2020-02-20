#ifndef ANDRICK_DEMOSTATE_H_
#define ANDRICK_DEMOSTATE_H_

#include <memory>
#include <sstream>
#include <iostream>
#include <string>
#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <animal3D/animal3D.h>
#include <animal3D-A3DG/animal3D-A3DG.h>
#include <animal3D-A3DM/animal3D-A3DM.h>
#include <RakNet/RakPeerInterface.h>
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"

struct a3_DemoState
{
	//---------------------------------------------------------------------
	// general variables pertinent to the state

	// terminate key pressed
	a3i32 exitFlag;

	// global vertical axis: Z = 0, Y = 1
	a3i32 verticalAxis;

	// asset streaming between loads enabled (careful!)
	a3i32 streaming;

	// window and full-frame dimensions
	a3ui32 windowWidth, windowHeight;
	a3real windowWidthInv, windowHeightInv, windowAspect;
	a3ui32 frameWidth, frameHeight;
	a3real frameWidthInv, frameHeightInv, frameAspect;
	a3i32 frameBorder;

	//---------------------------------------------------------------------
	// objects that have known or fixed instance count in the whole demo

	// text renderer
	a3i32 textInit, textMode, textModeCount;
	a3_TextRenderer text[1];

	// input
	a3_MouseInput mouse[1];
	a3_KeyboardInput keyboard[1];
	a3_XboxControllerInput xcontrol[4];

	// pointer to fast trig table
	a3f32 trigTable[4096 * 4];
	a3_Timer renderTimer[1];

	//---------------------------------------------------------------------
	//custom andrick stuff

	RakNet::RakPeerInterface* peer;
	std::shared_ptr<class SceneManager> mpSceneManager;
	std::shared_ptr<class Client> mpCurrentUser;

	//New input in order this frame.
	std::string newInput;

	//---------------------------------------------------------------------
	//Dan Networking Edit
	typedef a3byte a3netAddressStr[16];

	a3ui16 port_inbound, port_outbound;
	a3ui16 maxConnect_inbound, maxConnect_outbound;

	// startup networking
	a3i32 a3netStartup(a3ui16 port_inbound, a3ui16 port_outbound, a3ui16 maxConnect_inbound, a3ui16 maxConnect_outbound);

	// shutdown networking
	a3i32 a3netShutdown();


	// connect
	a3i32 a3netConnect(a3netAddressStr const ip);

	// disconnect
	a3i32 a3netDisconnect();


	// process inbound packets
	a3i32 a3netProcessInbound();

	// process outbound packets
	a3i32 a3netProcessOutbound();
};

#endif // !ANDRICK_DEMOSTATE_H_