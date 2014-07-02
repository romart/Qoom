#include "level.h"
#include "Player.h"

#include "..\framework\log.h"
#include "..\render\r_common.h"

extern Logger logger;
extern Render render;

#define BORDER_OFFSET	0.0f

Level::Level( void )
{
	gObject.clear();
	indexes.clear();
	borders.clear();
	player = NULL;
}

void Level::SetPlayer( Player * _player )
{
	player = _player;
}

Level::~Level( void )
{
	gObject.clear();
	indexes.clear();
	borders.clear();
}

int Level::LoadFromFile( const char * name )
{
	return (int)( name - NULL );
}

BYTE Level::GetGeneratorValue( int i ) const
{
	switch( i ) {
	case 1: return WALL;
	default: return ROAD;
	}
}

Vector3 Level::GenerateRandomLevel( int w, int h, float dw, float dh, int random )
{
	srand((int)random);
	
	BYTE * map = new BYTE[ w * h ];

	char c = '\n';

	File::Remove( "level.test" );
	File file;
	file.Open( "level.test", File::F_CREATE | File::F_WRITE_ONLY );

	for( int j = 0; j < h; ++j ) {
		for( int i = 0; i < w; ++i ) {
			map[ j * w + i ] = GetGeneratorValue( rand() % 3 );
			file.Write( &map[ j * w + i ], sizeof( map[ 0 ] ) );
		}
		file.Write( ( BYTE * )&c, sizeof( c ) );
	}
	map[ 1 * w + w / 2 ] = PLAYER;
	file.Lseek( 1 * w + w / 2 , File::F_SEEK_SET );
	file.Write( &map[ 1 * w + w / 2 ], sizeof( map[ 0 ] ) );
	file.Close();

	Vector3 ret( ( w / 2 ) * dw + dw / 2, 0.0f, 1 * dh + dh / 2 );

	for( int j = 0; j < h; ++j ) {
			CreateWallX( 
						Vector3( 0 * dw, 0.0f, j * dh ),
						Vector3( 0 * dw, dw, ( j + 1 ) * dh ),
						Vector3( 1.0f, 0.0f, 0.0f ),
						Vector2( 0.0f, 0.0f ),
						Vector2( 1.0f, 1.0f ),
						"textures\\brick_wall.jpg"
						);
			CreateWallX( 
						Vector3( w * dw, 0.0f, j * dh ),
						Vector3( w * dw, dw, ( j + 1 ) * dh ),
						Vector3( -1.0f, 0.0f, 0.0f ),
						Vector2( 0.0f, 0.0f ),
						Vector2( 1.0f, 1.0f ),
						"textures\\brick_wall.jpg"
						);
	}

	for( int i = 0; i < w; ++i ) {
		CreateWallZ( 
				Vector3( i * dw, 0.0f, 0 * dh ),
				Vector3( ( i + 1 )* dw, dw, 0 * dh ),
				Vector3( 0.0f, 0.0f, 1.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 1.0f, 1.0f ),
				"textures\\brick_wall.jpg"
				);
		CreateWallZ( 
				Vector3( i * dw, 0.0f, h * dh ),
				Vector3( ( i + 1 )* dw, dw, h * dh ),
				Vector3( 0.0f, 0.0f, -1.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 1.0f, 1.0f ),
				"textures\\brick_wall.jpg"
				);
	}
	GameObject obj;
	for( int j = 0; j < h; ++j ) {
		for( int i = 0; i < w; ++i ) {
			switch( map[ j * w + i ] ) {
			case WALL:
				CreateWallZ( 
							Vector3( i * dw, 0.0f, j * dh ),
							Vector3( ( i + 1 )* dw, dw, j * dh ),
							Vector3( 0.0f, 0.0f, -1.0f ),
							Vector2( 0.0f, 0.0f ),
							Vector2( 1.0f, 1.0f ),
							"textures\\brick_wall.jpg",
							false );
				CreateWallZ( 
							Vector3( i * dw, 0.0f, ( j + 1 ) * dh ),
							Vector3( ( i + 1 )* dw, dw, ( j + 1 ) * dh ),
							Vector3( 0.0f, 0.0f, 1.0f ),
							Vector2( 0.0f, 0.0f ),
							Vector2( 1.0f, 1.0f ),
							"textures\\brick_wall.jpg",
							false );
				CreateWallX( 
							Vector3( i * dw, 0.0f, j * dh ),
							Vector3( i * dw, dw, ( j + 1 ) * dh ),
							Vector3( -1.0f, 0.0f, 0.0f ),
							Vector2( 0.0f, 0.0f ),
							Vector2( 1.0f, 1.0f ),
							"textures\\brick_wall.jpg",
							false );
				CreateWallX( 
							Vector3( ( i + 1 ) * dw, 0.0f, j * dh ),
							Vector3( ( i + 1 )* dw, dw, ( j + 1 ) * dh ),
							Vector3( 1.0f, 0.0f, 0.0f ),
							Vector2( 0.0f, 0.0f ),
							Vector2( 1.0f, 1.0f ),
							"textures\\brick_wall.jpg",
							false );
				obj.CreateCMObject( Vector3( i * dw + dw / 2, dw / 2 ,j * dh + dh / 2 ), dw / 2 );
				AddObject( obj );
				obj.CreateCMObject( Vector3( i * dw + dw / 4, dw / 2, j * dh + dh / 4 ), dw / 3.333f );
				AddObject( obj );
				obj.CreateCMObject( Vector3( ( i + 1 ) * dw - dw / 4, dw / 2, j * dh + dh / 4 ), dw / 3.333f );
				AddObject( obj );
				obj.CreateCMObject( Vector3( ( i ) * dw + dw / 4, dw / 2, ( j + 1 ) * dh - dh / 4 ), dw / 3.333f );
				AddObject( obj );
				obj.CreateCMObject( Vector3( ( i + 1 ) * dw - dw / 4, dw / 2, ( j + 1 ) * dh - dh / 4 ), dw / 3.333f );
				AddObject( obj );
				break;
			case ROAD:
			default:
				CreateWallY(
							Vector3( i * dw, 0.0f, j * dh ),
							Vector3( ( i + 1 )* dw, 0.0f, ( j + 1 ) * dh ),
							Vector3( 0.0f, 1.0f, 0.0f ),
							Vector2( 0.0f, 0.0f ),
							Vector2( 10.0f, 10.0f ),
							"textures\\floor.jpg"
						);
				CreateWallY(
							Vector3( i * dw, dw, j * dh ),
							Vector3( ( i + 1 )* dw, dw, ( j + 1 ) * dh ),
							Vector3( 0.0f, -1.0f, 0.0f ),
							Vector2( 0.0f, 0.0f ),
							Vector2( 10.0f, 10.0f ),
							"textures\\roof.jpg"
						);
				break;
			}
		}
	}

	delete[] map;
	return ret;
}

