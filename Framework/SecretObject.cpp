#include "SecretObject.h"

CSecretObject *RTClass::CreateSecretObject()
{
	return (*lpfnCreateSecretObject)() ;
}

RTClass CSecretObject::classCSecretObject={"CSecretObject", sizeof(CSecretObject), NULL} ;
