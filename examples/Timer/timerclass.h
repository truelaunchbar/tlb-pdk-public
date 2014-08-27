#pragma once

#include "pluginStream.h"

class CTimer
{
public:
	DWORD	m_id;
	LPWSTR	m_message;
	LPWSTR	m_command;
	LPWSTR	m_name;
	LPWSTR	m_sound;
	BOOL	m_playSound;
	DWORD	m_time;
	DWORD	m_startTime;
	DWORD	m_curTime;

	CTimer(void);
	CTimer(CTimer& val);
	~CTimer(void);

	void save(CPluginStream* stream);
	void load(CPluginStream* stream);
	void formatTime(LPWSTR str);
	BOOL setSkinParams(TxSkin::CSkin* skin, DWORD silceID);
	BOOL checkTimer();
	void operator=(CTimer& val);

	static CTimer* createTest(CXUIEngine* xui);
};
