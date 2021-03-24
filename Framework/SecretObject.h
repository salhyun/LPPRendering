#pragma once

#include <stdio.h>

#define RuntimeClass(ClassName) (&ClassName::class##ClassName)

#define DeclareDynamic(ClassName) static RTClass class##ClassName ;

#define ImplementDynamic(ClassName) RTClass ClassName::class##ClassName = {\
	(#ClassName), sizeof(ClassName), ClassName::CreateSecretObject} ;

#define DeclareDynCreate(ClassName) static CSecretObject *CreateSecretObject() ;

#define ImplementDynCreate(ClassName) CSecretObject *ClassName::CreateSecretObject() {\
	return new ClassName;}

class CSecretObject ;

struct RTClass//자동코드생성기 역할
{
	char m_lpszClassName[64] ;//클래스 이름을 저장한다.
	int m_nClassSize ;//클래스 크기.
	CSecretObject *(*lpfnCreateSecretObject)() ;//동적으로 생성되는 함수포인터를 저장한다.
	CSecretObject *CreateSecretObject() ;//동적으로 생성되는 함수 (*lpfnCreateSecretObject) 함수를 리턴한다.
} ;

//RTTI(Run-Time Type Information)의 개념으로 만들어진 가장 상위 클래스.
class CSecretObject
{
public :
	DeclareDynamic(CSecretObject) //static RTClass classCSecret ;
    virtual RTClass *GetRTClass()	{return NULL ;}

	CSecretObject()	{}
	virtual ~CSecretObject()	{}

} ;//class CSecret
