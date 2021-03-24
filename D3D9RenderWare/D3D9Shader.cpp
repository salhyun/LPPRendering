#include "D3D9Shader.h"
#include <assert.h>
#include "SecretMesh.h"
#include "RenderingDef.h"
#include "SecretMaterial.h"

//#################################################//
//             CRenderingIngredient                //
//#################################################//
CRenderingIngredient::CRenderingIngredient()
{
	D3DXMatrixIdentity(&m_matWorld) ;
	D3DXMatrixIdentity(&m_matView) ;
	D3DXMatrixIdentity(&m_matProj) ;
}
CRenderingIngredient::~CRenderingIngredient()
{
}

//#################################################//
//                   CD3D9Shader                   //
//#################################################//
CD3D9Shader::CD3D9Shader()
{
	m_pEffect = NULL ;
	m_pd3dDevice = NULL ;
	m_pcIngredient = NULL ;
	sprintf_s(m_szName, "") ;
	m_pszShader = NULL ;
	m_nRenderingAttr = 0 ;
	m_nMaterialAttr = 0 ;
}
CD3D9Shader::~CD3D9Shader()
{
	release() ;
}
void CD3D9Shader::release()
{
	SAFE_RELEASE(m_pEffect) ;
	SAFE_DELETEARRAY(m_pszShader) ;
}
void CD3D9Shader::_readLine(FILE *pf, char *pszString, char *pszLine, int &nLineCount, int &nSize)
{
	char *result = fgets(pszLine, 256, pf) ;
	if(result)
	{
		nLineCount++ ;
		int l = strlen(pszLine) ;
		memcpy(pszString, pszLine, l) ;
		nSize += l+1 ;
		pszString[l] = '\0' ;
	}
	else
		pszString[0] = '\0' ;
}
void CD3D9Shader::_readRenderingAttributes(FILE *pf, int &nLineCount, int &nSize)
{
	char szLine[256], str[256] ;
	_readLine(pf, str, szLine, nLineCount, nSize) ;//read '{'

	while(true)
	{
		_readLine(pf, str, szLine, nLineCount, nSize) ;

		if(strstr(str, "LAMBERTLIGHTING"))
		{
			m_nRenderingAttr |= RENDERING_ATTR_LAMBERTLIGHTING ;
			TRACE("LAMBERTLIGHTING\r\n") ;
		}
		if(strstr(str, "RIGID"))
		{
			m_nRenderingAttr |= RENDERING_ATTR_RIGID ;
			TRACE("RIGID\r\n") ;
		}
		if(strstr(str, "TEXANI"))
		{
			m_nRenderingAttr |= RENDERING_ATTR_TEXANI ;
			TRACE("RIGID\r\n") ;
		}
		if(strstr(str, "SKINNING"))
		{
			m_nRenderingAttr |= RENDERING_ATTR_SKINNING ;
			TRACE("SKINNING\r\n") ;
		}

		if(strstr(str, "}"))
			break ;
	}
}
void CD3D9Shader::_readMaterialAttributes(FILE *pf, int &nLineCount, int &nSize)
{
	char szLine[256], str[256] ;

	_readLine(pf, str, szLine, nLineCount, nSize) ;//read '{'

	while(true)
	{
		_readLine(pf, str, szLine, nLineCount, nSize) ;

		if(strstr(str, "DIFFUSE"))
		{
			m_nMaterialAttr |= CSecretMaterial::MATERIAL_ATTR_DIFFUSE ;
			TRACE("MATERIAL_ATTR_DIFFUSE\r\n") ;
		}
		if(strstr(str, "NORMAL"))
		{
			m_nMaterialAttr |= CSecretMaterial::MATERIAL_ATTR_NORMAL ;
			TRACE("MATERIAL_ATTR_NORMAL\r\n") ;
		}
		if(strstr(str, "SPECULAR"))
		{
			m_nMaterialAttr |= CSecretMaterial::MATERIAL_ATTR_SPECULAR ;
			TRACE("MATERIAL_ATTR_SPECULAR\r\n") ;
		}
		if(strstr(str, "ALPHATEST"))
		{
			m_nMaterialAttr |= CSecretMaterial::MATERIAL_ATTR_ALPHATEST ;
			TRACE("MATERIAL_ATTR_ALPHATEST\r\n") ;
		}
		if(strstr(str, "ALPHABLEND"))
		{
			m_nMaterialAttr |= CSecretMaterial::MATERIAL_ATTR_ALPHABLEND ;
			TRACE("MATERIAL_ATTR_ALPHABLEND\r\n") ;
		}

		if(strstr(str, "}"))
			break ;
	}
}
char *CD3D9Shader::_readShaderFile(char *pszFileName)
{
	FILE *pf = fopen(pszFileName, "rt") ;
	if(pf == NULL)
		return NULL ;

	int nLineCount=0 ;
	int nTotalSize=0, nRenderingAttrSize=0, nMaterialAttrSize=0 ;

	char szLine[256], str[256] ;
	std::vector<std::string> lines ;
	while(!feof(pf))
	{
		_readLine(pf, str, szLine, nLineCount, nTotalSize) ;

		if(strstr(str, "RENDERING_ATTRIBUTES"))
		{
			_readRenderingAttributes(pf, nLineCount, nTotalSize) ;
			nRenderingAttrSize = nTotalSize ;
		}
		else if(strstr(str, "MATERIAL_ATTRIBUTES"))
		{
			_readMaterialAttributes(pf, nLineCount, nTotalSize) ;
			nMaterialAttrSize = nTotalSize-nRenderingAttrSize ;
		}
		else
			lines.push_back(str) ;
	}
	fclose(pf) ;

	int nSize=0 ;
	std::vector<std::string>::iterator it ;
	for(it=lines.begin(); it!=lines.end(); it++)
		nSize += (*it).size() ;

	if(nSize <= 0)
		return NULL ;

	nSize += 16 ;

	char *pszResult = new char[nSize] ;
	ZeroMemory(pszResult, sizeof(char)*nSize) ;

	for(it=lines.begin(); it!=lines.end(); it++)
	{
		strcat(pszResult, (*it).c_str()) ;
	}

	return pszResult ;
}

