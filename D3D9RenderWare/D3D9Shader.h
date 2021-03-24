/*******************************
 *   @file		D3D9Shader.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include <vector>
#include "SecretShader.h"

class CSecretMesh ;

class CRenderingIngredient
{
public :

	D3DXMATRIX m_matWorld ;
	D3DXMATRIX m_matView ;
	D3DXMATRIX m_matProj ;

	D3DXVECTOR3 m_vCameraPos ;
	D3DXVECTOR3 m_vLightDir ;

	float m_fNear, m_fFar ;
	int m_nTotalNode ;

public :
	CRenderingIngredient() ;
	~CRenderingIngredient() ;

} ;

#include <map>
#include <string>

class CD3D9Shader : public CSecretShader
{
public :

	CRenderingIngredient *m_pcIngredient ;

	LPD3DXEFFECT m_pEffect ;
	LPDIRECT3DDEVICE9 m_pd3dDevice ;

	D3DXHANDLE m_hTechnique ;

	char m_szName[256] ;
	char *m_pszShader ;

	//std::map<std::string, D3DXHANDLE> *m_pHandles ;
	//std::map<std::string, UINT> m_nPasses ;

	//¿Ã∆Â∆Æ∆ƒ¿œ Kind
	//Pass Attrubute

public :
	CD3D9Shader() ;
	virtual ~CD3D9Shader() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;
	void release() ;
	bool setHandle(D3DXHANDLE *pHandle, char *pszName) ;

	CRenderingIngredient *getIngredient() ;

	HRESULT setTechnique() ;
	HRESULT begin() ;
	HRESULT beginPass(int nPass) ;
	HRESULT endPass() ;
	HRESULT end() ;

	void _readLine(FILE *pf, char *pszString, char *pszLine, int &nLineCount, int &nSize) ;
	void _readRenderingAttributes(FILE *pf, int &nLineCount, int &nSize) ;
	void _readMaterialAttributes(FILE *pf, int &nLineCount, int &nSize) ;
	char *_readShaderFile(char *pszFileName) ;
	char *getName() ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void tuneParameters(float t) ;
	virtual void initializeParameters() ;
} ;
