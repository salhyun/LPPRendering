#pragma once

#include "D3D9PostProcessing.h"

class CD3D9PPLBuffer : public CD3D9PostProcessing
{
public :
	LPDIRECT3DTEXTURE9 m_pTexNormal ;
	LPDIRECT3DTEXTURE9 m_pTexDepth ;
	LPDIRECT3DTEXTURE9 m_pTexSpecularParam ;
	LPDIRECT3DTEXTURE9 m_pTexPrev ;

protected :


public :
	CD3D9PPLBuffer() ;
	~CD3D9PPLBuffer() ;

	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader) ;

	virtual void render(int nPass=0) ;
} ;