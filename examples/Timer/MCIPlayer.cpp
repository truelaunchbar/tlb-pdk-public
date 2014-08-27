#include "globals.h"
#include "MCIPlayer.h"

CMCIPlayer::CMCIPlayer(DWORD id /*= 0*/)
{
	m_id	= id;
	create(0, NULL, 0, 0, 0, 0, 0, NULL, NULL, g_hInst);
}

CMCIPlayer::~CMCIPlayer(void)
{
}

BOOL CMCIPlayer::play( LPCWSTR fileName )
{
	if(!fileName)
	{
		return FALSE;
	}

	stop();

	WCHAR str[255];

	wsprintf(str, L"open \"%s\" type mpegvideo alias media%d", fileName, m_id);
	if(!mciSendString(str, NULL, 0, NULL))
	{
		wsprintf(str, L"play media%d notify", m_id);
		if(!mciSendString(str, NULL, 0, m_hWnd))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CMCIPlayer::stop()
{
	WCHAR str[255];
	wsprintf(str, L"close media%d", m_id);
	mciSendString(str, NULL, 0, NULL);
}

void CMCIPlayer::onStop()
{

}

LRESULT CMCIPlayer::OnMessage( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	if(uMessage == MM_MCINOTIFY)
	{
		onStop();
	}
	return CTxWnd::OnMessage(hWnd, uMessage, wParam, lParam);
}