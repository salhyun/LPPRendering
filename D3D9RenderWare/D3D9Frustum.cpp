#include "D3D9Frustum.h"
#include "D3D9Shader.h"

CD3D9Frustum::CD3D9Frustum()
{
	m_pd3dDevice = NULL ;
	m_bDrawPlane = false ;
}
CD3D9Frustum::~CD3D9Frustum()
{
}
void CD3D9Frustum::initialize(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice ;
}
void CD3D9Frustum::buildFrustum(D3DXMATRIX *pmatV, D3DXMATRIX *pmatP)
{
	Matrix4 matV, matP ;
	COPY_MATRIX4(matV, (*pmatV)) ;
	COPY_MATRIX4(matP, (*pmatP)) ;
	CSecretFrustum::buildFrustum(&matV, &matP) ;
}
void CD3D9Frustum::render(Matrix4 *pmat, CSecretShader *pcShader, int nPass, DWORD dwColor)
{
	Matrix4 matWorld ;
	CD3D9Shader *pcd3dShader = (CD3D9Shader *)pcShader ;

	pcd3dShader->setTechnique() ;
	pcd3dShader->begin() ;
	pcd3dShader->beginPass(nPass) ;

	matWorld = (*pmat) ;

	CRenderingIngredient *pcIngredient = pcd3dShader->getIngredient() ;
	if(pcIngredient)
	{
		D3DXMATRIX matResult ;
		COPY_D3DXMATRIX(matResult, matWorld) ;
		pcIngredient->m_matWorld = matResult ;
	}
	pcd3dShader->setParametersToShader(NULL) ;
	pcd3dShader->m_pEffect->CommitChanges() ;

	render(dwColor) ;
	
	pcd3dShader->endPass() ;
	pcd3dShader->end() ;
}
void CD3D9Frustum::render(DWORD dwColor)
{
	//삼각형을 그릴때는 시계방향으로 그린다.
	unsigned short index[] =
	{
		//bottom
		0, 1, 2,
		0, 2, 3,
        //top
		4, 7, 6,
		4, 6, 5,
        //right
		1, 5, 6,
		1, 6, 2,
        //left
		0, 3, 7,
		0, 7, 4,
        //near
		0, 4, 5,
		0, 5, 1,
        //far
		//3, 7, 6,
		//3, 6, 2
		3, 2, 6,
		3, 6, 7
	} ;

	unsigned short index_edge[] = 
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		0, 4,
		3, 7,

		2, 6,
		1, 5
	} ;

	typedef struct
	{
		D3DXVECTOR3 pos ;
		unsigned long color ;
	} VERTEX ;

	int i ;
	VERTEX vertex[8] ;

    for(i=0 ; i<8 ; i++)
	{
		vertex[i].pos = D3DXVECTOR3(m_avVertices[i].x, m_avVertices[i].y, m_avVertices[i].z) ;
		vertex[i].color = 0x80ffffff ;
	}

    DWORD dwAlphaBlendEnable ;
	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable) ;

	DWORD dwTssColorOp01, dwTssColorOp02 ;
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwTssColorOp01) ;
	m_pd3dDevice->GetTextureStageState(1, D3DTSS_COLOROP, &dwTssColorOp02) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ;

	m_pd3dDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE) ;
	m_pd3dDevice->SetStreamSource(0, NULL, 0, sizeof(VERTEX)) ;
	m_pd3dDevice->SetTexture(0, NULL) ;
	m_pd3dDevice->SetIndices(0) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE) ;
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE) ;
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ;
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;

	unsigned short *pIndex=index ;

	m_bDrawPlane = true ;
	if(m_bDrawPlane)
	{
		for(i=0 ; i<8 ; i++)
			vertex[i].color = 0x8000ff00 ;

		m_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, pIndex, D3DFMT_INDEX16, vertex, sizeof(vertex[0])) ;

		for(i=0 ; i<8 ; i++)
			vertex[i].color = 0x80ff0000 ;// 0xfff0f0f ;

		m_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, pIndex+12, D3DFMT_INDEX16, vertex, sizeof(vertex[0])) ;

		for(i=0 ; i<8 ; i++)
			vertex[i].color = 0x800000ff ;

		m_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, pIndex+24, D3DFMT_INDEX16, vertex, sizeof(vertex[0])) ;
	}
	else
	{
		for(i=0 ; i<8 ; i++)
			vertex[i].color = dwColor ;

		m_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, index_edge, D3DFMT_INDEX16, vertex, sizeof(vertex[0])) ;
	}

	m_cLocalSystem.initialize(m_pd3dDevice) ;

	D3DXMATRIX matV ;
	D3DXMATRIX matIV ;	
	COPY_D3DXMATRIX(matV, m_matV) ;
	D3DXMatrixInverse(&matIV, NULL, &matV) ;
	m_cLocalSystem.setMatrix(&matIV) ;
	m_cLocalSystem.render() ;

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwTssColorOp01) ;
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, dwTssColorOp02) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable) ;
}