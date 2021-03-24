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

struct RTClass//�ڵ��ڵ������ ����
{
	char m_lpszClassName[64] ;//Ŭ���� �̸��� �����Ѵ�.
	int m_nClassSize ;//Ŭ���� ũ��.
	CSecretObject *(*lpfnCreateSecretObject)() ;//�������� �����Ǵ� �Լ������͸� �����Ѵ�.
	CSecretObject *CreateSecretObject() ;//�������� �����Ǵ� �Լ� (*lpfnCreateSecretObject) �Լ��� �����Ѵ�.
} ;

//RTTI(Run-Time Type Information)�� �������� ������� ���� ���� Ŭ����.
class CSecretObject
{
public :
	DeclareDynamic(CSecretObject) //static RTClass classCSecret ;
    virtual RTClass *GetRTClass()	{return NULL ;}

	CSecretObject()	{}
	virtual ~CSecretObject()	{}

} ;//class CSecret
