#ifndef __R_COMMON_H__
#define __R_COMMON_H__

#include "..\sys\sys_main_header.h"
#include "..\sys\sys_window.h"
#include "..\global.h"

#include "..\math\Vector.h"

#include "r_global.h"

typedef struct {
	std::string name;
	LPDIRECT3DVERTEXSHADER9 ptr;
} vShaderTag_t;

typedef struct {
	std::string name;
	LPDIRECT3DPIXELSHADER9 ptr;
} pShaderTag_t;


class Model;

class Render
{
public:
	Render( void );
	~Render( void );


	int Init( MainGameWindow & wnd, float zNear, float zFar, float fovY );

	int GetScreenWidth( void ) const;
	int GetScreenHeight( void ) const;

	int RegisterModel( const Model & model );
	int RegisterSurface( const surface_t & surf );


	void BeginDraw( void );
	int DrawIndexModel( int index, const D3DXMATRIX & matrix );
	int DrawIndexSurface( int index, const D3DXMATRIX & matrix );
	void EndDraw( void );

	void SetPixelShader( int psIndex = 0 );
	void SetVertexShader( int vsIndex = 0 );

	void SetVertexConstArray( const vec4_t * arr = NULL, int numConsts = 0 ) { numVSConsts = numConsts; vShaderConst = arr; }
	void SetPixelConstArray( const vec4_t * arr = NULL, int numConsts = 0 ) { numPSConsts = numConsts; pShaderConst = arr; }

	void Set2D( void );
	void Set3D( void );

	void SetRenderState( D3DRENDERSTATETYPE state, DWORD value ) { g_pd3dDevice->SetRenderState( state, value ); }

	float GetZNear( void ) const { return zNear; }
	float GetZFar( void ) const { return zFar; }
	float GetFovY( void ) const { return fovY; }

	int CreateVertexShader( const char * fileName );
	int CreatePixelShader( const char * fileName );

private:
	int DrawSurface( const rd3dSurface_t & surf, const D3DXMATRIX & matrix );
	LPDIRECT3DTEXTURE9 RegisterTexture( const char * texName );
	HRESULT SetVertexShaderConstLocal( void );
	HRESULT SetPixelShaderConstLocal( void );

private:

	LPDIRECT3D9						g_pd3d;
	LPDIRECT3DDEVICE9				g_pd3dDevice;
	LPDIRECT3DVERTEXDECLARATION9	g_pVD;

	LPDIRECT3DVERTEXSHADER9			pVShader;
	LPDIRECT3DPIXELSHADER9			pPShader;
	
	int width;
	int height;

	float zNear;
	float zFar;

	float fovY;

	D3DXMATRIX	projection;

	std::map< std::string, LPDIRECT3DTEXTURE9 > texManager;

	int modelCounter;
	int surfaceCounter;

	int maxModelIndex;
	int maxSurfaceIndex;
	int maxVertexIndex;
	int maxPixelIndex;

	const vec4_t	* vShaderConst;
	const vec4_t	* pShaderConst;

	int numVSConsts;
	int numPSConsts;

	std::vector< rd3dModel_t >		models;
	std::vector< rd3dSurface_t >	surfaces;
	std::vector< vShaderTag_t >	vShaders;
	std::vector< pShaderTag_t >	pShaders;
};


 __forceinline int Render::GetScreenWidth( void ) const
 { 
	 return width;
 }

 __forceinline int Render::GetScreenHeight( void ) const
 { 
	 return height;
 }

#endif