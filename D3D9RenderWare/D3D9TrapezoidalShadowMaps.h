/************************************************
 *   Software, DGEntertaiment Inc., DAEGU, KOREA
 *   Copyright(c) 2011 by DGEntertaiment Inc.
 *************************************************/

/*******************************
 *   @file		D3D9TrapezoidalShadowMaps.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.05.16
 *   @note		SecretEngine for Direct3D9
 ********************************/
#pragma once

#include "D3D9Def.h"
#include "SecretTrapezoidalShadowMaps.h"

class CD3D9RenderTarget ;

class CD3D9TrapezoidalShadowMaps : public CSecretTrapezoidalShadowMaps
{
public :

protected :
	CD3D9RenderTarget *m_pcRenderTarget ;

public :
	CD3D9TrapezoidalShadowMaps() ;
	virtual ~CD3D9TrapezoidalShadowMaps() ;

	void release() ;
	void initialize(CD3D9RenderTarget *pcRenderTarget, CSecretShader *pcShader) ;
	CD3D9RenderTarget *getRenderTarget() ;

	virtual void storeBackbuffer() ;
	virtual void setRenderTarget() ;
	virtual void restoreBackbuffer() ;
} ;