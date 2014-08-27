#pragma once
#include "timerclass.h"
#include "MCIPlayer.h"

class CTimerPropDlg : public CXUIDialog
{
	int			m_hours;
	int			m_minutes;
	int			m_seconds;
	CTimer*		m_timer;

	TMCIPlayer<CTimerPropDlg>	m_player;
	BOOL						m_bPlayerStopped;
public:
	BOOL		m_saveAndRun;

	CTimerPropDlg(CTimer* timer, CXUIEngine* engine);
	virtual ~CTimerPropDlg(void);

	virtual void OnInitDialog();
	virtual BOOL OnEndDialog(UINT code);

	XUI_BEGIN_DATA_MAP
		XUI_DATA_MAP_PSTR	(L"message",	m_timer->m_message)
		XUI_DATA_MAP_PSTR	(L"command",	m_timer->m_command)
		XUI_DATA_MAP_PSTR	(L"name",		m_timer->m_name)
		XUI_DATA_MAP_PSTR	(L"soundFile",	m_timer->m_sound)
		XUI_DATA_MAP_INT	(L"playSound",	m_timer->m_playSound)
		XUI_DATA_MAP_INT	(L"hours",		m_hours)
		XUI_DATA_MAP_INT	(L"minutes",	m_minutes)
		XUI_DATA_MAP_INT	(L"seconds",	m_seconds)
	XUI_END_DATA_MAP

	XUI_BEGIN_EVENT_MAP
		XUI_HANDLE_CLICKED	(L"saverun",	OnSaveAndRun)
		XUI_HANDLE_CLICKED	(L"play",	OnPlay)
	XUI_END_EVENT_MAP

private:
	BOOL	OnSaveAndRun();
	void	OnSoundStop();
	BOOL	OnPlay();
};
