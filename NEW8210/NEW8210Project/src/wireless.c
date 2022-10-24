#include <posapi.h>
#include "seos.h"
#include "common.h"

int gprs_connect()
{
	int32_t     iRet;
	
	lcdCls();
	//check if link is alive
	iRet = PPPCheck(PPP_DEV_GPRS);
	if(iRet==0)
		return 0; //connected
		
	lcdDisplay(0,2,DISP_CFONT, "INITIALIZING NETWORK...");
	lcdFlip();
	
	iRet = WnetInit(20000);
	if(0 != iRet)
	{
		lcdClrLine(2,4);
		lcdDisplay(0,2,DISP_CFONT, "INITIALIZATION FAILED: %d", iRet);
		lcdFlip();
		kbGetKey();
		return -1;
	}
	//check SIM card
	iRet = WnetCheckSim();
	if(0 != iRet)
	{
		lcdClrLine(2,4);
		if(iRet==NET_ERR_NOSIM)
			lcdDisplay(0,2,DISP_CFONT, "NO SIM CARD");
		else if(iRet==NET_ERR_PIN)
			lcdDisplay(0,2,DISP_CFONT, "SIM PIN ERROR");
		else
			lcdDisplay(0,2,DISP_CFONT, "SIM CARD ERROR");
			
		lcdFlip();
		kbGetKey();
		return -1;
	}
	lcdClrLine(2,4);
	lcdDisplay(0,2,DISP_CFONT, "NETWORK LOGGING IN...");
	lcdFlip();
	
	iRet = PPPLogin(PPP_DEV_GPRS, (char*)ucAPN, (char*)ucUID, (char*)ucUPWD, 0, 65000);
	if(iRet != NET_OK)
	{
		lcdClrLine(2,4);
		lcdDisplay(0,2,DISP_CFONT, "NETWORK LOGIN FAILED: %d", iRet);
		lcdFlip();
		
		iRet = kbGetKey();
		return -1;
	}
	while(1)
	{
		lcdClrLine(2,4);
		lcdDisplay(0,2,DISP_CFONT, "CHECKING CONNECTION...");
		lcdFlip();
		iRet = PPPCheck(PPP_DEV_GPRS);
		if(iRet != -NET_ERR_LINKOPENING)
		{
			break;
		}
		sysDelayMs(100);
	}
	return 0;
}
