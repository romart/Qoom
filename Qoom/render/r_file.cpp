#include "r_file.h"
#include "r_common.h"
#include "..\framework\log.h"

extern Logger logger;

Model::Model( void )
{
	memset( &header, 0, sizeof( md3Header_t ) );

	frames = NULL;
	tags = NULL;
	surfaces = NULL;

	parsed = false;
	numOfSurfaces = 0;

	vertexS = NULL;
	pixelS = NULL;

}

void Model::ClearMemory( void )
{
	if ( surfaces != NULL ) {
		for( int i = 0; i < header.num_surfaces; ++i ) {
			delete[] surfaces[ i ].xyz; surfaces[ i ].xyz = NULL;
			delete[] surfaces[ i ].st;	surfaces[ i ].st = NULL;
			delete[] surfaces[ i ].shaders;	surfaces[ i ].shaders = NULL;
		}
	}
	delete[] frames;	frames = NULL;
	delete[] tags;		tags =  NULL;
	delete[] surfaces;	surfaces = NULL;
}

Model::~Model( void )
{
	file.Close();
	ClearMemory();
}

void Model::LoadModel( const char * filename )
{
	file.Open( filename, File::F_READ_ONLY | File::F_BINARY );
	if ( !file.IsOpen() ) {
		logger.Error( "%s: cannot open file", filename );
		return ;
	}

	logger.Info( "Model::LoadModel: load %s", filename );

	file.Read( ( byte* )&header, sizeof( md3Header_t ) );

	if ( header.num_frames < 1 ) {
		logger.Warning( "Model::LoadModel: %s has no frames\n", filename );
		return ;
	}

	frames = new md3Frame_t[ header.num_frames ];

	for ( int i = 0 ; i < header.num_frames ; ++i ) {
		file.Read( ( byte* )&frames[ i ], sizeof( md3Frame_t ) );
	}

	logger.Info( "Model::LoadModel: was read %d frames from %s", header.num_frames, filename );

	if ( header.num_tags > 0 ) {
		tags = new md3Tag_t[ header.num_tags ];
		for ( int i = 0; i < header.num_tags; ++i ) {
			file.Read( ( byte* )&tags[i], sizeof( md3Tag_t ) );
		}
		logger.Info( "Model::LoadModel: was read %d tags from %s", header.num_tags, filename );
	} else {
		logger.Warning( "Model::LoadModel: %s has no tags\n", filename );
	}

	logger.Info( "Model::LoadModel: was read %d tags from %s", header.num_tags, filename );

	surfaces = new md3Surface_t[ header.num_surfaces ];

	numOfSurfaces = header.num_surfaces;

	for ( int i = 0; i < header.num_surfaces; ++i ) {
		file.Read( ( byte* )&surfaces[ i ].ident, sizeof( surfaces[ i ].ident ) );
		file.Read( ( byte* )&surfaces[ i ].name, sizeof( surfaces[ i ].name ) );
		file.Read( ( byte* )&surfaces[ i ].flags, sizeof( surfaces[ i ].flags ) );
		file.Read( ( byte* )&surfaces[ i ].numFrames, sizeof( surfaces[ i ].numFrames ) );
		file.Read( ( byte* )&surfaces[ i ].numShaders, sizeof( surfaces[ i ].numShaders ) );
		file.Read( ( byte* )&surfaces[ i ].numVerts, sizeof( surfaces[ i ].numVerts ) );
		file.Read( ( byte* )&surfaces[ i ].numTriangles, sizeof( surfaces[ i ].numTriangles ) );
		file.Read( ( byte* )&surfaces[ i ].ofsTriangles, sizeof( surfaces[ i ].ofsTriangles ) );
		file.Read( ( byte* )&surfaces[ i ].ofsShaders, sizeof( surfaces[ i ].ofsShaders ) );
		file.Read( ( byte* )&surfaces[ i ].ofsSt, sizeof( surfaces[ i ].ofsSt ) );
		file.Read( ( byte* )&surfaces[ i ].ofsXyzNormals, sizeof( surfaces[ i ].ofsXyzNormals ) );
		file.Read( ( byte* )&surfaces[ i ].ofsEnd, sizeof( surfaces[ i ].ofsEnd ) );

		if ( surfaces[ i ].numVerts > SHADER_MAX_VERTEXES ) {
			logger.Error( "Model::LoadModel: %s has more than %i verts on a surface (%i)",
				filename, SHADER_MAX_VERTEXES, surfaces[ i ].numVerts );
		}
		if ( surfaces[ i ].numTriangles*3 > SHADER_MAX_INDEXES ) {
			logger.Error( "Model::LoadModel: %s has more than %i triangles on a surface (%i)",
				filename, SHADER_MAX_INDEXES / 3, surfaces[ i ].numTriangles );
		}

		int j = strlen( surfaces[ i ].name );
		if ( j > 2 && surfaces[ i ].name[j-2] == '_' ) {
			surfaces[ i ].name[j-2] = 0;
		}

		surfaces[ i ].shaders = new md3Shader_t[ surfaces[ i ].numShaders ];

		for( int j = 0; j < surfaces[ i ].numShaders; ++j ) {
			file.Read( ( byte* )&surfaces[ i ].shaders[ j ], sizeof( md3Shader_t ) );
		}

		surfaces[ i ].tri = new md3Triangle_t[ surfaces[ i ].numTriangles ];

		for ( int j = 0 ; j < surfaces[ i ].numTriangles ; j++ ) {
			file.Read( ( byte* )&surfaces[ i ].tri[ j ].indexes[ 0 ], sizeof( int ) );
			file.Read( ( byte* )&surfaces[ i ].tri[ j ].indexes[ 1 ], sizeof( int ) );
			file.Read( ( byte* )&surfaces[ i ].tri[ j ].indexes[ 2 ], sizeof( int ) );
		}

		/*numOfTriangles += surfaces[ i ].numTriangles;*/

		surfaces[ i ].st = new md3St_t[ surfaces[ i ].numVerts ];

		for ( int j = 0 ; j < surfaces[ i ].numVerts ; ++j ) {
			file.Read( ( byte* )&surfaces[ i ].st[ j ].st[ 0 ], sizeof( float ) );
			file.Read( ( byte* )&surfaces[ i ].st[ j ].st[ 1 ], sizeof( float ) );
        }

		int numVertexes = surfaces[ i ].numVerts * surfaces[ i ].numFrames;

		surfaces[ i ].xyz = new md3XyzNormal_t[ numVertexes ];

		for ( int j = 0 ; j < numVertexes; j++ ) {
			file.Read( ( byte* )&surfaces[ i ].xyz[ j ].xyz[ 0 ], sizeof( short ) );
			file.Read( ( byte* )&surfaces[ i ].xyz[ j ].xyz[ 1 ], sizeof( short ) );
			file.Read( ( byte* )&surfaces[ i ].xyz[ j ].xyz[ 2 ], sizeof( short ) );

			file.Read( ( byte* )&surfaces[ i ].xyz[ j ].normal, sizeof( short ) );
		}
		//numOfvertexes += numVertexes;
	}

	logger.Info( "Model::LoadModel: was read %d surfaces from %s", header.num_surfaces, filename );
	file.Close();
	return ;
}


