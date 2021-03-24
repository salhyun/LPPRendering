#pragma once

#include "D3D9PostProcessing.h"

class CD3D9PPLPPFinal : public CD3D9PostProcessing
{
public :

protected :


public :
	CD3D9PPLPPFinal() ;
	~CD3D9PPLPPFinal() ;

	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader) ;

	virtual void render(int nPass=0) ;
} ;