int Level::CreateTestLevel( void )
{
	//left
	CreateWallX(
				Vector3( -50.0f, -50.0f, 0.0f ),
				Vector3( -50.0f, 50.0f, 200.0f ),
				Vector3( 1.0f, 0.0f, 0.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 2.0f, 1.0f ),
				"textures\\brick_wall.jpg"
				);

	

		//right
	CreateWallX(
				Vector3( 50.0f, -50.0f, 0.0f ),
				Vector3( 50.0f, 50.0f, 200.0f ),
				Vector3( -1.0f, 0.0f, 0.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 2.0f, 1.0f ),
				"textures\\brick_wall.jpg"
				);

		//front
	CreateWallZ(
				Vector3( -50.0f, -50.0f, 200.0f ),
				Vector3( 50.0f, 50.0f, 200.0f ),
				Vector3( 0.0f, 0.0f, -1.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 1.0f, 1.0f ),
				"textures\\brick_wall.jpg"
				);

		//back
	CreateWallZ(
				Vector3( -50.0f, -50.0f, 0.0f ),
				Vector3( 50.0f, 50.0f, 0.0f ),
				Vector3( 0.0f, 0.0f, 1.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 1.0f, 1.0f ),
				"textures\\brick_wall.jpg"
				);

		//top
	CreateWallY(
				Vector3( -50.0f, 50.0f, 0.0f ),
				Vector3( 50.0f, 50.0f, 200.0f ),
				Vector3( 0.0f, -1.0f, 0.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 10.0f, 20.0f ),
				"textures\\roof.jpg"
				);
		//bot
	CreateWallY(
				Vector3( -50.0f, -50.0f, 0.0f ),
				Vector3( 50.0f, -50.0f, 200.0f ),
				Vector3( 0.0f, 1.0f, 0.0f ),
				Vector2( 0.0f, 0.0f ),
				Vector2( 10.0f, 20.0f ),
				"textures\\floor.jpg"
				);

	return 0;
}

