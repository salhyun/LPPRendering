/*******************************
 *   @file		D3D9LPPRenderer.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2012.02.09
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include <vector>

class CD3D9MultipleRenderTargets ;
class CD3D9Environment ;
class CD3D9ShaderManager ;
class CRenderingIngredient ;
class CSecretLightProperty ;
class CD3D9PPLBuffer ;
class CHLSLLBuffer ;
class CD3D9RenderTarget ;
class CSecretShader ;

#define MAX_LBUFFER 2

class CD3D9LPPRenderer//Light-Pre Pass Rendering
{
public :

private :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	CD3D9MultipleRenderTargets *m_pcGBufferMRTs ;
	CD3D9PPLBuffer *m_pcLBuffer ;
	CD3D9ShaderManager *m_pcShaderManager ;
	CRenderingIngredient *m_pcRenderingIngredient ;

	std::vector<CSecretLightProperty *> m_cLightProperties ;

	void _setParameters(CHLSLLBuffer *pcLBuffer, CSecretLightProperty *pcLightProperty) ;

public :
	CD3D9LPPRenderer() ;
	~CD3D9LPPRenderer() ;
	void release() ;

	void initialize(CD3D9Environment *pcEnvironment, CD3D9ShaderManager *pcShaderManager) ;
	void render() ;
	void renderQuad(int x, int y, int width, int height) ;

	void storeBackBuffer() ;
	void setGBuffer() ;
	void restoreBackBuffer() ;

	void addLightProperty(CSecretLightProperty *pcLightProperty) ;//input after allocated
	CD3D9PPLBuffer *getCurrentLBuffer() ;
	void findGBufferShader(int nNodeKind, CSecretShader **shader, int *pass) ;
	CD3D9MultipleRenderTargets *getGBufferMRTs() ;
	std::vector<CSecretLightProperty *> *getLightProperites();

} ;
