#include "globals.h"
#include ".\pluginstream.h"

CPluginStream::CPluginStream(void)
{
}

CPluginStream::~CPluginStream(void)
{
	Clear();
}

// Loading data from stream
BOOL CPluginStream::Load(IStream* stream)
{
	ULONG cbRead = 0;
	DWORD count = 0;
	stream->Read(&count, sizeof(count), &cbRead);
	for(DWORD i=0; i < count; i++)
	{
		STREAM_DATA data;
		ZeroMemory(&data, sizeof(data));
		DWORD sz = 0;
		stream->Read(&sz, sizeof(sz), &cbRead);
		data.id = new TCHAR[sz / sizeof(TCHAR)];
		stream->Read(data.id, sz, &cbRead);
		stream->Read(&data.type, sizeof(data.type), &cbRead);
		stream->Read(&data.cbSize, sizeof(data.cbSize), &cbRead);
		if(data.cbSize)
		{
			data.lpData = new BYTE[data.cbSize];
			stream->Read(data.lpData, data.cbSize, &cbRead);
		}
		m_data.Add(&data);
	}

	return TRUE;
}

// Save data to stream
BOOL CPluginStream::Save(IStream* stream)
{
	ULONG cbWritten;
	DWORD sz = m_data.GetCount();
	stream->Write(&sz, sizeof(DWORD), &cbWritten);
	for(int i=0; i < m_data.GetCount(); i++) 
	{
		sz = (lstrlen(m_data[i].id) + 1) * sizeof(TCHAR);
		stream->Write(&sz, sizeof(DWORD), &cbWritten);
		stream->Write(m_data[i].id, sz, &cbWritten);
		stream->Write(&m_data[i].type, sizeof(m_data[i].type), &cbWritten);
		stream->Write(&m_data[i].cbSize, sizeof(m_data[i].cbSize), &cbWritten);
		if(m_data[i].cbSize)
		{
			stream->Write(m_data[i].lpData, m_data[i].cbSize, &cbWritten);
		}
	}

	return TRUE;
}

void CPluginStream::Clear(void)
{
	for(int i=0; i < m_data.GetCount(); i++)
	{
		if(m_data[i].id) delete m_data[i].id;
		if(m_data[i].lpData) delete m_data[i].lpData;
	}
	m_data.Clear();
}

DWORD CPluginStream::GetDWORD(LPCTSTR id, DWORD defValue)
{
	STREAM_DATA* data = findData(id);
	if(!data || data && (data->type != SDT_DATA || data->cbSize != sizeof(DWORD)))
	{
		return defValue;
	}
	DWORD ret = defValue;
	memcpy(&ret, data->lpData, sizeof(DWORD));
	return ret;
}

WORD CPluginStream::GetWORD(LPCTSTR id, WORD defValue)
{
	STREAM_DATA* data = findData(id);
	if(!data || data && (data->type != SDT_DATA || data->cbSize != sizeof(WORD)))
	{
		return defValue;
	}
	WORD ret = defValue;
	memcpy(&ret, data->lpData, sizeof(WORD));
	return ret;
}

LPCTSTR CPluginStream::GetString(LPCTSTR id, LPCTSTR defValue)
{
	STREAM_DATA* data = findData(id);
	if(!data || data && data->type != SDT_DATA)
	{
		return defValue;
	}
	return (LPCTSTR) data->lpData;
}

LPBYTE CPluginStream::GetBIN(LPCTSTR id, DWORD* cbSize)
{
	STREAM_DATA* data = findData(id);
	if(cbSize) *cbSize = 0;
	if(!data || data && data->type != SDT_DATA)
	{
		return NULL;
	}
	if(cbSize) *cbSize = data->cbSize;
	return data->lpData;
}

STREAM_DATA* CPluginStream::findData(LPCTSTR id)
{
	int nodeIndex = -1;
	if(m_nodes.GetCount()) 
	{
		nodeIndex = m_nodes[m_nodes.GetCount() - 1];
	}
	for(int i=nodeIndex + 1; i < m_data.GetCount(); i++)
	{
		if(m_data[i].type == SDT_ENDNODE && *((LPDWORD) m_data[i].lpData) == nodeIndex) 
		{
			break;
		}
		if(!StrCmpI(m_data[i].id, id)) 
		{
			return &m_data[i];
		}
	}
	return NULL;
}

void CPluginStream::SaveDWORD(LPCTSTR id, DWORD data)
{
	saveData(id, SDT_DATA, (LPBYTE) &data, sizeof(DWORD));
}

void CPluginStream::SaveWORD(LPCTSTR id, WORD data)
{
	saveData(id, SDT_DATA, (LPBYTE) &data, sizeof(WORD));
}

void CPluginStream::SaveString(LPCTSTR id, LPCTSTR data)
{
	if(!data) 
	{
		return;
	}
	saveData(id, SDT_DATA, (LPBYTE) data, (lstrlen(data) + 1) * sizeof(TCHAR));
}

void CPluginStream::SaveBIN(LPCTSTR id, LPBYTE data, DWORD cbData)
{
	if(!data) 
	{
		return;
	}
	saveData(id, SDT_DATA, data, cbData);
}

void CPluginStream::BeginNode(LPCTSTR id)
{
	STREAM_DATA dt;
	ZeroMemory(&dt, sizeof(dt));
	MAKE_STR(dt.id, id);
	dt.type = SDT_BEGINNODE;
	m_data.Add(&dt);
	int nodeIndex = m_data.GetCount() - 1;
	m_nodes.Add(&nodeIndex);
}

BOOL CPluginStream::EndNode()
{
	if(!m_nodes.GetCount())
	{
		return FALSE;
	}
	DWORD nodeIndex = m_nodes[m_nodes.GetCount() - 1];
	saveData(m_data[nodeIndex].id, SDT_ENDNODE, (LPBYTE) &nodeIndex, sizeof(nodeIndex));
	m_nodes.Delete(m_nodes.GetCount() - 1);
	return TRUE;
}

BOOL CPluginStream::OpenNode(LPCTSTR id)
{
	int nodeIndex = -1;
	if(m_nodes.GetCount()) 
	{
		nodeIndex = m_nodes[m_nodes.GetCount() - 1];
	}
	for(int i=nodeIndex + 1; i < m_data.GetCount(); i++)
	{
		if(nodeIndex >= 0 && m_data[i].type == SDT_ENDNODE && *((LPDWORD)m_data[i].lpData) == nodeIndex)
		{
			break;
		}
		if(!StrCmpI(m_data[i].id, id) && m_data[i].type == SDT_BEGINNODE)
		{
			nodeIndex = i;
			m_nodes.Add(&nodeIndex);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPluginStream::CloseNode()
{
	if(m_nodes.GetCount()) 
	{
		m_nodes.Delete(m_nodes.GetCount() - 1);
		return TRUE;
	}
	return FALSE;
}

void CPluginStream::saveData(LPCTSTR id, DWORD type, LPBYTE data, DWORD cbSize)
{
	STREAM_DATA dt;
	ZeroMemory(&dt, sizeof(dt));
	MAKE_STR(dt.id, id);
	dt.type = type;
	dt.cbSize = cbSize;
	if(cbSize) 
	{
		dt.lpData = new BYTE[cbSize];
		memcpy(dt.lpData, data, cbSize);
	}
	m_data.Add(&dt);
}
