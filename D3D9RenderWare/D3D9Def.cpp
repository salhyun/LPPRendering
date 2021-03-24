#include "D3D9Def.h"

void SCVertex::set(D3DXVECTOR3 &p, DWORD dwColor)
{
	pos = p ;
	color = dwColor ;
}

bool CopyTexture(LPDIRECT3DTEXTURE9 pTexSrc, LPDIRECT3DTEXTURE9 pTexDest, LPDIRECT3DDEVICE9 pd3dDevice)
{
    LPDIRECT3DSURFACE9 pSrcSurf=NULL ;
    D3DSURFACE_DESC desc ;
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC)) ;
	pTexSrc->GetSurfaceLevel(0, &pSrcSurf) ;
	pSrcSurf->GetDesc(&desc) ;

	//if(FAILED(D3DXCreateTexture(pd3dDevice, desc.Width, desc.Height, 0, 0, D3DFMT_R8G8B8, D3DPOOL_MANAGED, &pTexDest)))
	//{
	//	char szMsg[256] ;
	//	sprintf(szMsg, "Error CreateTexture") ;
	//	assert(false && szMsg) ;
	//	return false ;
	//}

    LPDIRECT3DSURFACE9 pDestSurf=NULL ;
	pTexDest->GetSurfaceLevel(0, &pDestSurf) ;

	D3DXLoadSurfaceFromSurface(pDestSurf, NULL, NULL, pSrcSurf, NULL, NULL, D3DX_FILTER_BOX, 0) ;

	pSrcSurf->Release() ;
	pDestSurf->Release() ;

	return true ;
}

void FillTextureWithColor(LPDIRECT3DTEXTURE9 pTex, DWORD dwColor, int nMapLevel)
{
	D3DSURFACE_DESC desc ;
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC)) ;
	pTex->GetLevelDesc(nMapLevel, &desc) ;

	DWORD *pdwColor ;
	D3DLOCKED_RECT d3drect ;
	pTex->LockRect(nMapLevel, &d3drect, NULL, D3DLOCK_NO_DIRTY_UPDATE) ;
	pTex->AddDirtyRect(NULL) ;
	pdwColor = (DWORD *)d3drect.pBits ;

	for(int y=0; y<(int)desc.Height; y++)
	{
		for(int x=0; x<(int)desc.Width; x++, pdwColor++)
		{
			*pdwColor = dwColor ;
		}
	}
	pTex->UnlockRect(nMapLevel) ;
}
void FillTexture32WithBuffer(LPDIRECT3DTEXTURE9 pTex, DWORD *pdwSrcColor, int nColorDepth, int nMapLevel)
{
	D3DSURFACE_DESC desc ;
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC)) ;
	pTex->GetLevelDesc(nMapLevel, &desc) ;

	DWORD *pdwColor ;
	D3DLOCKED_RECT d3drect ;
	pTex->LockRect(nMapLevel, &d3drect, NULL, D3DLOCK_NO_DIRTY_UPDATE) ;
	pTex->AddDirtyRect(NULL) ;
	pdwColor = (DWORD *)d3drect.pBits ;
	int nDepthSize = nColorDepth/8 ;

	for(int y=0; y<(int)desc.Height; y++)
	{
		for(int x=0; x<(int)desc.Width; x++, pdwColor++, pdwSrcColor++)
		{
			*pdwColor = *pdwSrcColor ;
		}
	}
	pTex->UnlockRect(nMapLevel) ;
}
DWORD GetColorfromBuffer(BYTE *pbyBuffer, int nDepthSize, D3DFORMAT fmt)
{
	DWORD value=0 ;

    if(nDepthSize == 1)//8bit color
	{
		BYTE byColor = *pbyBuffer ;

		if(fmt == D3DFMT_X8R8G8B8)
			value = 0x00000000|(byColor<<16)|(byColor<<8)|(byColor) ;
		else if(fmt == D3DFMT_A8R8G8B8)
			value = 0xff000000|(byColor<<16)|(byColor<<8)|(byColor) ;
	}
	else if(nDepthSize == 3)//24bit color
	{
		//TRACE("r=%x, g=%x b=%x\r\n", *pbyBuffer, *(pbyBuffer+1), *(pbyBuffer+2)) ;

			//       alpha              red                green                blue
			value = 0xff000000 | ((*(pbyBuffer+2))<<16) | ((*(pbyBuffer+1))<<8) | (*(pbyBuffer)) ;
	}
	else if(nDepthSize == 4)//32bit color
	{//DWORD를 BYTE로 바꿔서 0xffffffff 이 원본이 바이트형태로 되어버려서 가장끝부분부터 액세스가 되고 앞쪽으로 가게 된다. 뒤에서부터 읽어드린다고 생각하면 된다.
		value = (*(pbyBuffer+3))<<24 | (*(pbyBuffer+2))<<16 | (*(pbyBuffer+1))<<8 | (*(pbyBuffer)) ;
	}

	return value ;
}
void FillTextureWithBuffer(LPDIRECT3DTEXTURE9 pTex, BYTE *pbyColor, int nColorDepth, int nMapLevel)
{
	D3DSURFACE_DESC desc ;
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC)) ;
	pTex->GetLevelDesc(nMapLevel, &desc) ;

	DWORD *pdwColor ;
	D3DLOCKED_RECT d3drect ;
	pTex->LockRect(nMapLevel, &d3drect, NULL, D3DLOCK_NO_DIRTY_UPDATE) ;
	pTex->AddDirtyRect(NULL) ;
	pdwColor = (DWORD *)d3drect.pBits ;
	int nDepthSize = nColorDepth/8 ;

	for(int y=0; y<(int)desc.Height; y++)
	{
		for(int x=0; x<(int)desc.Width; x++, pbyColor += nDepthSize, pdwColor++)
		{
			*pdwColor = GetColorfromBuffer(pbyColor, nDepthSize, desc.Format) ;
		}
	}
	pTex->UnlockRect(nMapLevel) ;
}
void FillAlphaTexture(LPDIRECT3DTEXTURE9 pTex, BYTE *pbyColor)
{
	D3DSURFACE_DESC desc ;
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC)) ;
	pTex->GetLevelDesc(0, &desc) ;

	BYTE *pbyDest ;
	D3DLOCKED_RECT d3drect ;
	pTex->LockRect(0, &d3drect, NULL, D3DLOCK_NO_DIRTY_UPDATE) ;
	pTex->AddDirtyRect(NULL) ;
	pbyDest = (BYTE *)d3drect.pBits ;

	for(int y=0; y<(int)desc.Height; y++)
	{
		for(int x=0; x<(int)desc.Width; x++, pbyColor++, pbyDest++)
			(*pbyDest) = (*pbyColor) ;
	}
	pTex->UnlockRect(0) ;
}
void FillSurfaceWithColor(LPDIRECT3DSURFACE9 pSurf, DWORD dwColor)
{
	D3DSURFACE_DESC desc ;
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC)) ;
	pSurf->GetDesc(&desc) ;

	D3DLOCKED_RECT d3drect ;
	pSurf->LockRect(&d3drect, NULL, D3DLOCK_NO_DIRTY_UPDATE) ;
	DWORD *pdw = (DWORD *)d3drect.pBits ;

	for(int y=0; y<(int)desc.Height; y++)
	{
		for(int x=0; x<(int)desc.Width; x++, pdw++)
			*pdw = dwColor ;
	}
	pSurf->UnlockRect() ;
}
