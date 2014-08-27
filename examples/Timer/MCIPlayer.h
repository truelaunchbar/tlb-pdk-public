#pragma once

#include "TxWnd.h"

class CMCIPlayer : public CTxWnd
{
	DWORD	m_id;
public:
	CMCIPlayer(DWORD id = 0);
	virtual ~CMCIPlayer(void);

	BOOL	play(LPCWSTR fileName);
	void	stop();
	DWORD	getID()	{	return m_id;	}

	virtual void onStop();

protected:
	virtual LRESULT OnMessage(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
};

template <class T> class TMCIPlayer : public CMCIPlayer
{
public:
	typedef private void (T::*OnStopFunction)(void);

	TMCIPlayer()
	{
		m_pOnStopFunction = NULL;
		m_pClass = NULL;
	}

	void SetOnStop(T *pClass, OnStopFunction pFunc)
	{
		m_pClass         = pClass;
		m_pOnStopFunction = pFunc;
	}

protected:
	void onStop()  
	{
		if (m_pOnStopFunction && m_pClass)
		{
			(m_pClass->*m_pOnStopFunction)();
		}
	}

private:
	T *m_pClass;
	OnStopFunction m_pOnStopFunction;
};