void Model::Parce( void )
{
	if ( surfaces == NULL ) {
		logger.Error( "Model::Parce: try to parce bad model" );
		return ;
	}

	if ( parsed ) return ;

	for( int i = 0; i < header.num_surfaces; ++i ) {

		r_surfaces[ i ].vertexes = new vertex_t[ surfaces[ i ].numVerts ];
		r_surfaces[ i ].indexes = new word[ 3 * surfaces[ i ].numTriangles ];

		r_surfaces[ i ].numOfvertexes = surfaces[ i ].numVerts;
		r_surfaces[ i ].numOfindexes  = 3 * surfaces[ i ].numTriangles;
		r_surfaces[ i ].numOfTriangles= surfaces[ i ].numTriangles;

		int k = 0;
		for ( int j = 0; j < surfaces[ i ].numTriangles; ++j ) {
			r_surfaces[ i ].indexes[ k++ ] = ( word )surfaces[ i ].tri[ j ].indexes[ 2 ];
			r_surfaces[ i ].indexes[ k++ ] = ( word )surfaces[ i ].tri[ j ].indexes[ 1 ];
			r_surfaces[ i ].indexes[ k++ ] = ( word )surfaces[ i ].tri[ j ].indexes[ 0 ];
		}
		delete[] surfaces[ i ].tri;

		r_surfaces[ i ].numOfindexes = k;

		for ( int j = 0; j < surfaces[ i ].numVerts; ++j ) {
			//pos
			r_surfaces[ i ].vertexes[ j ].pos.x = (float)( surfaces[ i ].xyz[ j ].xyz[ 0 ] ) * MD3_XYZ_SCALE;
			r_surfaces[ i ].vertexes[ j ].pos.y = (float)( surfaces[ i ].xyz[ j ].xyz[ 1 ] ) * MD3_XYZ_SCALE;
			r_surfaces[ i ].vertexes[ j ].pos.z = (float)( surfaces[ i ].xyz[ j ].xyz[ 2 ] ) * MD3_XYZ_SCALE;

			float lat = (float)( ( surfaces[ i ].xyz[ j ].normal >> 8 ) & 0xff );
			float lng = (float)( surfaces[ i ].xyz[ j ].normal & 0xff );
			lat *= D3DX_PI / 128;
			lng *= D3DX_PI / 128;

			r_surfaces[ i ].vertexes[ j ].n.x = cos( lat ) * sin( lng );
			r_surfaces[ i ].vertexes[ j ].n.y = sin( lat ) * sin( lng );
			r_surfaces[ i ].vertexes[ j ].n.z = cos( lng );


			r_surfaces[ i ].vertexes[ j ].tex.x = surfaces[ i ].st[ j ].st[ 0 ];
			r_surfaces[ i ].vertexes[ j ].tex.y = surfaces[ i ].st[ j ].st[ 1 ];
			

			r_surfaces[ i ].vertexes[ j ].b.x = r_surfaces[ i ].vertexes[ j ].n * Vector3( 1.0f, 0.0f, 0.0f );
			r_surfaces[ i ].vertexes[ j ].b.y = r_surfaces[ i ].vertexes[ j ].n * Vector3( 0.0f, 0.0f, -1.0f );
			r_surfaces[ i ].vertexes[ j ].b.z = r_surfaces[ i ].vertexes[ j ].n * Vector3( 0.0f, 1.0f, 0.0f );

			r_surfaces[ i ].vertexes[ j ].tan.x = r_surfaces[ i ].vertexes[ j ].n * Vector3( 0.0f, 0.0f, -1.0f );
			r_surfaces[ i ].vertexes[ j ].tan.y = r_surfaces[ i ].vertexes[ j ].n * Vector3( 0.0f, 1.0f, 0.0f );
			r_surfaces[ i ].vertexes[ j ].tan.z = r_surfaces[ i ].vertexes[ j ].n * Vector3( 1.0f, 0.0f, 0.0f );
		}

#if 0
		for( int j = 0; j < surfaces[ i ].numShaders; ++j ) {
			int l = strlen( surfaces[ i ].shaders[ j ].name );
			int _l;
			for( _l = ( l - 1 ); _l > 0 && surfaces[ i ].shaders[ j ].name[ _l ] != '/'; --_l ) ;
			++_l;
			strcpy( &r_surfaces[ i ].texfilename[ 0 ], surfaces[ i ].shaders[ j ].name + _l );
			texfilename[ k ][ l - _l - 3 ] = 'j';
			texfilename[ k ][ l - _l - 2 ] = 'p';
			texfilename[ k ][ l - _l - 1 ] = 'g';
			++k;
		}
#endif
		r_surfaces[ i ].VShaderName = vertexS;
		r_surfaces[ i ].PShaderName = pixelS;

		strcpy( &r_surfaces[ i ].texfilename[ 0 ], surfaces[ i ].shaders[ 0 ].name );
		r_surfaces[ i ].numOftextures = 1;
	}
	parsed = true;
}