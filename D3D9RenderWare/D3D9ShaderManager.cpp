#include "D3D9ShaderManager.h"
#include "D3D9Shader.h"

CD3D9ShaderManager::CD3D9ShaderManager()
{
	m_pcShaders = NULL ;
}
CD3D9ShaderManager::~CD3D9ShaderManager()
{
	release() ;
}
void CD3D9ShaderManager::release()
{
	if(m_pcShaders)
	{
		std::vector<CD3D9Shader *>::iterator it ;
		for(it=m_pcShaders->begin(); it!=m_pcShaders->end(); it++)
			delete (*it) ;
		delete m_pcShaders ;
		m_pcShaders = NULL ;
	}
}
void CD3D9ShaderManager::initialize()
{
	m_pcShaders = new std::vector<CD3D9Shader *> ;
}
void CD3D9ShaderManager::addShader(CD3D9Shader *pcShader)
{
	m_pcShaders->push_back(pcShader) ;
}
CD3D9Shader *CD3D9ShaderManager::getShader(int nIndex)
{
	return m_pcShaders->at(nIndex) ;
}
CD3D9Shader *CD3D9ShaderManager::getSuitedShader(int nRenderingAttr, int nMaterialAttr)
{
	std::vector<CD3D9Shader *>::iterator it ;
	for(it=m_pcShaders->begin(); it!=m_pcShaders->end(); it++)
	{
		if(((*it)->m_nRenderingAttr == nRenderingAttr) && ((*it)->m_nMaterialAttr == nMaterialAttr))
			return (*it) ;
	}
	return NULL ;
}
CD3D9Shader *CD3D9ShaderManager::findShader(char *pszName)
{
	std::vector<CD3D9Shader *>::iterator it ;
	for(it=m_pcShaders->begin(); it!=m_pcShaders->end(); it++)
	{
		if( !strcmp(pszName, (*it)->getName()) )
			return (*it) ;
	}
	return NULL ;
}
