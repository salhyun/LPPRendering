#include "D3D9TextureContainer.h"
#include "targa.h"

CD3D9TextureContainer::CD3D9TextureContainer(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice ;
}
CD3D9TextureContainer::~CD3D9TextureContainer()
{
	release() ;
}
void CD3D9TextureContainer::release()
{
	std::map<std::string, LPDIRECT3DTEXTURE9>::iterator it ;
	for(it=m_d3dTextures.begin(); it!=m_d3dTextures.end(); it++)
		SAFE_RELEASE((*it).second) ;
}
LPDIRECT3DTEXTURE9 CD3D9TextureContainer::_createTextureFromTGA(STexture *psTex)
{
	tgamem *mem = new tgamem() ;
	tga_image *tga = new tga_image ;

	sprintf_s(mem->szName, 128, "%s", psTex->szName) ;
	mem->totalsize = psTex->nBufferSize ;
	mem->buffer = (uint8_t *)psTex->pBuffer ;

	tga_MEMRead(tga, mem) ;
	D3DFORMAT fmt = D3DFMT_X8R8G8B8 ;
	if(tga->pixel_depth == 24)
		fmt = D3DFMT_X8R8G8B8 ;
	else if(tga->pixel_depth == 32)
		fmt = D3DFMT_A8R8G8B8 ;

	LPDIRECT3DTEXTURE9 tex ;
	m_pd3dDevice->CreateTexture(tga->width, tga->height, 0, D3DUSAGE_AUTOGENMIPMAP, fmt, D3DPOOL_MANAGED, &tex, NULL) ;
	FillTextureWithBuffer(tex, tga->image_data, tga->pixel_depth) ;
	tex->GenerateMipSubLevels() ;

	m_d3dTextures.insert(std::pair<std::string, LPDIRECT3DTEXTURE9>(psTex->szName, tex)) ;

	tga_free_buffers(tga) ;
	delete tga ;
	delete mem ;

	return tex ;
}
LPDIRECT3DTEXTURE9 CD3D9TextureContainer::_createTextureFromDDS(STexture *psTex)
{
	LPDIRECT3DTEXTURE9 tex=NULL ;

	return tex ;
}
HRESULT CD3D9TextureContainer::_createD3DTexture(STexture *psTex)
{
	switch(psTex->nType)
	{
	case TYPE_TGA :
		_createTextureFromTGA(psTex) ;
		break ;
	case TYPE_DDS :
		break ;
	default :
		break ;
	} ;

	return S_OK ;
}
bool CD3D9TextureContainer::importFile(char *pszFileName)
{
	CSecretTextureContainer::importFile(pszFileName) ;
	
	std::vector<STexture *>::iterator it ;
	for(it=m_sTextures.begin() ; it!=m_sTextures.end() ; it++)
	{
		_createD3DTexture((*it)) ;
	}
	return true ;
}
LPDIRECT3DTEXTURE9 CD3D9TextureContainer::findD3DTexture(char *pszName)
{
	return m_d3dTextures[pszName] ;
}