#include "globals.h"
#include "timerclass.h"
#include "xuistrings.h"

CTimer::CTimer(void)
{
	m_message			= NULL;
	m_command		= NULL;
	m_name			= NULL;
	m_sound			= NULL;
	m_time			= 0;
	m_startTime		= 0;
	m_curTime		= 0;
	m_id			= 0;
	m_playSound		= FALSE;
}

CTimer::CTimer( CTimer& val )
{
	m_message		= NULL;
	m_command		= NULL;
	m_name			= NULL;
	m_sound			= NULL;

	*this = val;
}

CTimer::~CTimer(void)
{
	FREE_CLEAR_STR(m_message);
	FREE_CLEAR_STR(m_command);
	FREE_CLEAR_STR(m_name);
	FREE_CLEAR_STR(m_sound);
}

void CTimer::save( CPluginStream* stream )
{
	stream->SaveString	(L"text",		m_message);
	stream->SaveString	(L"command",	m_command);
	stream->SaveString	(L"name",		m_name);
	stream->SaveString	(L"sound",		m_sound);
	stream->SaveDWORD	(L"time",		m_time);
	stream->SaveDWORD	(L"id",			m_id);
	stream->SaveDWORD	(L"playSound",	m_playSound);
}

void CTimer::load( CPluginStream* stream )
{
	MAKE_STR(m_message,	stream->GetString(L"text"));
	MAKE_STR(m_command, stream->GetString(L"command"));
	MAKE_STR(m_name,	stream->GetString(L"name"));
	MAKE_STR(m_sound,	stream->GetString(L"sound"));
	m_time		= stream->GetDWORD(L"time");
	m_id		= stream->GetDWORD(L"id");
	m_playSound	= stream->GetDWORD(L"playSound", 1);
}

void CTimer::formatTime( LPWSTR str )
{
	DWORD time = m_time;
	if(m_startTime)
	{
		if(time < (m_curTime - m_startTime))
		{
			time = 0;
		} else
		{
			time = time - (m_curTime - m_startTime);
		}
	}
	int hours	= time / 1000 / 3600;
	int minutes	= (time - (hours * 3600 * 1000)) / 1000 / 60;
	int seconds	= (time - (hours * 3600 * 1000) - (minutes * 60 * 1000)) / 1000;
	wsprintf(str, L"%02d:%02d:%02d", hours, minutes, seconds);
}

BOOL CTimer::checkTimer()
{
	if(m_time <= (m_curTime - m_startTime))
	{
		return TRUE;
	}
	return FALSE;
}

void CTimer::operator=( CTimer& val )
{
	MAKE_STR(m_message,	val.m_message);
	MAKE_STR(m_command,	val.m_command);
	MAKE_STR(m_name,	val.m_name);
	MAKE_STR(m_sound,	val.m_sound);

	m_time			= val.m_time;
	m_startTime		= val.m_startTime;
	m_curTime		= val.m_curTime;
	m_id			= val.m_id;
	m_playSound		= val.m_playSound;
}

#define SETRET(ret, v) if(v) { ret = TRUE; }

BOOL CTimer::setSkinParams( TxSkin::CSkin* skin, DWORD silceID )
{
	BOOL ret = FALSE;

	DWORD time = m_time;
	WCHAR str[255];

	int hours	= time / 1000 / 3600;
	int minutes	= (time - (hours * 3600 * 1000)) / 1000 / 60;
	int seconds	= (time - (hours * 3600 * 1000) - (minutes * 60 * 1000)) / 1000;
	wsprintf(str, L"%02d:%02d:%02d", hours, minutes, seconds);
	SETRET(ret, skin->setParamString	(L"time", str,						silceID));
	SETRET(ret, skin->setParamInt		(L"time",		time,		NULL,	silceID));
	SETRET(ret, skin->setParamInt		(L"hours",		hours,		L"%02d", silceID));
	SETRET(ret, skin->setParamInt		(L"minutes",	minutes,	L"%02d", silceID));
	SETRET(ret, skin->setParamInt		(L"seconds",	seconds,	L"%02d", silceID));

	if(m_startTime)
	{
		if(time < (m_curTime - m_startTime))
		{
			time = 0;
		} else
		{
			time = time - (m_curTime - m_startTime);
		}
	}

	hours	= time / 1000 / 3600;
	minutes	= (time - (hours * 3600 * 1000)) / 1000 / 60;
	seconds	= (time - (hours * 3600 * 1000) - (minutes * 60 * 1000)) / 1000;
	wsprintf(str, L"%02d:%02d:%02d", hours, minutes, seconds);
	SETRET(ret, skin->setParamString	(L"time-left",		str,				silceID));
	SETRET(ret, skin->setParamInt		(L"time-left",		time,		NULL,	silceID));
	SETRET(ret, skin->setParamInt		(L"hours-left",		hours,		L"%02d", silceID));
	SETRET(ret, skin->setParamInt		(L"minutes-left",	minutes,	L"%02d", silceID));
	SETRET(ret, skin->setParamInt		(L"seconds-left",	seconds,	L"%02d", silceID));

	time = m_time - time;

	hours	= time / 1000 / 3600;
	minutes	= (time - (hours * 3600 * 1000)) / 1000 / 60;
	seconds	= (time - (hours * 3600 * 1000) - (minutes * 60 * 1000)) / 1000;
	wsprintf(str, L"%02d:%02d:%02d", hours, minutes, seconds);
	SETRET(ret, skin->setParamString	(L"time-pass",		str,				silceID));
	SETRET(ret, skin->setParamInt		(L"time-pass",		time,		NULL,	silceID));
	SETRET(ret, skin->setParamInt		(L"hours-pass",		hours,		L"%02d", silceID));
	SETRET(ret, skin->setParamInt		(L"minutes-pass",	minutes,	L"%02d", silceID));
	SETRET(ret, skin->setParamInt		(L"seconds-pass",	seconds,	L"%02d", silceID));

	if(m_startTime)
	{
		SETRET(ret, skin->setParamInt(L"started", 1, NULL, silceID));
	} else
	{
		SETRET(ret, skin->setParamInt(L"started", 0, NULL, silceID));
	}

	if(m_name)
	{
		SETRET(ret, skin->setParamString	(L"name",		m_name,				silceID));
	} else
	{
		SETRET(ret, skin->setParamString	(L"name",		L"",				silceID));
	}
	if(m_message)
	{
		SETRET(ret, skin->setParamString	(L"message",	m_message,			silceID));
	} else
	{
		SETRET(ret, skin->setParamString	(L"message",	L"",			silceID));
	}

	return ret;
}

CTimer* CTimer::createTest( CXUIEngine* xui )
{
	CTimer* timer = new CTimer;
	MAKE_STR(timer->m_message, xui->getStringDef(XUI_STR_TESTMESSAGE));
	MAKE_STR(timer->m_command, L"C:\\Windows\\Notepad.exe");
	MAKE_STR(timer->m_name, xui->getStringDef(XUI_STR_TESTNAME));
	timer->m_sound		= NULL;
	timer->m_id			= 0;
	timer->m_playSound	= TRUE;
	timer->m_time		= 7531;
	timer->m_startTime	= GetTickCount() - timer->m_time / 2;
	timer->m_curTime	= GetTickCount();

	return timer;
}

