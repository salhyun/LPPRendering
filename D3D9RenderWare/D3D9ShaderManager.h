/*******************************
 *   @file		D3D9ShaderManager.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include <vector>

class CD3D9Shader ;

class CD3D9ShaderManager
{
public :

private :
	std::vector<CD3D9Shader *> *m_pcShaders ;

public :
	CD3D9ShaderManager() ;
	~CD3D9ShaderManager() ;

	void release() ;
	void initialize() ;
	void addShader(CD3D9Shader *pcShader) ;
	CD3D9Shader *getShader(int nIndex) ;
	CD3D9Shader *findShader(char *pszName) ;
	CD3D9Shader *getSuitedShader(int nRenderingAttr, int nMaterialAttr) ;

} ;