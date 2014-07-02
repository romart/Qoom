#include "r_common.h"
#include "r_file.h"
#include "..\framework\log.h"

Render render;

extern Logger logger;

//static LPDIRECT3DVERTEXSHADER9 d_pVShader = NULL;
//static LPDIRECT3DPIXELSHADER9 d_pPShader = NULL;


void Render::SetPixelShader( int psIndex )
{
	pPShader = pShaders[ psIndex ].ptr;
}

void Render::SetVertexShader( int vsIndex )
{
	pVShader = vShaders[ vsIndex ].ptr;
}

Render::Render( void )
{
	this->g_pd3dDevice = NULL;
	this->g_pd3d = NULL;
	this->modelCounter = 0;
	this->surfaceCounter = 0;
	this->g_pVD = NULL;
	this->models.clear();
	this->surfaces.clear();
	this->fovY = 0.0f;
	this->zFar = 0.0f;
	this->zNear = 0.0f;
	this->vShaderConst = NULL;
	this->pShaderConst = NULL;
	this->numVSConsts = 0;
	this->numPSConsts = 0;
	vShaders.clear();
	pShaders.clear();

	texManager.clear();
	ZeroMemory( &projection, sizeof( projection ) );
	projection._11 = projection._22 = projection._33 = projection._44 = 1.0f;
	maxModelIndex = -1;
	maxSurfaceIndex = -1;
	maxVertexIndex = -1;
	maxPixelIndex = -1;
}

Render::~Render( void )
{
	models.clear();
	surfaces.clear();

	if( g_pd3dDevice != NULL )
		g_pd3dDevice->Release();
	g_pd3dDevice = NULL;
	if( g_pd3d != NULL )
		g_pd3d->Release();
	g_pd3d = NULL;
}

