#pragma once

#include "SecretObject.h"

class CSecretRoot ;
class CSecretUniqueNumber ;

//��鿡 ���� �⺻���� �Է±���ü
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

	//��� ���Ŭ������ ���������� ����ϴ� �Լ��� �����Լ��� �����Ѵ�.
	virtual bool Initialize() ;//����� �ʱ�ȭ �Ѵ�.
	virtual void PreProcess() ;//cpu�� �δ��ؾ� �Ǵ� ��� ������ �ʱ�ȭ
	virtual void Process() ;//cpu�� �δ��ؾ� �Ǵ� ���.
	virtual void PostProcess() ;//cpu�� �δ��ؾ� �Ǵ� ��� �Ŀ� ��������� �غ� ���� ������� ������

	virtual void Render() ;
	virtual void RenderText() ;
	virtual void Release() ;

	virtual long RestoreDevice() ;//����̽��� ���µǴ� ���(Alt+Tab, Minimize ect..), ����ʱ�ȭ������ ���� ���ȴ�.

	virtual void ProcessInputDevice(float fElaspedTime) ;//��ġ���� �Էµ� ���� ó���Ѵ�.
	virtual float GetTimeSinceLastFrame() ;
	char *GetSceneName() ;

	void setRoot(CSecretRoot *pcRoot) ;

} ;
