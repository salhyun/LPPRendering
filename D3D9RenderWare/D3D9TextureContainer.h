/*******************************
 *   @file		D3D9TextureContainer.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.01
 *   @note		SecretEngine for Direct3D9
 ********************************/
#pragma once

#include "D3D9Def.h"
#include "SecretTextureContainer.h"
#include <map>
#include <string>

class CD3D9TextureContainer : public CSecretTextureContainer
{
public :

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	std::map<std::string, LPDIRECT3DTEXTURE9> m_d3dTextures ;

	HRESULT _createD3DTexture(STexture *psTex) ;
	LPDIRECT3DTEXTURE9 _createTextureFromTGA(STexture *psTex) ;
	LPDIRECT3DTEXTURE9 _createTextureFromDDS(STexture *psTex) ;

public :
	CD3D9TextureContainer(LPDIRECT3DDEVICE9 pd3dDevice) ;
	virtual ~CD3D9TextureContainer() ;

	void release() ;

	bool importFile(char *pszFileName) ;
	LPDIRECT3DTEXTURE9 findD3DTexture(char *pszName) ;
} ;