D3DVERTEXELEMENT9 declaration[] =
{
	{0, 0,	D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12,	D3DDECLTYPE_FLOAT2,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{0, 20,	D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 32,	D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
	{0, 44,	D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
	D3DDECL_END()
};


int Render::Init( MainGameWindow & wnd, float zNear, float zFar, float fovY  )
{
	g_pd3d = Direct3DCreate9( D3D_SDK_VERSION );
	if ( NULL == g_pd3d ) {
		logger.FatalError( "Render::Init: Direct3DCreate9 - error" );
		exit( RENDER_ERROR_INIT );
	}

	this->width = wnd.width;
	this->height = wnd.height;
	this->zNear  = zNear;
	this->zFar = zFar;
	this->fovY = fovY;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	
	

	HRESULT hr = g_pd3d->CreateDevice( D3DADAPTER_DEFAULT, 
						 D3DDEVTYPE_HAL, 
						 wnd.hWnd,
						 D3DCREATE_HARDWARE_VERTEXPROCESSING,
						 &d3dpp, 
						 &g_pd3dDevice 
					   );

	if ( FAILED( hr ) ) {
		logger.FatalError( "Render::Init: CreateDevice error" );
		exit( RENDER_ERROR_INIT );
	}

	hr = g_pd3dDevice->CreateVertexDeclaration( declaration, &g_pVD );
	if ( FAILED( hr ) ) {
		logger.FatalError( "Render::Init: Cannot create vertex declaration" );
		exit( RENDER_ERROR_INIT );
	}

	g_pd3dDevice->CreateVertexDeclaration( declaration, &g_pVD );

	if ( CreateVertexShader( "vertex.vsh" ) < 0 ) {
		logger.FatalError( "Render::Init: Cannot create default vertex shared" );
		exit( RENDER_ERROR_INIT );
	}
	if ( CreatePixelShader( "pixel.psh" ) < 0 ) {
		logger.FatalError( "Render::Init: Cannot create default pixel shared" );
		exit( RENDER_ERROR_INIT );
	}

	SetVertexShader();
	SetPixelShader();

	logger.Info( "Render::Init: created vertex declaration" );
	logger.Info( "Render::Init: CreateDevice - succesed" );
	return INIT_OK;

}


int Render::RegisterModel( const Model & model )
{
	rd3dModel_t d_model;
	memset( &d_model, 0, sizeof( d_model ) );
	d_model.numOfserfaces = model.GetNumOfSurfaces();

	for( int i = 0; i < model.GetNumOfSurfaces(); ++i ) {

		HRESULT hr = g_pd3dDevice->CreateVertexBuffer( model[i].numOfvertexes * sizeof( vertex_t ), 0, 0, D3DPOOL_DEFAULT, &d_model.surfaces[ i ].v_buffer, NULL );
		if ( FAILED( hr ) ) {
			logger.Error( "Render::RegisterModel: cannot create vertex buffer" );
			return RENDER_ERROR_REG;
		}

		VOID* pVertices;
		if( FAILED( d_model.surfaces[ i ].v_buffer->Lock( 0, model[i].numOfvertexes * sizeof( vertex_t ), ( void** )&pVertices, 0 ) ) ) {
			logger.Error( "Render::RegisterModel: cannot lock vertex buffer" );
			return RENDER_ERROR_REG;
		}
		memcpy( pVertices, model[ i ].vertexes, model[ i ].numOfvertexes * sizeof( vertex_t ) );
		d_model.surfaces[ i ].v_buffer->Unlock();
		d_model.surfaces[ i ].numOfvertex = model[i].numOfvertexes;


		hr = g_pd3dDevice->CreateIndexBuffer( model[ i ].numOfindexes * sizeof( WORD ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &d_model.surfaces[ i ].i_buffer, NULL );

		if( FAILED( hr ) ) {
			logger.Error( "Render::RegisterModel: cannot create index buffer" );
			return RENDER_ERROR_REG;
		}
	
		VOID* pIndices;
		hr = d_model.surfaces[ i ].i_buffer->Lock( 0, model[ i ].numOfindexes * sizeof( WORD ), ( void** )&pIndices, 0 );
		if( FAILED( hr ) ) {
			logger.Error( "Render::RegisterModel: cannot lock index buffer" );
			return RENDER_ERROR_REG;
		}
		memcpy( pIndices, model[ i ].indexes, model[ i ].numOfindexes * sizeof( WORD ) );
		d_model.surfaces[ i ].i_buffer->Unlock();

		d_model.surfaces[ i ].numOfindex = model[ i ].numOfindexes;
		d_model.surfaces[ i ].numOftrian = model[ i ].numOfTriangles;
		
		d_model.surfaces[ i ].texture = RegisterTexture( model[ i ].texfilename );

		d_model.surfaces[ i ].pVShaderIndex = CreateVertexShader( model[ i ].VShaderName );
		d_model.surfaces[ i ].pPShaderIndex = CreatePixelShader( model[ i ].PShaderName );

		logger.Info( "Render::RegisterModel: register surfaces[ %d ] with %d vertexes, %d indexes and %d triangles", i, d_model.surfaces[ i ].numOfvertex, d_model.surfaces[ i ].numOfindex, d_model.surfaces[ i ].numOftrian );
	}
	logger.Info( "Render::RegisterModel: register model[ %d ] with %d surfaces", modelCounter, d_model.numOfserfaces );

	models.push_back( d_model );
	maxModelIndex = modelCounter;



	return modelCounter++;

}

LPDIRECT3DTEXTURE9 Render::RegisterTexture( const char * texName )
{
	std::map< std::string, LPDIRECT3DTEXTURE9 >::const_iterator ifind = texManager.find( texName );

	if ( ifind != texManager.end() ) {
		return ( *ifind ).second;
	} else {
		LPDIRECT3DTEXTURE9 tmp;
		HRESULT hr = D3DXCreateTextureFromFile( g_pd3dDevice, texName, &tmp );
		if ( FAILED( hr ) ) {
			logger.Error( "Render::RegisterTexture: cannot load texture from %s", texName );
			return NULL;
		}
		texManager[ texName ] = tmp;
		return tmp;
	}

}

int Render::RegisterSurface( const surface_t & surf )
{

	HRESULT hr;
	rd3dSurface_t ret;

	memset( &ret, 0, sizeof( ret ) );

	hr = g_pd3dDevice->CreateVertexBuffer( surf.numOfvertexes * sizeof( vertex_t ), 0, 0, D3DPOOL_DEFAULT, &ret.v_buffer, NULL );

	if ( FAILED( hr ) ) {
		logger.Error( "Render::RegisterSurface: cannot create vertex buffer" );
		return RENDER_ERROR_REG;
	}

	VOID* pVertices;
	if( FAILED( ret.v_buffer->Lock( 0, surf.numOfvertexes * sizeof( vertex_t ), ( void** )&pVertices, 0 ) ) ) {
		logger.Error( "Render::RegisterSurface: cannot lock vertex buffer" );
		return RENDER_ERROR_REG;
	}
	memcpy( pVertices, surf.vertexes, surf.numOfvertexes * sizeof( vertex_t ) );
	ret.v_buffer->Unlock();
	ret.numOfvertex = surf.numOfvertexes;

	hr = g_pd3dDevice->CreateIndexBuffer( surf.numOfindexes * sizeof( WORD ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ret.i_buffer, NULL );

	if( FAILED( hr ) ) {
		logger.Error( "Render::RegisterSurface: cannot create index buffer" );
		return RENDER_ERROR_REG;
	}

	VOID* pIndices;
	hr = ret.i_buffer->Lock( 0,surf.numOfindexes * sizeof( WORD ), ( void** )&pIndices, 0 );
	if( FAILED( hr ) ) {
		logger.Error( "Render::RegisterSurface: cannot lock index buffer" );
		return RENDER_ERROR_REG;
	}
	memcpy( pIndices, surf.indexes, surf.numOfindexes * sizeof( WORD ) );
	ret.i_buffer->Unlock();

	ret.numOfindex = surf.numOfindexes;
	ret.numOftrian = surf.numOfTriangles;

	if ( surf.texfilename ) {
		ret.texture = RegisterTexture( surf.texfilename );
	}


	ret.pVShaderIndex = surf.VShaderName ? CreateVertexShader( surf.VShaderName ) : 0;
	ret.pPShaderIndex = surf.PShaderName ? CreatePixelShader( surf.PShaderName ) : 0;


	logger.Info( "Render::RegisterSurface: register surfaces[ %d ] with %d vertexes, %d indexes and %d triangles", surfaceCounter, ret.numOfvertex, ret.numOfindex, ret.numOftrian );
	surfaces.push_back( ret );
	maxSurfaceIndex = surfaceCounter;
	return surfaceCounter++;
}

void Render::BeginDraw( void )
{
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	g_pd3dDevice->BeginScene();
}

void Render::EndDraw( void )
{
	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

int Render::DrawIndexModel( int index, const D3DXMATRIX & matrix )
{
	if ( index > maxModelIndex ) {
		logger.Warning( "Render::DrawIndexModel: wrong index = %d, too large", index );
		index = maxModelIndex;
	}

	if ( index < 0 ) {
		logger.Warning( "Render::DrawIndexModel: wrong index = %d < 0", index );
		index  = 0;
	}

	rd3dModel_t & model = models[ index ];
	
	for( int i = 0; i < model.numOfserfaces; ++i ) {
		if ( model.surfaces[ i ].texture ) {
			DrawSurface( model.surfaces[ i ], matrix );
		}
	}



	return 0;
}


int Render::DrawSurface( const rd3dSurface_t & surf, const D3DXMATRIX & matrix ) 
{

	HRESULT hr;
	SetVertexShader( surf.pVShaderIndex );
	SetPixelShader( surf.pPShaderIndex );

	D3DXMATRIX mat;

	D3DXMatrixMultiplyTranspose( &mat, &matrix, &projection );

	hr = g_pd3dDevice->SetVertexShader( pVShader );
	hr = g_pd3dDevice->SetVertexDeclaration( g_pVD);
	hr = g_pd3dDevice->SetVertexShaderConstantF( 0, (float*)&mat, 4 );
	hr = SetVertexShaderConstLocal();
	hr = g_pd3dDevice->SetPixelShader( pPShader );
	hr = SetPixelShaderConstLocal();
	hr = g_pd3dDevice->SetTexture( 0, surf.texture );
	hr = g_pd3dDevice->SetStreamSource( 0, surf.v_buffer, 0, sizeof( vertex_t ) );
	hr = g_pd3dDevice->SetIndices( surf.i_buffer );
	hr = g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, surf.numOfindex, 0, surf.numOftrian );

	return 0;
}

int Render::DrawIndexSurface( int index, const D3DXMATRIX & matrix )
{
	if ( index > maxSurfaceIndex ) {
		logger.Warning( "Render::DrawIndexSurface: wrong index = %d, too large", index );
		index = maxModelIndex;
	}

	if ( index < 0 ) {
		logger.Warning( "Render::DrawIndexSurface: wrong index = %d < 0", index );
		index  = 0;
	}

	rd3dSurface_t & surf = surfaces[ index ];

	DrawSurface( surf, matrix );

	return 0;
}

void Render::Set2D( void )
{
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	D3DXMatrixOrthoLH( &projection, (float)width, (float)height, zNear, zFar );
}


void Render::Set3D( void )
{
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );	
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	D3DXMatrixPerspectiveFovLH( &projection, fovY, (float)width / (float)height, zNear, zFar );
}

HRESULT Render::SetVertexShaderConstLocal( void )
{
	numVSConsts = numVSConsts < 32 ? numVSConsts : 32;
	if ( vShaderConst != NULL ) {
		return g_pd3dDevice->SetVertexShaderConstantF( 4, (float*)&vShaderConst[ 0 ][ 0 ], numVSConsts );
	}
//	logger.Error( "Render::SetVertexShaderConstLocal: cannot set vertex consts" );
	return -1;
}

HRESULT Render::SetPixelShaderConstLocal( void )
{
	numPSConsts = numPSConsts < 32 ? numPSConsts : 32;
	if ( pShaderConst != NULL ) {
		return g_pd3dDevice->SetPixelShaderConstantF( 0, (float*)&pShaderConst[ 0 ][ 0 ], numPSConsts );
	}
//	logger.Error( "Render::SetPixelShaderConstLocal: cannot set pixel consts" );
	return -1;
}

int Render::CreateVertexShader( const char * fileName )
{
	if ( fileName == NULL ) return 0;
	for( int i = 0; i < (int)vShaders.size(); ++i ) {
		if ( vShaders[ i ].name == fileName ) return i;
	}
	
	vShaderTag_t ret;
	LPD3DXBUFFER code = NULL;
	LPD3DXBUFFER error = NULL;
	ret.name = fileName;
	g_pd3dDevice->CreateVertexDeclaration( declaration, &g_pVD );
	HRESULT hr = D3DXAssembleShaderFromFile( fileName, 0, 0 , 0, &code, &error );
	if ( FAILED( hr ) ) {
		if ( error != NULL ) {
			char * err = ( char* )error->GetBufferPointer();
			logger.Error( "D3D Error: %s", err );
			error->Release();
		} else {
			logger.Error( "Couldn't load Vertex shader from %s", fileName );
		}
		return -1;
	}

	g_pd3dDevice->CreateVertexShader( ( DWORD* )code->GetBufferPointer(), &ret.ptr );
	code->Release();

	vShaders.push_back( ret );
	return ++maxVertexIndex;

}

int Render::CreatePixelShader( const char * fileName )
{
	if ( fileName == NULL ) return 0;
	for( int i = 0; i < (int)pShaders.size(); ++i ) {
		if ( pShaders[ i ].name == fileName ) return i;
	}
	
	pShaderTag_t ret;
	LPD3DXBUFFER code = NULL;
	LPD3DXBUFFER error = NULL;
	ret.name = fileName;
	HRESULT hr = D3DXAssembleShaderFromFile( fileName, 0, 0 , 0, &code, &error );
	if ( FAILED( hr ) ) {
		if ( error != NULL ) {
			char * err = ( char* )error->GetBufferPointer();
			logger.Error( "D3D Error: %s", err );
			error->Release();
		} else {
			logger.Error( "Couldn't load Pixel shader from %s", fileName );
		}
		return -1;
	}

	g_pd3dDevice->CreatePixelShader( ( DWORD* )code->GetBufferPointer(), &ret.ptr );
	code->Release();

	pShaders.push_back( ret );
	return ++maxPixelIndex;

}