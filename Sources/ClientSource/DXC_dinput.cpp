// DXC_dinput.cpp: implementation of the DXC_dinput class.
//
//////////////////////////////////////////////////////////////////////

#include "DXC_dinput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <windowsx.h>
#include "Game.h"

extern CGame * G_pGame;

DXC_dinput::DXC_dinput()
{
	m_pDI    = NULL;
	m_pMouse = NULL;
	m_sX     = 0;
	m_sY     = 0;
	m_sZ     = 0;
}

DXC_dinput::~DXC_dinput()
{
	if (m_pMouse != NULL) {
		m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = NULL;
	}
	if (m_pDI != NULL) {
		m_pDI->Release();
        m_pDI = NULL;
	}
}

BOOL DXC_dinput::bInit(HWND hWnd, HINSTANCE hInst)
{
	if(_new_method)
		return TRUE;

 HRESULT hr;
 DIMOUSESTATE dims;	
 POINT Point;

	GetCursorPos(&Point);
	m_sX     = (short)(Point.x);
	m_sY     = (short)(Point.y); 

	hr = DirectInputCreate( hInst, DIRECTINPUT_VERSION, &m_pDI, NULL );
    if (hr != DI_OK) return FALSE;
	hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
	if (hr != DI_OK) return FALSE;
	hr = m_pMouse->SetDataFormat( &c_dfDIMouse );
	if (hr != DI_OK) return FALSE;
	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (hr != DI_OK) return FALSE;

//	m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	if ( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK )
	{
		m_pMouse->Acquire();
		//return TRUE;
	}

	return TRUE;
}


void DXC_dinput::SetAcquire(BOOL bFlag)
{
	if(_new_method)
		return;

 DIMOUSESTATE dims;

	if (m_pMouse == NULL) return;
	if (bFlag == TRUE) {
		m_pMouse->Acquire();
		m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	}
	else m_pMouse->Unacquire();
}

void DXC_dinput::UpdateMouseState(short * pX, short * pY, short * pZ, char * pLB, char * pRB)
{
	if(_new_method)
	{
		*pX = X;
		*pY = Y;
		*pZ = m_sZ;
		*pLB = LB;
		*pRB = RB;
		return;
	}

	if ( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK )
	{
		m_pMouse->Acquire();
		return;
	}
	m_sX += (short)dims.lX;
	m_sY += (short)dims.lY;
	if( (short)dims.lZ != 0 )m_sZ = (short)dims.lZ;
	if (m_sX < 0) m_sX = 0;
	if (m_sY < 0) m_sY = 0;
	if (m_sX > G_pGame->m_DDraw._res_x - 1) m_sX = G_pGame->m_DDraw._res_x - 1;
	if (m_sY > G_pGame->m_DDraw._res_y - 1) m_sY = G_pGame->m_DDraw._res_y - 1;
	*pX = m_sX;
	*pY = m_sY;
	*pZ = m_sZ;
	*pLB = (char)dims.rgbButtons[0];
	*pRB = (char)dims.rgbButtons[1];
}


void DXC_dinput::Update(UINT message, WPARAM wParam, LPARAM lParam) {
	if(!_new_method)
		return;

	switch(message) {
	case WM_MOUSEMOVE:
		X = GET_X_LPARAM(lParam);
		Y = GET_Y_LPARAM(lParam);
		LB = (LOWORD(wParam) & MK_LBUTTON) ? true : false;
		RB = (LOWORD(wParam) & MK_RBUTTON) ? true : false;
		MB = (LOWORD(wParam) & MK_MBUTTON) ? true : false;
		break;

	case WM_LBUTTONUP:
		LB = false;
		break;

	case WM_MBUTTONUP:
		MB = false;
		break;
		
	case WM_RBUTTONUP:
		RB = false;
		break;

	case WM_LBUTTONDOWN:
	{
		static DWORD lastClickTime = 0;
		LB = true;
		DbClick = lastClickTime > timeGetTime();		
		//G_pGame->AddEventList(format("{}: {} > {}", DbClick, lastClickTime, timeGetTime()).data());
		lastClickTime = timeGetTime() + 300;
		break;
	}

	case WM_RBUTTONDOWN:
		RB = true;
		break;

	case WM_MOUSEWHEEL:
		m_sZ = GET_WHEEL_DELTA_WPARAM(wParam);
		break;

	case WM_MBUTTONDOWN:
		MB = true;
		break;
	}

	if(X > G_pGame->m_DDraw._res_x-1)
		X = G_pGame->m_DDraw._res_x-1;

	if(X < 0)
		X = 0;

	if(Y > G_pGame->m_DDraw._res_y-1)
		Y = G_pGame->m_DDraw._res_y-1;

	if(Y < 0)
		Y = 0;
}