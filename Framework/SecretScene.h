#pragma once

#include "SecretObject.h"

class CSecretRoot ;
class CSecretUniqueNumber ;

//장면에 대한 기본적인 입력구조체
struct SSceneInput
{
	float fPitch, fYaw, fRoll ;
	float fLeftRight, fUpDown, fZoom ;

	SSceneInput() ;
	virtual ~SSceneInput() ;
} ;

class CSecretScene : public CSecretObject
{
//Member Variable
private :

protected :
	char m_szSceneName[256] ;

public :
	float m_fElapsedTime ;
	float m_fTimeSinceLastFrame ;
	int m_nfps ;

//Member Function
protected :
	CSecretRoot *m_pcRoot ;
	bool m_bEnableRender ;
	CSecretUniqueNumber *m_pcUniqueNumber ;

public :
	DeclareDynamic(CSecretScene)
    DeclareDynCreate(CSecretScene)

	CSecretScene() ;
	virtual ~CSecretScene() ;

	//모든 장면클래스가 공통적으로 사용하는 함수를 가상함수로 선언한다.
	virtual bool Initialize() ;//장면을 초기화 한다.
	virtual void PreProcess() ;//cpu가 부담해야 되는 계산 이전에 초기화
	virtual void Process() ;//cpu가 부담해야 되는 계산.
	virtual void PostProcess() ;//cpu가 부담해야 되는 계산 후에 다음장면의 준비를 위한 현재장면 마무리

	virtual void Render() ;
	virtual void RenderText() ;
	virtual void Release() ;

	virtual long RestoreDevice() ;//디바이스가 리셋되는 경우(Alt+Tab, Minimize ect..), 장면초기화에서도 같이 사용된다.

	virtual void ProcessInputDevice(float fElaspedTime) ;//장치에서 입력된 값을 처리한다.
	virtual float GetTimeSinceLastFrame() ;
	char *GetSceneName() ;

	void setRoot(CSecretRoot *pcRoot) ;

} ;
