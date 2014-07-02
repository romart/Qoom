#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "..\sys\sys_main_header.h"
#include "..\math\Plane.h"
#include "..\render\r_global.h"
#include "..\global.h"

#include "GameObject.h"

enum TYPE_BORD {
	TB_PLANE,
	TB_SPHERE
};

typedef struct {
	Plane	plane;
	Vector3 minB;
	Vector3 maxB;
	bool	isActive;

}g_surface;

class Player;

class Level
{
public:

	Level( void );
	~Level( void );

	int LoadFromFile( const char * fileName );

	int CreateTestLevel( void );

	void DrawLevel( const D3DXMATRIX & mat );
	void DrawLevelObject( const D3DXMATRIX * mat, const D3DXMATRIX & view ); 

	const g_surface & GetIndexSurface( int index ) const { assert( index >= 0 && index < (int)borders.size()); return borders[ index ]; }
	const GameObject & GetIndexObject( int index ) const { assert( index >= 0 && index < (int)gObject.size()); return gObject[ index ]; }

	int	GetNumOfSurfaces( void ) const { return borders.size(); }
	int GetNumOfGameObjects( void ) const { return gObject.size(); }

	int CreateWallX( const Vector3 & posMin, const Vector3 & posMax, const Vector3 & n, const Vector2 & texMin, const Vector2 & texMax, const char * texName, bool cm_active = true );
	int CreateWallY( const Vector3 & posMin, const Vector3 & posMax, const Vector3 & n, const Vector2 & texMin, const Vector2 & texMax, const char * texName, bool cm_active = true );
	int CreateWallZ( const Vector3 & posMin, const Vector3 & posMax, const Vector3 & n, const Vector2 & texMin, const Vector2 & texMax, const char * texName, bool cm_active = true );

	void SetPlayer( Player * player );

	Vector3 GenerateRandomLevel( int w, int h, float dw, float dh, int random );

	int AddObject( GameObject & obj );

private:

	static const BYTE	WALL	= 'W';
	static const BYTE	ROAD	= 'R';
	static const BYTE	PLAYER	= 'P';

	Player * player;

	BYTE GetGeneratorValue( int i ) const;

	std::vector< GameObject >	gObject;
	std::vector< int >			indexes;
	std::vector< g_surface >	borders;
};

#endif