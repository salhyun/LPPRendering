#pragma once

#include "D3D9PostProcessing.h"

class CD3D9Environment;

class CD3D9PPSSAO : public CD3D9PostProcessing
{
public :
	LPDIRECT3DTEXTURE9 m_pTexNormal ;
	LPDIRECT3DTEXTURE9 m_pTexDepth ;
	LPDIRECT3DTEXTURE9 m_pTexRandom ;

	CD3D9RenderTarget *m_pcRTSSAO;

protected :

public :
	CD3D9PPSSAO() ;
	~CD3D9PPSSAO() ;

	void setSampleOffsets() ;
	void setMatInvProj(D3DXMATRIX *pmat) ;

	HRESULT initialize(CD3D9Environment *pcEnvironment, CD3D9Shader *pcShader);
	//HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader) ;

	virtual void render(int nPass=0) ;
} ;