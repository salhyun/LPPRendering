/*******************************
 *   @file		D3D9Model.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include "SecretModel.h"

class CD3D9TextureContainer ;
class CD3D9ShaderManager ;
class CD3D9Material ;
class CD3D9Shader ;

class CD3D9Model : public CSecretModel
{
public :

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;

	CD3D9Material *m_pcD3D9Materials ;

	void _setParent(CSecretNode *node) ;
	void _setMaterial(CD3D9Material *dest, SMaterial *src) ;

public :
	CD3D9ShaderManager *m_pcShaderManager ;

public :
	CD3D9Model(LPDIRECT3DDEVICE9 pd3dDevice) ;
	virtual ~CD3D9Model() ;

	void release() ;
	void initialize(CASEData *pcASEData, CD3D9TextureContainer *pcTexContainer, CD3D9ShaderManager *pcShaderManager) ;

	virtual void render(CSecretNode *pcNode, Matrix4 *pmat, CSecretShader *pcShader=NULL, int nPass=0) ;

	virtual void render(CSecretMesh *pcNode, CSecretTNRModel *pcTNRModel, CSecretShader *pcShader=NULL, int nPass=0) ;
	virtual void render(CSecretTNRModel *pcTNRModel, CSecretShader *pcShader=NULL, int nPass=0) ;

	virtual CSecretMaterial *getMaterial(int nNum) ;
} ;