void Level::DrawLevel( const D3DXMATRIX & mat )
{
	vec4_t v1[ 2 ];
	Vector3 view = player->GetCamera().GetForward();
	v1[ 0 ][ 0 ] = view.x; v1[ 0 ][ 1 ] = view.y; v1[ 0 ][ 2 ] = view.z; v1[ 0 ][ 3 ] = 1.0f;
	v1[ 1 ][ 0 ] = 0.0f; v1[ 1 ][ 1 ] = 300.0f; v1[ 1 ][ 2 ] = 300.0f; v1[ 1 ][ 3 ] = 0.0f;
	render.SetVertexConstArray( v1, 2 );
	for( int i = 0; i < (int)indexes.size(); ++i ) {
		render.DrawIndexSurface( indexes[ i ], mat );
	}
	render.SetVertexConstArray();
}

void Level::DrawLevelObject( const D3DXMATRIX * mat, const D3DXMATRIX & view )
{
	if ( NULL == mat ) return ;


	for( int i = 0, k = 0; i < (int)gObject.size(); ++i ) {
		if( gObject[ i ].IsRActive() )
			gObject[ i ].DrawObject( mat[ k++ ], view );
	}
}

int Level::CreateWallX( const Vector3 & posMin, const Vector3 & posMax, const Vector3 & n, const Vector2 & texMin, const Vector2 & texMax, const char * texName, bool cm_active )
{
	g_surface gsurf;
	surface_t surf;
	ZeroMemory( &surf, sizeof( surf ) );
	Vector3 center;
	WORD * indx;
#if 1
	WORD ibuf[ 6 ] = { 2, 0, 3, 2, 1, 0 };
	WORD ibufi[ 6 ] = { 0, 1, 2, 3, 0, 2 };
#else
	WORD ibufi[ 6 ] = { 2, 0, 3, 2, 1, 0 };
	WORD ibuf[ 6 ] = { 0, 1, 2, 3, 0, 2 };
#endif
	indx = n.x > 0.0f ? ibuf : ibufi;
	vertex_t vbuf[ 4 ];
	gsurf.isActive = cm_active;
	gsurf.minB.x = min( posMin.x, posMax.x );
	gsurf.minB.y = min( posMin.y, posMax.y );
	gsurf.minB.z = min( posMin.z, posMax.z );

	gsurf.maxB.x = max( posMin.x, posMax.x );
	gsurf.maxB.y = max( posMin.y, posMax.y );
	gsurf.maxB.z = max( posMin.z, posMax.z );

	Vector3 dMin( BORDER_OFFSET, 0.0f, BORDER_OFFSET );

	gsurf.minB += dMin;
	gsurf.maxB -= dMin;

	gsurf.plane.SetNormal( n );
	gsurf.plane.SetDist( ( ( gsurf.minB + gsurf.maxB ) / 2  ) * n );

	vbuf[ 0 ].n = vbuf[ 1 ].n = vbuf[ 2 ].n = vbuf[ 3 ].n = n;

	vbuf[ 0 ].pos = Vector3( posMin.x, posMax.y, posMin.z );
	vbuf[ 1 ].pos = Vector3( posMin.x, posMin.y, posMin.z );
	vbuf[ 2 ].pos = Vector3( posMin.x, posMin.y, posMax.z );
	vbuf[ 3 ].pos = Vector3( posMin.x, posMax.y, posMax.z );

	vbuf[ 0 ].tex = Vector2( texMin.x, texMin.y );
	vbuf[ 1 ].tex = Vector2( texMin.x, texMax.y );
	vbuf[ 2 ].tex = Vector2( texMax.x, texMax.y );
	vbuf[ 3 ].tex = Vector2( texMax.x, texMin.y );

	vbuf[ 0 ].b.x = vbuf[ 0 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 0 ].b.y = vbuf[ 0 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 0 ].b.z = vbuf[ 0 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 0 ].tan.x = vbuf[ 0 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 0 ].tan.y = vbuf[ 0 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 0 ].tan.z = vbuf[ 0 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 1 ].b.x = vbuf[ 1 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 1 ].b.y = vbuf[ 1 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 1 ].b.z = vbuf[ 1 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 1 ].tan.x = vbuf[ 1 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 1 ].tan.y = vbuf[ 1 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 1 ].tan.z = vbuf[ 1 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 2 ].b.x = vbuf[ 2 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 2 ].b.y = vbuf[ 2 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 2 ].b.z = vbuf[ 2 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 2 ].tan.x = vbuf[ 2 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 2 ].tan.y = vbuf[ 2 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 2 ].tan.z = vbuf[ 2 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 3 ].b.x = vbuf[ 3 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 3 ].b.y = vbuf[ 3 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 3 ].b.z = vbuf[ 3 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 3 ].tan.x = vbuf[ 3 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 3 ].tan.y = vbuf[ 3 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 3 ].tan.z = vbuf[ 3 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	surf.indexes = indx;
	surf.vertexes = vbuf;
	surf.numOfindexes = 6;
	surf.numOftextures = 1;
	surf.numOfTriangles = 2;
	surf.numOfvertexes = 4;

	surf.PShaderName = "shaders\\room.psh";
	surf.VShaderName = "shaders\\room.vsh";

	strcpy( surf.texfilename, texName );

	indexes.push_back( render.RegisterSurface( surf ) );
	borders.push_back( gsurf );

	return 0;
}

int Level::CreateWallY( const Vector3 & posMin, const Vector3 & posMax, const Vector3 & n, const Vector2 & texMin, const Vector2 & texMax, const char * texName, bool cm_active  )
{
	g_surface gsurf;
	surface_t surf;
	Vector3 center;
	WORD * indx;

	WORD ibuf[ 6 ] = { 2, 0, 3, 2, 1, 0 };
	WORD ibufi[ 6 ] = { 0, 1, 2, 3, 0, 2 };

	indx = n.y < 0.0f ? ibuf : ibufi;
	vertex_t vbuf[ 4 ];

	gsurf.isActive = cm_active;
	gsurf.minB.x = min( posMin.x, posMax.x );
	gsurf.minB.y = min( posMin.y, posMax.y );
	gsurf.minB.z = min( posMin.z, posMax.z );

	gsurf.maxB.x = max( posMin.x, posMax.x );
	gsurf.maxB.y = max( posMin.y, posMax.y );
	gsurf.maxB.z = max( posMin.z, posMax.z );

	Vector3 dMin( BORDER_OFFSET, 0.0f, BORDER_OFFSET );

	gsurf.minB += dMin;
	gsurf.maxB -= dMin;

	gsurf.plane.SetNormal( n );
	gsurf.plane.SetDist( ( ( gsurf.minB + gsurf.maxB ) / 2  ) * n );

	vbuf[ 0 ].n = vbuf[ 1 ].n = vbuf[ 2 ].n = vbuf[ 3 ].n = n;

	vbuf[ 0 ].pos = Vector3( posMin.x, posMin.y, posMin.z );
	vbuf[ 1 ].pos = Vector3( posMin.x, posMin.y, posMax.z );
	vbuf[ 2 ].pos = Vector3( posMax.x, posMin.y, posMax.z );
	vbuf[ 3 ].pos = Vector3( posMax.x, posMin.y, posMin.z );

	vbuf[ 0 ].tex = Vector2( texMin.x, texMin.y );
	vbuf[ 1 ].tex = Vector2( texMin.x, texMax.y );
	vbuf[ 2 ].tex = Vector2( texMax.x, texMax.y );
	vbuf[ 3 ].tex = Vector2( texMax.x, texMin.y );

	vbuf[ 0 ].b.x = vbuf[ 0 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 0 ].b.y = vbuf[ 0 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 0 ].b.z = vbuf[ 0 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 0 ].tan.x = vbuf[ 0 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 0 ].tan.y = vbuf[ 0 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 0 ].tan.z = vbuf[ 0 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 1 ].b.x = vbuf[ 1 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 1 ].b.y = vbuf[ 1 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 1 ].b.z = vbuf[ 1 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 1 ].tan.x = vbuf[ 1 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 1 ].tan.y = vbuf[ 1 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 1 ].tan.z = vbuf[ 1 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 2 ].b.x = vbuf[ 2 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 2 ].b.y = vbuf[ 2 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 2 ].b.z = vbuf[ 2 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 2 ].tan.x = vbuf[ 2 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 2 ].tan.y = vbuf[ 2 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 2 ].tan.z = vbuf[ 2 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 3 ].b.x = vbuf[ 3 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 3 ].b.y = vbuf[ 3 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 3 ].b.z = vbuf[ 3 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 3 ].tan.x = vbuf[ 3 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 3 ].tan.y = vbuf[ 3 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 3 ].tan.z = vbuf[ 3 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	surf.indexes = indx;
	surf.vertexes = vbuf;
	surf.numOfindexes = 6;
	surf.numOftextures = 1;
	surf.numOfTriangles = 2;
	surf.numOfvertexes = 4;

	surf.PShaderName = "shaders\\room.psh";
	surf.VShaderName = "shaders\\room.vsh";

	strcpy( surf.texfilename, texName );

	indexes.push_back( render.RegisterSurface( surf ) );
	borders.push_back( gsurf );

	return 0;
}

int Level::CreateWallZ( const Vector3 & posMin, const Vector3 & posMax, const Vector3 & n, const Vector2 & texMin, const Vector2 & texMax, const char * texName, bool cm_active  )
{
	g_surface gsurf;
	surface_t surf;
	Vector3 center;
	WORD * indx;

#if 1
	WORD ibuf[ 6 ] = { 2, 0, 3, 2, 1, 0 };
	WORD ibufi[ 6 ] = { 0, 1, 2, 3, 0, 2 };
#else
	WORD ibufi[ 6 ] = { 2, 0, 3, 2, 1, 0 };
	WORD ibuf[ 6 ] = { 0, 1, 2, 3, 0, 2 };
#endif

	indx = n.z < 0.0f ? ibuf : ibufi;
	vertex_t vbuf[ 4 ];



	gsurf.isActive = cm_active;
	gsurf.minB.x = min( posMin.x, posMax.x );
	gsurf.minB.y = min( posMin.y, posMax.y );
	gsurf.minB.z = min( posMin.z, posMax.z );

	gsurf.maxB.x = max( posMin.x, posMax.x );
	gsurf.maxB.y = max( posMin.y, posMax.y );
	gsurf.maxB.z = max( posMin.z, posMax.z );

	Vector3 dMin( BORDER_OFFSET, 0.0f, BORDER_OFFSET );

	gsurf.minB += dMin;
	gsurf.maxB -= dMin;

	gsurf.plane.SetNormal( n );
	gsurf.plane.SetDist( ( ( gsurf.minB + gsurf.maxB ) / 2  ) * n );

	vbuf[ 0 ].n = vbuf[ 1 ].n = vbuf[ 2 ].n = vbuf[ 3 ].n = n;

	vbuf[ 0 ].pos = Vector3( posMin.x, posMax.y, posMin.z );
	vbuf[ 1 ].pos = Vector3( posMin.x, posMin.y, posMin.z );
	vbuf[ 2 ].pos = Vector3( posMax.x, posMin.y, posMin.z );
	vbuf[ 3 ].pos = Vector3( posMax.x, posMax.y, posMin.z );

	vbuf[ 0 ].tex = Vector2( texMin.x, texMin.y );
	vbuf[ 1 ].tex = Vector2( texMin.x, texMax.y );
	vbuf[ 2 ].tex = Vector2( texMax.x, texMax.y );
	vbuf[ 3 ].tex = Vector2( texMax.x, texMin.y );

	vbuf[ 0 ].b.x = vbuf[ 0 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 0 ].b.y = vbuf[ 0 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 0 ].b.z = vbuf[ 0 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 0 ].tan.x = vbuf[ 0 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 0 ].tan.y = vbuf[ 0 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 0 ].tan.z = vbuf[ 0 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 1 ].b.x = vbuf[ 1 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 1 ].b.y = vbuf[ 1 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 1 ].b.z = vbuf[ 1 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 1 ].tan.x = vbuf[ 1 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 1 ].tan.y = vbuf[ 1 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 1 ].tan.z = vbuf[ 1 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 2 ].b.x = vbuf[ 2 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 2 ].b.y = vbuf[ 2 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 2 ].b.z = vbuf[ 2 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 2 ].tan.x = vbuf[ 2 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 2 ].tan.y = vbuf[ 2 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 2 ].tan.z = vbuf[ 2 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	vbuf[ 3 ].b.x = vbuf[ 3 ].n * Vector3( 1.0f, 0.0f, 0.0f );
	vbuf[ 3 ].b.y = vbuf[ 3 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 3 ].b.z = vbuf[ 3 ].n * Vector3( 0.0f, 1.0f, 0.0f );

	vbuf[ 3 ].tan.x = vbuf[ 3 ].n * Vector3( 0.0f, 0.0f, -1.0f );
	vbuf[ 3 ].tan.y = vbuf[ 3 ].n * Vector3( 0.0f, 1.0f, 0.0f );
	vbuf[ 3 ].tan.z = vbuf[ 3 ].n * Vector3( 1.0f, 0.0f, 0.0f );

	surf.indexes = indx;
	surf.vertexes = vbuf;
	surf.numOfindexes = 6;
	surf.numOftextures = 1;
	surf.numOfTriangles = 2;
	surf.numOfvertexes = 4;

	surf.PShaderName = "shaders\\room.psh";
	surf.VShaderName = "shaders\\room.vsh";

	strcpy( surf.texfilename, texName );

	indexes.push_back( render.RegisterSurface( surf ) );
	borders.push_back( gsurf );

	return 0;
}

int Level::AddObject( GameObject & obj )
{
	gObject.push_back( obj );

	return 0;
}