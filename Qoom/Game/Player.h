#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "..\math\Box.h"
#include "..\math\Sphere.h"
#include "Camera.h"
#include "level.h"
#include "weapon.h"


#define EPSILON_COL_LVL	( 0.1f )
#define EPSILON_COL_OBJ	( 0.5f )

class Player
{
public:
	Player( void );

	~Player( void );

	void SetLevel( Level * level );
	void Move( float dx, float dy, float dz );
	void Rotation( float pitch, float yaw, float roll );
	void SetPos( float x, float y, float z );
	void SetSize( float w, float h, float d );
	void SetCMSize( float r ) { sphere.SetRadius( r ); } 
	void SetWeapon( Weapon * w ) { weapon = w; w->SetPlayer( this ); }
	void DrawView( void );

	const Camera & GetCamera( void ) const { return camera; }

private:

	float width;
	float height;
	float depth;

	Sphere	sphere;
	Level *	level;
	Camera	camera;
	Box		box;
	Weapon *weapon;
};

#endif