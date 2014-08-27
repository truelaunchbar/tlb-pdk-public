#include "globals.h"
#include "TimerMainBtn.h"
#include <vld.h>

// {CC2EEFBE-792B-4917-9668-EA4C2FE1B132}
EXTERN_C GUID CLSID_TIMER = 
{ 0xcc2eefbe, 0x792b, 0x4917, { 0x96, 0x68, 0xea, 0x4c, 0x2f, 0xe1, 0xb1, 0x32 } };


LPCLSID GetCLSID()
{
	return &CLSID_TIMER;
}

LPSTR GetPluginName()
{
	return "Timer";
}

void InitDLL()
{
	CTxDIB::Initialise();
}

void UninitDLL()
{
	CTxDIB::DeInitialise();
}

CTlbButton*	CreatePlugin()
{
	return new CTimerMainBtn;
}

