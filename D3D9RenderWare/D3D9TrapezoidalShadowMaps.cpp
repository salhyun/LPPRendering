#include "D3D9TrapezoidalShadowMaps.h"
#include "D3D9RenderTarget.h"

CD3D9TrapezoidalShadowMaps::CD3D9TrapezoidalShadowMaps()
{
	m_pcRenderTarget = NULL ;
}
CD3D9TrapezoidalShadowMaps::~CD3D9TrapezoidalShadowMaps()
{
	release() ;
}
void CD3D9TrapezoidalShadowMaps::release()
{
	SAFE_DELETE(m_pcRenderTarget) ;
}
void CD3D9TrapezoidalShadowMaps::initialize(CD3D9RenderTarget *pcRenderTarget, CSecretShader *pcShader)
{
	m_pcRenderTarget = pcRenderTarget ;
	m_pcShader = pcShader ;
}
CD3D9RenderTarget *CD3D9TrapezoidalShadowMaps::getRenderTarget()
{
	return m_pcRenderTarget ;
}
void CD3D9TrapezoidalShadowMaps::storeBackbuffer()
{
	m_pcRenderTarget->storeBackBuffer() ;
}
void CD3D9TrapezoidalShadowMaps::setRenderTarget()
{
	m_pcRenderTarget->setRenderTarget() ;
}
void CD3D9TrapezoidalShadowMaps::restoreBackbuffer()
{
	m_pcRenderTarget->restoreBackbuffer() ;
}
