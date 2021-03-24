#include "D3D9Material.h"

CD3D9Material::CD3D9Material()
{
	m_pTexDiffuse = NULL ;
	m_pTexNormal = NULL ;
	m_pTexSpecular = NULL ;
}
CD3D9Material::~CD3D9Material()
{
	release() ;
}
void CD3D9Material::release()
{
}