HRESULT CD3D9Shader::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	HRESULT hr ;
	LPD3DXBUFFER pError=NULL ;

	m_pd3dDevice = pd3dDevice ;

	m_pszShader = _readShaderFile(pszFileName) ;
	int nSize = strlen(m_pszShader) ;


#ifdef _SHADERDEBUG_
	if(FAILED(hr=D3DXCreateEffectFromFile(pd3dDevice, pszFileName, NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_pEffect, &pError)))
	{
		MessageBox(NULL, (LPCTSTR)pError->GetBufferPointer(), "Error", MB_OK) ;
		return hr ;
	}
#else
	if(FAILED(hr=D3DXCreateEffect(m_pd3dDevice, m_pszShader, nSize, NULL, NULL, 0, NULL, &m_pEffect, &pError)))
	//if(FAILED(hr=D3DXCreateEffectFromFile(m_pd3dDevice, pszFileName, NULL, NULL, 0, NULL, &m_pEffect, &pError)))
	{
		MessageBox(NULL, (LPCTSTR)pError->GetBufferPointer(), "Error", MB_OK) ;
		return hr ;
	}
#endif
	else
	{
		if((m_hTechnique = m_pEffect->GetTechniqueByName("TShader")) == NULL)
		{
			assert(false && "hTechnique is NULL (TShader)") ;
			return hr ;
		}
	}
	SAFE_RELEASE(pError) ;

	GetFileNameFromPath(m_szName, pszFileName) ;

	return hr ;
}
char *CD3D9Shader::getName()
{
	return m_szName ;
}
bool CD3D9Shader::setHandle(D3DXHANDLE *pHandle, char *pszName)
{
	if((*pHandle = m_pEffect->GetParameterByName(NULL, pszName)) == NULL)
	{
		char err[128] ;
		sprintf_s(err, 128, "%s Handle is NULL", pszName) ;
		assert(false && err) ;
		return false ;
	}
	return true ;
}
CRenderingIngredient *CD3D9Shader::getIngredient()
{
	return m_pcIngredient ;
}
HRESULT CD3D9Shader::setTechnique()
{
	return m_pEffect->SetTechnique(m_hTechnique) ;
}
HRESULT CD3D9Shader::begin()
{
	return m_pEffect->Begin(NULL, 0) ;
}
HRESULT CD3D9Shader::beginPass(int nPass)
{
	return m_pEffect->BeginPass(nPass) ;
}
HRESULT CD3D9Shader::endPass()
{
	return m_pEffect->EndPass() ;
}
HRESULT CD3D9Shader::end()
{
	return m_pEffect->End() ;
}
void CD3D9Shader::setParametersToShader(CSecretMesh *pcMesh)
{
}
void CD3D9Shader::tuneParameters(float t)
{
}
void CD3D9Shader::initializeParameters()
{
}
