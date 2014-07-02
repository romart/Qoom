#ifndef __R_FILE_H__
#define __R_FILE_H__

#include "..\global.h"
#include "..\math\Vector.h"
#include "..\file\file.h"

#include "r_global.h"

#define	SHADER_MAX_VERTEXES	1000
#define	SHADER_MAX_INDEXES	(6*SHADER_MAX_VERTEXES)
// the maximum size of game relative pathnames


#define MD3_IDENT			(('3'<<24)+('P'<<16)+('D'<<8)+'I')
#define MD3_VERSION			15

// limits
#define MD3_MAX_LODS		3
#define	MD3_MAX_TRIANGLES	8192	// per surface
#define MD3_MAX_VERTS		4096	// per surface
#define MD3_MAX_SHADERS		256		// per surface
#define MD3_MAX_FRAMES		1024	// per model
#define	MD3_MAX_SURFACES	32		// per model
#define MD3_MAX_TAGS		16		// per frame

#define	MD3_XYZ_SCALE		( 1.0f / 64.0f )


typedef struct {
	int ident;
	int version;
	byte name[64];
	int flags;
	int num_frames;
	int num_tags;
	int num_surfaces;
	int num_skins;
	int ofs_frames;
	int ofs_tags;
	int ofs_surfaces;
	int ofs_eof;
}md3Header_t;

typedef struct {
	Vector3		bounds[ 2 ];
	Vector3		localOrigin;
	float		radius;
	char		name[ 16 ];
} md3Frame_t;

typedef struct {
	char		name[ MAX_QPATH ];	// tag name
	Vector3		origin;
	Vector3		axis[ 3 ];
} md3Tag_t;

/*
** md3Surface_t
**
** CHUNK			SIZE
** header			sizeof( md3Surface_t )
** shaders			sizeof( md3Shader_t ) * numShaders
** triangles[0]		sizeof( md3Triangle_t ) * numTriangles
** st				sizeof( md3St_t ) * numVerts
** XyzNormals		sizeof( md3XyzNormal_t ) * numVerts * numFrames
*/

typedef struct {
	char			name[MAX_QPATH];
	int				shaderIndex;	// for in-game use
} md3Shader_t;

typedef struct {
	int			indexes[3];
} md3Triangle_t;

typedef struct {
	float		st[2];
} md3St_t;

typedef struct {
	short		xyz[3];
	short		normal;
} md3XyzNormal_t;

typedef struct {
	int		ident;				// 

	char	name[MAX_QPATH];	// polyset name

	int		flags;
	int		numFrames;			// all surfaces in a model should have the same

	int		numShaders;			// all surfaces in a model should have the same
	int		numVerts;

	int		numTriangles;
	int		ofsTriangles;

	int		ofsShaders;			// offset from start of md3Surface_t
	int		ofsSt;				// texture coords are common for all frames
	int		ofsXyzNormals;		// numVerts * numFrames

	int		ofsEnd;				// next surface follows

	md3Shader_t * shaders;
	md3Triangle_t * tri;
	md3St_t		*	st;
	md3XyzNormal_t * xyz;
	
} md3Surface_t;

class Model
{
public:
	Model( void );
	~Model( void );

	void	ClearMemory( void );

	void LoadModel( const char * filename );
	
	void Parce( void );

	int GetNumOfSurfaces( void ) const { return numOfSurfaces; }
	int GetNumOfFrames( void ) const { return header.num_frames; }

	const md3Frame_t * GetFramePtr( void ) const { return frames; }
	const surface_t & operator[] ( int i ) const { return r_surfaces[i]; }

	void SetVertexShader( const char * fileName ) { vertexS = fileName; }
	void SetPixelShader( const char * fileName )  { pixelS = fileName; }
private:
	File			file;
	md3Header_t		header;
	md3Frame_t	*	frames;
	md3Tag_t	*	tags;
	md3Surface_t*	surfaces;


	surface_t		r_surfaces[ MAX_NUM_OF_SURFACES ];

	bool	parsed;
	int numOfSurfaces;

	const char * pixelS;
	const char * vertexS;
};

#endif