#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "..\render\r_common.h"
#include "..\framework\log.h"

class Player;

class Weapon
{
public:
	Weapon( void );
	~Weapon( void );

	void LoadFromFile( const char * fileName );
	void DrawWeapon( void );

	void SetPos( float x, float y, float z );
	void SetRot( float ax, float ay, float az );
	void SetScale( float s ) { scale = s; }
	void SetPlayer( Player * player );
private:
	int index;

	float x, y, z;
	float ax, ay, az;
	float scale;
	Player * player;

};

#endif