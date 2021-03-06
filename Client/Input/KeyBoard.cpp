#include "StdAfx.h"
#include "KeyBoard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKeyBoard::CKeyBoard()
{
	m_pDI = NULL;
	m_pdidKeyboard = NULL;
	m_dwItems = KEYBUFFERSIZE;
	m_lFirstKey = 0;
}

CKeyBoard::~CKeyBoard()
{
	m_pdidKeyboard->Unacquire();
	SAFE_RELEASE( m_pdidKeyboard );
    SAFE_RELEASE( m_pDI );
}

// -------------------------------------------------------
// Name: Create()
// Describe: 初始化键盘硬件设备(DInput)
// -------------------------------------------------------
bool CKeyBoard::Create(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
    if( FAILED( hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI,
                                        NULL) ) )
    {
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "DirectInput 8 KeyBoard Initialize Error!" );
		return false;
    }

    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pdidKeyboard,
                                          NULL ) ) )
    {
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "DirectInput 8 KeyBoard Initialize Error! - CreateDevice - " );
		return false;
    }

    if( FAILED( hr = m_pdidKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "DirectInput 8 KeyBoard Initialize Error! - SetDataFormat - " );
		return false;
    }

    if( FAILED( hr = m_pdidKeyboard->SetCooperativeLevel( hWnd,
                                    DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
    {
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"DirectInput 8 KeyBoard Initialize Error! - SetCooperativeLevel - " );
		return false;
    }

	DIPROPDWORD property;
	property.diph.dwSize = sizeof(DIPROPDWORD);
	property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	property.diph.dwObj = 0;
	property.diph.dwHow = DIPH_DEVICE;
	property.dwData = KEYBUFFERSIZE;

	if FAILED( hr = m_pdidKeyboard->SetProperty(DIPROP_BUFFERSIZE, &property.diph) )
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"DirectInput 8 KeyBoard Initialize Error! - SetProperty - " );
		return false;
    }

	//得到设备通道
	if( FAILED( hr=m_pdidKeyboard->Acquire() ) )
	{
		switch (hr)
		{
		case S_FALSE:
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"DirectInput 8 KeyBoard Acquire Error! - S_FALSE - " );
			break;
		case DIERR_INVALIDPARAM:
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"DirectInput 8 KeyBoard Acquire Error! - DIERR_INVALIDPARAM - " );
			break;
		case DIERR_NOTINITIALIZED:
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "DirectInput 8 KeyBoard Acquire Error! - DIERR_NOTINITIALIZED - " );
			break;
		case DIERR_OTHERAPPHASPRIO:
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "DirectInput 8 KeyBoard Acquire Error! - DIERR_OTHERAPPHASPRIO - " );
			break;
		}

		return false;
	}

	memset(KeyBuffer, 0, KEYBUFFERSIZE);			// initialize key buffer
//	KeyBuffer_Start = KeyBuffer_End = 0;

	return true;
}

// -------------------------------------------------------
// Name: RecieveKeyboardInput()
// Describe: 获取键盘输入数据
// -------------------------------------------------------
void CKeyBoard::RecieveKeyboardInput()
{
	// 即时数据
	QueryKeyboardState(ks);

	// 缓冲数据
//	GetKeyData();
//	m_Key=GetKeyBuffer();
}

// -------------------------------------------------------
// Name: QueryKeyboardState()
// Describe: 询问键盘的输入
// -------------------------------------------------------
void CKeyBoard::QueryKeyboardState(KEYBOARDSTATE& ks)
{
    extern bool g_bInIME;
	if (g_bInIME)
	    return;
    HRESULT hr;

	hr = DIERR_INPUTLOST;

	// if input is lost then acquire and keep trying 
	while ( DIERR_INPUTLOST == hr ) 
	{
		hr = m_pdidKeyboard->GetDeviceState( sizeof(ks), &ks );
		if ( hr == DIERR_INPUTLOST )
		{
			hr = m_pdidKeyboard->Acquire();
//			PutDebugString("failed in dinput(KEYBOARD): DIERR_INPUTLOST");
		}
		if (hr == DIERR_INVALIDPARAM )
		{
			Log4c::Warn(ROOT_MODULE,"failed in dinput(KEYBOARD): DIERR_INVALIDPARAM");
			assert(0);
		}
		if (hr == DIERR_NOTACQUIRED )
		{
			hr = m_pdidKeyboard->Acquire();
//			PutDebugString("failed in dinput(KEYBOARD): DIERR_NOTACQUIRED");
		}
		if (hr == DIERR_NOTINITIALIZED )
		{
			Log4c::Warn(ROOT_MODULE,"failed in dinput(KEYBOARD): DIERR_NOTINITIALIZED");
			assert(0);
		}
	}

	if (FAILED(hr))
	{
		memset(&ks,0,sizeof(KEYBOARDSTATE));
		return;
	}

	m_dwItems = KEYBUFFERSIZE;
	hr = m_pdidKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), Object, &m_dwItems, 0);

	if (m_bNextQueryInvalid)
	{
		m_bNextQueryInvalid = false;
		ClearUpData();
		return;
	}

    for(DWORD i=0; i<m_dwItems; i++)
    {
        if((Object[i].dwData & 0x80))
        {
            long value = Object[i].dwOfs;
            m_queueKey.push( value );
        }
        else
        {
            long value = Object[i].dwOfs+128;
            m_queueKey.push( value );
        }
    }

	if( m_queueKey.size() == 0)
		m_lFirstKey = 0;
	else
	{
		m_lFirstKey = (unsigned char)m_queueKey.front();
		m_queueKey.pop();
	}

	if ( hr != DI_OK )
	{
		hr = m_pdidKeyboard->Acquire();
		Log4c::Warn(ROOT_MODULE,"failed in dinput(KEYBOARD): DI_BUFFEROVERFLOW");
	}

}

// -------------------------------------------------------
// Name: IsKeyDown()
// Describe: 检测某键是否被按下
// -------------------------------------------------------
bool CKeyBoard::IsKeyDown(long value)
{
	if( ks.key[value] & 0x80 )
	{
		return true;
	}
	return false;
}

// -------------------------------------------------------
// Name: ClearUpData()
// Describe: 清除键盘数据(但不释放设备)
// -------------------------------------------------------
void CKeyBoard::ClearUpData()
{
	m_lFirstKey = 0;
	ZeroMemory(&ks, sizeof(KEYBOARDSTATE));
	ZeroMemory(Object, sizeof(DIDEVICEOBJECTDATA) * KEYBUFFERSIZE);

	while (!m_queueKey.empty())
        m_queueKey.pop();
}

// -------------------------------------------------------
// Name: GetCurrentKey()
// Describe: 获取当前输入
// -------------------------------------------------------
unsigned char CKeyBoard::GetCurrentKey()
{
	return m_lFirstKey;
}

// -------------------------------------------------------
// Name: SetCurrentKey()
// Describe: 设置当前输入
// -------------------------------------------------------
void CKeyBoard::SetCurrentKey(unsigned char value)
{
	m_lFirstKey = value;
}
