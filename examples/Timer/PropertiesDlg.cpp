#include "globals.h"
#include "PropertiesDlg.h"
#include "xuistrings.h"
#include "timerclass.h"

CPropertiesDlg::CPropertiesDlg(CXUIEngine* engine) :	CXUIDialog(L"res:properties.xml", engine)
{
	m_player.SetOnStop(this, &CPropertiesDlg::OnSoundStop);

	m_strSndFile		= NULL;
	m_playSound			= TRUE;
	m_bPlayerStopped	= TRUE;
}

CPropertiesDlg::~CPropertiesDlg(void)
{
	FREE_CLEAR_STR(m_strSndFile);
}

void CPropertiesDlg::OnInitDialog()
{
	CXUIDialog::OnInitDialog();

	MAKE_STR(m_strSndFile, m_soundFile.c_str());

	dataExchange(FALSE, NULL);


	TCHAR str[MAX_PATH];
	GetModuleFileName(g_hInst, str, MAX_PATH);
	DWORD versionLS, versionMS;
	GetFileVersion(str, versionMS, versionLS);

	TCHAR strVer[255];
	Version2Str(versionMS, versionLS, strVer);
	TCHAR strText[255];
	wsprintf(strText, TEXT("v%s"), strVer);

	find(L"version")->value_STR(strText);

	loadSkins();
	initSkins();
}

BOOL CPropertiesDlg::OnEndDialog( UINT code )
{
	if(code == IDOK)
	{
		dataExchange(TRUE, NULL);

		if(m_strSndFile)
		{
			m_soundFile = m_strSndFile;
		} else
		{
			m_soundFile = L"";
		}

		if(m_ctlSkinMenu->value_INT() >= 1 && m_ctlSkinMenu->value_INT() <= m_skinsMenu.size())
		{
			m_skinFileMenu = m_skinsMenu[m_ctlSkinMenu->value_INT() - 1];
		} else
		{
			m_skinFileMenu = L"";
		}
		int idx = m_ctlSkinBtn->value_INT();
		if(idx == 1)
		{
			m_showTimers = FALSE;
		} else
		{
			m_showTimers = TRUE;
			idx -= 2;
			if(idx >= 0 && idx < m_skinsBtn.size())
			{
				m_skinFileBtn = m_skinsBtn[idx];
			} else
			{
				m_skinFileBtn = L"";
			}
		}
		if(m_ctlSkinCtrls->value_INT() >= 1 && m_ctlSkinCtrls->value_INT() <= m_skinsCtrls.size())
		{
			m_skinFileCtrls = m_skinsCtrls[m_ctlSkinCtrls->value_INT() - 1];
		} else
		{
			m_skinFileCtrls = L"";
		}
		if(m_ctlSkinNotify->value_INT() >= 1 && m_ctlSkinNotify->value_INT() <= m_skinsNotify.size())
		{
			m_skinFileNotify = m_skinsNotify[m_ctlSkinNotify->value_INT() - 1];
		} else
		{
			m_skinFileNotify = L"";
		}
	}
	return TRUE;
}

BOOL CPropertiesDlg::OnPlay()
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

void CPropertiesDlg::OnSoundStop()
{
	m_bPlayerStopped = TRUE;
	find(L"play")->value_STR(m_engine->getStringDef(XUI_STR_PLAY));
}

void CPropertiesDlg::onSkinFileFound( LPCWSTR skinFile, LPWSTR shortPath )
{
	WCHAR name[255];
	GetPrivateProfileString(L"info", L"name", L"", name, 255, skinFile);
	if(name[0])
	{
		WCHAR str[255];
		GetPrivateProfileString(L"skins", INI_MENU_SKIN, L"", str, 255, skinFile);
		if(str[0])
		{
			m_skinsMenu.push_back(std::wstring(shortPath));
			int val = (int) m_skinsMenu.size();
			m_ctlSkinMenu->addItem(val, name);
			if(!StrCmpI(shortPath, m_skinFileMenu.c_str()))
			{
				m_ctlSkinMenu->value_INT(val);
			}
		}
		GetPrivateProfileString(L"skins", INI_NOTIFY_SKIN, L"", str, 255, skinFile);
		if(str[0])
		{
			m_skinsNotify.push_back(std::wstring(shortPath));
			int val = (int) m_skinsNotify.size();
			m_ctlSkinNotify->addItem(val, name);
			if(!StrCmpI(shortPath, m_skinFileNotify.c_str()))
			{
				m_ctlSkinNotify->value_INT(val);
			}
		}
		GetPrivateProfileString(L"skins", INI_MENUBTN_SKIN, L"", str, 255, skinFile);
		if(str[0])
		{
			m_skinsBtn.push_back(std::wstring(shortPath));
			int val = (int) m_skinsBtn.size() + 1;
			m_ctlSkinBtn->addItem(val, name);
			if(m_showTimers && !StrCmpI(shortPath, m_skinFileBtn.c_str()))
			{
				m_ctlSkinBtn->value_INT(val);
			}
		}
		GetPrivateProfileString(L"skins", INI_CONTROLS_SKIN, L"", str, 255, skinFile);
		if(str[0])
		{
			m_skinsCtrls.push_back(std::wstring(shortPath));
			int val = (int) m_skinsCtrls.size();
			m_ctlSkinCtrls->addItem(val, name);
			if(!StrCmpI(shortPath, m_skinFileCtrls.c_str()))
			{
				m_ctlSkinCtrls->value_INT(val);
			}
		}
	}
}

