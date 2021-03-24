#pragma once

class CSecretInput
{
public :
	struct SCursor
	{
		int x ;
		int y ;
	} ;

	enum KEYSTATUS
	{
		KEYSTA_UP=0,
		KEYSTA_DOWN
	} ;

	SCursor m_CurrentCursor ;
	SCursor m_PrevCursor ;

	unsigned int m_nLButton ;
	unsigned int m_nRButton ;
	unsigned int m_nKey ;

	CSecretInput() ;
	~CSecretInput() ;

	//key hiword is key status
	//key loword is key event

	unsigned int getKeyStatus() ;
	bool isKeyEvent(char key) ;
	unsigned long getKeyEvent() ;

	unsigned int getLButtonStatus() ;
	unsigned int getRButtonStatus() ;

	void receiveLButton(unsigned int status) ;
	void receiveRButton(unsigned int status) ;
	void receiveKeyEvent(unsigned int key, unsigned int status) ;

	void reset() ;
} ;

bool ScanKeyCode(int nChar) ;
