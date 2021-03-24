/*******************************
 *   @file		D3D9Environment.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.01
 *   @note		SecretEngine for Direct3D9
 ********************************/
#pragma once

#include "D3D9Def.h"

class CD3D9Environment
{
private :
    LPDIRECT3D9           m_pd3d ;					//The main D3D object
	LPDIRECT3DDEVICE9     m_pd3dDevice ;			//The D3D Rendering device
	D3DCAPS9              m_d3dCaps ;
	D3DSURFACE_DESC       m_d3dsBackBuffer ;

	D3DPRESENT_PARAMETERS m_d3dpp ;

public :
	HWND                  m_hwnd ;
	UINT m_lScreenWidth, m_lScreenHeight ;
	UINT m_lRefreshRateInHz ;
	D3DMULTISAMPLE_TYPE m_MultisampleType;

	float m_fFov ;
	float m_fNear, m_fFar ;

private :
    
public :
	CD3D9Environment() ;
	~CD3D9Environment() ;

	HRESULT initialize(HWND hwnd, UINT lWidth, UINT lHeight, DWORD dwAddBehaviorFlag=0, BOOL bWindowed=TRUE) ;

	HRESULT outputError(char *pszMessage) ;
	LPDIRECT3DDEVICE9 getD3DDevice() {return m_pd3dDevice;} ;
	LPDIRECT3D9 getDirect3d() {return m_pd3d;} ;


	D3DPRESENT_PARAMETERS *getPresentParameters() ;

	void cleanup() ;
} ;
