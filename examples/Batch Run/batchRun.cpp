#include "globals.h"
#include "batchRunBtn.h"

// {A0CD9E4B-EB20-40ab-B10A-D81F90C1D11F}
EXTERN_C GUID CLSID_PLUGIN = 
{ 0xa0cd9e4b, 0xeb20, 0x40ab, { 0xb1, 0xa, 0xd8, 0x1f, 0x90, 0xc1, 0xd1, 0x1f } };


LPCLSID GetCLSID()
{
	return &CLSID_PLUGIN;
}

LPSTR GetPluginName()
{
	return "Batch Run";
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
	return new CBatchRunBtn;
}