void CPropertiesDlg::loadSkins()
{
	TCHAR path[MAX_PATH];
	GetModuleFileName(g_hInst, path, MAX_PATH);
	PathRemoveFileSpec(path);
	PathAddBackslash(path);
	lstrcat(path, TEXT("skins\\"));

	m_ctlSkinNotify->clearItems();
	m_ctlSkinMenu->clearItems();
	m_ctlSkinBtn->clearItems();
	m_ctlSkinCtrls->clearItems();

	m_skinsMenu.clear();
	m_skinsBtn.clear();
	m_skinsNotify.clear();
	m_skinsCtrls.clear();

	m_ctlSkinNotify->addItem(0, m_engine->getStringDef(XUI_STR_DEFSKIN));
	m_ctlSkinMenu->addItem(0, m_engine->getStringDef(XUI_STR_DEFSKIN));
	m_ctlSkinBtn->addItem(1, m_engine->getStringDef(XUI_STR_HIDETIMERS));
	m_ctlSkinBtn->addItem(0, m_engine->getStringDef(XUI_STR_DEFSKIN));
	m_ctlSkinCtrls->addItem(0, m_engine->getStringDef(XUI_STR_DEFSKIN));

	m_ctlSkinNotify->value_INT(0);
	m_ctlSkinMenu->value_INT(0);
	if(m_showTimers)
	{
		m_ctlSkinBtn->value_INT(0);
	} else
	{
		m_ctlSkinBtn->value_INT(1);
	}
	m_ctlSkinCtrls->value_INT(0);

	m_skinMenu.enumSkins(this, path, L"timer.ini");
}

void CPropertiesDlg::initSkins()
{
	m_skinMenu.loadSkin(m_skinFileMenu);
	m_skinBtn.loadSkin(m_skinFileBtn);
	m_skinCtrls.loadSkin(m_skinFileCtrls);
	m_skinNotify.loadSkin(m_skinFileNotify);

	CTimer* timer = CTimer::createTest(m_engine);
	timer->setSkinParams(&m_skinMenu,	0);
	for(int i = 1; i <= 4; i++)
	{
		m_skinBtn.addSlice(i);
		timer->setSkinParams(&m_skinBtn, i);
	}

	m_skinBtn.setSliceMode((TxSkin::sliceMode) m_rowColsMode, m_RowsCols);

	timer->setSkinParams(&m_skinNotify,	0);
	delete timer;
}

BOOL CPropertiesDlg::OnDrawMenuSkin( HDC hdc, LPRECT rcDraw )
{
	m_skinMenu.draw(hdc, rcDraw->left, rcDraw->top);
	return TRUE;
}

BOOL CPropertiesDlg::OnDrawBtnSkin( HDC hdc, LPRECT rcDraw )
{
	if(m_showTimers)
	{
		RECT rcTimers;
		RECT rcCtl;
		getTimersRect(&rcTimers);
		m_skinBtn.draw(hdc, rcDraw->left + rcTimers.left, rcDraw->top + rcTimers.top);
		if(getCtlRect(&rcCtl))
		{
			m_skinCtrls.draw(hdc, rcDraw->left + rcCtl.left, rcDraw->top + rcCtl.top);
		}
	} else
	{
		m_engine->DrawImage(hdc, rcDraw->left, rcDraw->top, 48, 48, L"logo.png");
	}
	return TRUE;
}

BOOL CPropertiesDlg::OnDrawNotifySkin( HDC hdc, LPRECT rcDraw )
{
	m_skinNotify.draw(hdc, rcDraw->left, rcDraw->top);
	return TRUE;
}

BOOL CPropertiesDlg::OnChangedMenuSkin()
{
	int idx = m_ctlSkinMenu->value_INT() - 1;
	if(idx >= 0 && idx < (int) m_skinsMenu.size())
	{
		m_skinMenu.loadSkin(m_skinsMenu[idx]);
	} else
	{
		m_skinMenu.loadSkin(std::wstring(L""));
	}
	m_ctlDrawMenu->redraw(TRUE);
	return TRUE;
}

