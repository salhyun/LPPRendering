#include "D3D9RigidBumpMesh.h"
#include "D3D9Material.h"
#include "D3D9TextureContainer.h"

D3DVERTEXELEMENT9 decl_RigidBump[] =
{
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
	{0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
} ;

CD3D9RigidBumpMesh::CD3D9RigidBumpMesh()
{
	m_pcShader = NULL ;
	m_pVB = NULL ;
	m_pIB = NULL ;
	m_pDecl = NULL ;
}
CD3D9RigidBumpMesh::~CD3D9RigidBumpMesh()
{
	release() ;
}
void CD3D9RigidBumpMesh::release()
{
	SAFE_RELEASE(m_pDecl) ;

	SAFE_RELEASE(m_pVB) ;
	SAFE_RELEASE(m_pIB) ;

}
HRESULT CD3D9RigidBumpMesh::initialize(LPDIRECT3DDEVICE9 pd3dDevice, SMesh *psMesh)
{
	HRESULT hr ;
	CSecretRigidBumpMesh::initialize(psMesh) ;

	m_pd3dDevice = pd3dDevice ;

	hr = m_pd3dDevice->CreateVertexDeclaration(decl_RigidBump, &m_pDecl) ;

	RETURN_D3D9FAILED(hr);

	hr = m_pd3dDevice->CreateVertexBuffer(m_nNumVertex*sizeof(SVertex), 0, NULL, D3DPOOL_MANAGED, &m_pVB, NULL) ;
	RETURN_D3D9FAILED(hr);

	SVertex *psVertices=NULL ;
	hr = m_pVB->Lock(0, m_nNumVertex*sizeof(SVertex), (void **)&psVertices, 0) ;
	RETURN_D3D9FAILED(hr);

	for(int i=0; i<m_nNumVertex; i++)
	{
		COPY_VECTOR3(psVertices[i].pos, m_sVertexBuffer.pos[i]) ;
		COPY_VECTOR3(psVertices[i].normal, m_sVertexBuffer.normal[i]) ;
		COPY_VECTOR3(psVertices[i].tangent, m_sVertexBuffer.tangent[i]) ;
		COPY_VECTOR2(psVertices[i].tex, m_sVertexBuffer.tex[i]) ;

		psVertices[i].tex.y = 1.0f - psVertices[i].tex.y ;
	}
	m_pVB->Unlock() ;

	hr = m_pd3dDevice->CreateIndexBuffer(m_nNumIndex*sizeof(SIndex), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL) ;
	RETURN_D3D9FAILED(hr);

	SIndex *psIndices=NULL ;
	hr = m_pIB->Lock(0, m_nNumIndex*sizeof(SIndex), (void **)&psIndices, NULL) ;
	RETURN_D3D9FAILED(hr);

	for(int i=0; i<m_nNumIndex; i++)
	{
		psIndices[i].index[0] = m_psIndices[i].index[0] ;
		psIndices[i].index[1] = m_psIndices[i].index[1] ;
		psIndices[i].index[2] = m_psIndices[i].index[2] ;
	}
	m_pIB->Unlock() ;

	return S_OK ;
}
void CD3D9RigidBumpMesh::render()
{
	m_pd3dDevice->SetVertexDeclaration(m_pDecl) ;
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex)) ;
	m_pd3dDevice->SetIndices(m_pIB) ;
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumVertex, 0, m_nNumIndex) ;
}
void CD3D9RigidBumpMesh::setMaterial(CD3D9Material *pcMaterial, CD3D9TextureContainer *pcTexContainer)
{
	int nAttr = pcMaterial->getAttr() ;
	if(nAttr & CD3D9Material::MATERIAL_ATTR_DIFFUSE)
	{
		pcMaterial->m_pTexDiffuse = pcTexContainer->findD3DTexture(pcMaterial->m_szDiffuseMap) ;
	}
	if(nAttr & CD3D9Material::MATERIAL_ATTR_NORMAL)
	{
		pcMaterial->m_pTexNormal = pcTexContainer->findD3DTexture(pcMaterial->m_szNormalMap) ;
	}

	m_pcMaterial = (CSecretMaterial *)pcMaterial ;
}
CD3D9Material *CD3D9RigidBumpMesh::getD3D9Material()
{
	return (CD3D9Material *)m_pcMaterial ;
}
