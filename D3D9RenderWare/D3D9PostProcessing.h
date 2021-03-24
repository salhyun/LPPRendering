/*******************************
 *   @file		D3D9PostProcessing.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"

class CD3D9RenderTarget ;
class CD3D9Shader ;

#define D3DFVF_POSTPROCESSINGVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

class CD3D9PostProcessing
{
public :
	struct SVertex
	{
		D3DXVECTOR3 pos ;
		D3DXVECTOR2 tex ;
	} ;

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	CD3D9RenderTarget *m_pcRenderTarget ;
	CD3D9Shader *m_pcShader ;

	SVertex m_sVertices[4] ;

public :
	CD3D9PostProcessing() ;
	virtual ~CD3D9PostProcessing() ;

	void release() ;

	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader) ;
	virtual void render(int nPass=0) ;
	CD3D9RenderTarget *getRenderTarget() ;
	CD3D9Shader *getShader() ;

} ;