BOOL CPropertiesDlg::OnChangedBtnSkin()
{
	int idx = m_ctlSkinBtn->value_INT();
	if(idx == 1)
	{
		m_showTimers = FALSE;
	} else
	{
		m_showTimers = TRUE;
	}
	idx -= 2;
	if(idx >= 0 && idx < (int) m_skinsBtn.size())
	{
		m_skinBtn.loadSkin(m_skinsBtn[idx]);
	} else
	{
		m_skinBtn.loadSkin(std::wstring(L""));
	}
	m_skinBtn.setSliceMode((TxSkin::sliceMode) find(L"rowColsMode")->value_INT(), find(L"RowsCols")->value_INT());
	m_ctlDrawBtn->redraw(TRUE);
	return TRUE;
}

BOOL CPropertiesDlg::OnChangedCtrlSkin()
{
	int idx = m_ctlSkinCtrls->value_INT() - 1;
	if(idx >= 0 && idx < (int) m_skinsCtrls.size())
	{
		m_skinCtrls.loadSkin(m_skinsCtrls[idx]);
	} else
	{
		m_skinCtrls.loadSkin(std::wstring(L""));
	}
	m_ctlDrawBtn->redraw(TRUE);
	return TRUE;
}

BOOL CPropertiesDlg::OnChangedNotifySkin()
{
	int idx = m_ctlSkinNotify->value_INT() - 1;
	if(idx >= 0 && idx < (int) m_skinsNotify.size())
	{
		m_skinNotify.loadSkin(m_skinsNotify[idx]);
	} else
	{
		m_skinNotify.loadSkin(std::wstring(L""));
	}
	m_ctlDrawNotify->redraw(TRUE);
	return TRUE;
}

BOOL CPropertiesDlg::OnRedrawBtnSkin()
{
	m_ctlDrawBtn->redraw(TRUE);
	return TRUE;
}

void CPropertiesDlg::getTimersRect( LPRECT rcSkin )
{
	DWORD ctlPos = find(L"ctlSide")->value_INT() | find(L"ctlPos")->value_INT();
	switch(ctlPos & CTL_SIDE_MASK)
	{
	case CTL_SIDE_LEFT:
		rcSkin->left	= m_skinCtrls.width();
		rcSkin->top		= 0;
		break;
	case CTL_SIDE_TOP:
		rcSkin->left	= 0;
		rcSkin->top		= m_skinCtrls.height();
		break;
	default:
		rcSkin->left	= 0;
		rcSkin->top		= 0;
		break;
	}
	rcSkin->right	= rcSkin->left	+ m_skinBtn.width();
	rcSkin->bottom	= rcSkin->top	+ m_skinBtn.height();
}

BOOL CPropertiesDlg::getCtlRect( LPRECT rcSkin )
{
	DWORD ctlPos = find(L"ctlSide")->value_INT() | find(L"ctlPos")->value_INT();

	int w = m_skinBtn.width();
	int h = m_skinBtn.height();
	switch(ctlPos & CTL_SIDE_MASK)
	{
	case CTL_SIDE_LEFT:
	case CTL_SIDE_RIGHT:
		w += m_skinCtrls.width();
		h = max(m_skinCtrls.height(), h);
		break;
	case CTL_SIDE_TOP:
	case CTL_SIDE_BOTTOM:
		h += m_skinCtrls.height();
		w = max(m_skinCtrls.width(), w);
		break;
	}


	switch(ctlPos & CTL_SIDE_MASK)
	{
	case CTL_SIDE_HIDE:
		return FALSE;
	case CTL_SIDE_RIGHT:
		rcSkin->left	= m_skinBtn.width();
		switch(ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->top = h - m_skinCtrls.height();
			break;
		case CTL_POS_CENTER:
			rcSkin->top = h / 2 - m_skinCtrls.height() / 2;
			break;
		default:
			rcSkin->top = 0;
		}
		break;
	case CTL_SIDE_LEFT:
		rcSkin->left	= 0;
		switch(ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->top = h - m_skinCtrls.height();
			break;
		case CTL_POS_CENTER:
			rcSkin->top = h / 2 - m_skinCtrls.height() / 2;
			break;
		default:
			rcSkin->top = 0;
		}
		break;
	case CTL_SIDE_TOP:
		rcSkin->top		= 0;
		switch(ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->left = w - m_skinCtrls.width();
			break;
		case CTL_POS_CENTER:
			rcSkin->left = w / 2 - m_skinCtrls.width() / 2;
			break;
		default:
			rcSkin->left = 0;
		}
		break;
	case CTL_SIDE_BOTTOM:
		rcSkin->top		= m_skinBtn.height();
		switch(ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->left = w - m_skinCtrls.width();
			break;
		case CTL_POS_CENTER:
			rcSkin->left = w / 2 - m_skinCtrls.width() / 2;
			break;
		default:
			rcSkin->left = 0;
		}
		break;
	default:
		rcSkin->left	= 0;
		rcSkin->top		= 0;
	}
	rcSkin->right	= rcSkin->left	+ m_skinCtrls.width();
	rcSkin->bottom	= rcSkin->top	+ m_skinCtrls.height();

	return TRUE;
}
