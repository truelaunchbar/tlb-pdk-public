#include "globals.h"
#include "TimerPropDlg.h"
#include "xuistrings.h"

CTimerPropDlg::CTimerPropDlg(CTimer* timer, CXUIEngine* engine) : CXUIDialog(L"res:timer.xml", engine)
{
	m_player.SetOnStop(this, &CTimerPropDlg::OnSoundStop);
	m_timer				= timer;
	m_saveAndRun		= FALSE;
	m_bPlayerStopped	= TRUE;
}

CTimerPropDlg::~CTimerPropDlg(void)
{
}

void CTimerPropDlg::OnInitDialog()
{
	CXUIDialog::OnInitDialog();

	m_hours		= m_timer->m_time / 1000 / 3600;
	m_minutes	= (m_timer->m_time - (m_hours * 3600 * 1000)) / 1000 / 60;
	m_seconds	= (m_timer->m_time - (m_hours * 3600 * 1000) - (m_minutes * 60 * 1000)) / 1000;

	dataExchange(FALSE, NULL);
}

BOOL CTimerPropDlg::OnEndDialog( UINT code )
{
	if(code == IDOK)
	{
		dataExchange(TRUE, NULL);
		m_timer->m_time = (m_hours * 3600 + m_minutes * 60 + m_seconds) * 1000;
	}
	return TRUE;
}

BOOL CTimerPropDlg::OnSaveAndRun()
{
	if(OnEndDialog(IDOK))
	{
		m_saveAndRun = TRUE;
		EndDialog(CXUIDialog::get_wnd(), IDOK);
	}
	return TRUE;
}

BOOL CTimerPropDlg::OnPlay()
{
	if(!m_bPlayerStopped)
	{
		m_player.stop();
		OnSoundStop();
	} else
	{
		LPCWSTR snd = find(L"soundFile")->value_STR();
		WCHAR defSound[MAX_PATH];

		if(!snd)
		{
			GetModuleFileName(g_hInst, defSound, MAX_PATH);
			PathRemoveFileSpec(defSound);
			PathAddBackslash(defSound);
			lstrcat(defSound, TEXT("sound.mp3"));
			snd = defSound;
		}

		if(m_player.play(snd))
		{
			m_bPlayerStopped = FALSE;
			find(L"play")->value_STR(m_engine->getStringDef(XUI_STR_STOP));
		}
	}
	return TRUE;
}

void CTimerPropDlg::OnSoundStop()
{
	m_bPlayerStopped = TRUE;
	find(L"play")->value_STR(m_engine->getStringDef(XUI_STR_PLAY));
}
