#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "..\math\Box.h"
#include "..\render\r_file.h"

class Player;

class GameObject
{
public:
	GameObject( void );
	~GameObject( void ) {}

	bool IsCmActive( void ) const { return cm_active; }
	bool IsRActive( void ) const { return r_active; }

	inline	void ActivateCM( void ) { cm_active = true; }
	inline	void DeActivateCM( void ) { cm_active = false; }

	inline	void ActivateR( void ) { r_active = true; }
	inline	void DeActivateR( void ) { r_active = false; }

	void SetVertexShader( const char * shader ) { vShader = shader; }
	void SetPixelShader( const char * shader ) { pShader = shader; }

	void LoadFromFile( const char * fileName );

	const Box		&	operator ()( Box ) const { return box; }
	const Vector3	&	operator ()( Vector3 ) const { return pos; }

	const Sphere	&	GetSphere( void ) const { return sphere; }

	void	SetPos( const Vector3 & vec );
	void	SetScale( float scale );
	void	DrawObject( const D3DXMATRIX & mat, const D3DXMATRIX & view );
	void	SetPlayer( Player * player ) { this->player = player; }

	void	CreateCMObject( const Vector3 & pos, float r );

private:

	int CreateFromModel( Model & model );

	Vector3 pos;
	Box		box;
	Sphere	sphere;
	float	w;
	float	h;
	float	d;

	float	scale;

	bool	cm_active;	//collision model
	bool	r_active;	//render

	int		index;

	const char * fileName;
	const char * vShader;
	const char * pShader;

	Player * player;
};

#endif