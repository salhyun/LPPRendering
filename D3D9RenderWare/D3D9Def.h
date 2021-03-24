/*******************************
 *   @file		D3D9Def.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.01
 *   @note		SecretEngine for Direct3D9
 ********************************/
#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if(p){p->Release(); p=NULL;}}
#endif

#ifndef RETURN_D3D9FAILED
#define RETURN_D3D9FAILED(hr) if(FAILED(hr)){TRACE("D3D_ERROR code=%d\r\n", hr); return hr;}
#endif

#ifndef COPY_VECTOR3
#define COPY_VECTOR3(out, in) {out.x=in.x; out.y=in.y; out.z=in.z;}
#endif

#ifndef COPY_VECTOR2
#define COPY_VECTOR2(out, in) {out.x=in.x; out.y=in.y;}
#endif

#ifndef COPY_D3DXMATRIX
#define COPY_D3DXMATRIX(out, in) {out._11=in.m11; out._12=in.m12; out._13=in.m13; out._14=in.m14; \
								  out._21=in.m21; out._22=in.m22; out._23=in.m23; out._24=in.m24; \
  								  out._31=in.m31; out._32=in.m32; out._33=in.m33; out._34=in.m34; \
								  out._41=in.m41; out._42=in.m42; out._43=in.m43; out._44=in.m44;}
#endif
#ifndef COPY_MATRIX4
#define COPY_MATRIX4(out, in) {out.m11=in._11; out.m12=in._12; out.m13=in._13; out.m14=in._14; \
							   out.m21=in._21; out.m22=in._22; out.m23=in._23; out.m24=in._24; \
  							   out.m31=in._31; out.m32=in._32; out.m33=in._33; out.m34=in._34; \
							   out.m41=in._41; out.m42=in._42; out.m43=in._43; out.m44=in._44;}
#endif

#ifndef PROJECT_VECTOR3
#define PROJECT_VECTOR3(out, in) {out.x=in.x/in.w; out.y=in.y/in.w; out.z=in.z/in.w;}
#endif

#ifndef COLOR_TO_VECTOR3
#define COLOR_TO_VECTOR3(out, r, g, b) {out.x=(float)r/255.0f; out.y=(float)g/255.0f; out.z=(float)b/255.0f;}
#endif

bool CopyTexture(LPDIRECT3DTEXTURE9 pTexSrc, LPDIRECT3DTEXTURE9 pTexDest, LPDIRECT3DDEVICE9 pd3dDevice) ;//¹Ó¸ÊÀÌ ¾ø´Â°æ¿ì »ç¿ë.
void FillTextureWithColor(LPDIRECT3DTEXTURE9 pTex, DWORD dwColor, int nMapLevel=0) ;
void FillTextureWithColorArray(LPDIRECT3DTEXTURE9 pTex, DWORD *pdwSrcColor, int nMapLevel=0) ;
void FillTextureWithBuffer(LPDIRECT3DTEXTURE9 pTex, BYTE *pbyColor, int nColorDepth, int nMapLevel=0) ;
void FillAlphaTexture(LPDIRECT3DTEXTURE9 pTex, BYTE *pbyColor) ;
void FillTexture32WithBuffer(LPDIRECT3DTEXTURE9 pTex, DWORD *pdwSrcColor, int nColorDepth, int nMapLevel=0) ;
void FillSurfaceWithColor(LPDIRECT3DSURFACE9 pSurf, DWORD dwColor) ;

struct SCVertex
{
	D3DXVECTOR3 pos ;
	unsigned long color ;

    void set(D3DXVECTOR3 &p, DWORD dwColor) ;
} ;
