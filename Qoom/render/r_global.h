#ifndef __R_GLOBAL_H__
#define __R_GLOBAL_H__

#define	MAX_QPATH		64

typedef struct {
	LPDIRECT3DVERTEXBUFFER9	v_buffer;
	LPDIRECT3DINDEXBUFFER9	i_buffer;
	LPDIRECT3DTEXTURE9		texture;
	int	pVShaderIndex;
	int pPShaderIndex;
	int numOfvertex;
	int numOfindex;
	int numOftrian;
} rd3dSurface_t;

typedef struct {
	rd3dSurface_t		surfaces[ MAX_NUM_OF_SURFACES ];
	int					numOfserfaces;
} rd3dModel_t;


typedef struct {
	Vector3 pos;
	Vector2 tex;
	Vector3 n;
	Vector3 b;
	Vector3 tan;
} vertex_t;

typedef struct {
	vertex_t	*	vertexes;
	word		*	indexes;
	char			texfilename[ MAX_QPATH ];
	int				numOftextures;
	int				numOfvertexes;
	int				numOfindexes;
	int				numOfTriangles;
	const char	*	VShaderName;
	const char	*	PShaderName;
} surface_t;

typedef float vec4_t[ 4 ];

#endif