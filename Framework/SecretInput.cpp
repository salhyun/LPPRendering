#include "SecretInput.h"

CSecretInput::CSecretInput()
{
	reset() ;
}
CSecretInput::~CSecretInput()
{
}
unsigned int CSecretInput::getKeyStatus()
{
	return (m_nKey>>16) ;
}
bool CSecretInput::isKeyEvent(char key)
{
	if((m_nKey&0x0000ffff) & key)
		return true ;
	return false ;
}
unsigned long CSecretInput::getKeyEvent()
{
	return (m_nKey&0x0000ffff) ;
}
unsigned int CSecretInput::getLButtonStatus()
{
	return (m_nLButton>>16) ;
}
unsigned int CSecretInput::getRButtonStatus()
{
	return (m_nRButton>>16) ;
}
void CSecretInput::receiveLButton(unsigned int status)
{
	m_nLButton |= ((status<<16)&0xffff0000) ;
}
void CSecretInput::receiveRButton(unsigned int status)
{
	m_nRButton |= ((status<<16)&0xffff0000) ;
}
void CSecretInput::receiveKeyEvent(unsigned int key, unsigned int status)
{
	m_nKey = (key&0x0000ffff) ;
	m_nKey |= ((status<<16)&0xffff0000) ;
}
void CSecretInput::reset()
{
	m_CurrentCursor.x = m_CurrentCursor.y = 0 ;
	m_PrevCursor.x = m_PrevCursor.y = 0 ;
	m_nKey = 0 ;

	m_nLButton = 0 ;
	m_nRButton = 0 ;
}

#include <windows.h>

bool ScanKeyCode(int nChar)
{
    if(GetAsyncKeyState(nChar) & 0x8000)
		return true ;
 
	return false ;
}
