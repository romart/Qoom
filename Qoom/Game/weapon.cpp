#include "weapon.h"
#include "Player.h"
#include "..\render\r_file.h"

extern Render render;
extern Logger logger;

#define WEAPON_P_SHADER	"shaders\\weapon.psh"
#define WEAPON_V_SHADER	"shaders\\weapon.vsh"

Weapon::Weapon( void )
{
	index = -1;
	x = y = z = 0.0f;
	ax = ay = az = 0.0f;
	player = NULL;
}

Weapon::~Weapon( void )
{

}

void Weapon::LoadFromFile( const char * fileName )
{
	Model model;
	model.LoadModel( fileName );
	model.SetVertexShader( "shaders\\weapon.vsh" );
	model.SetPixelShader( "shaders\\weapon.psh" );
	model.Parce();
	index = render.RegisterModel( model );
	if ( index < 0 ) {
		logger.Error( "Weapon::LoadFromFile: cannot load weapon from %s", fileName );
	}
}

void Weapon::DrawWeapon( void )
{
	vec4_t v1[ 6 ];
	Vector3 view = player->GetCamera().GetForward();
	Vector3 pos =  player->GetCamera().GetEye();
	D3DXMATRIX mat;
	D3DXMatrixTranslation( &mat, pos.x, pos.y, pos.z );

	v1[ 0 ][ 0 ] = view.x; v1[ 0 ][ 1 ] = view.y; v1[ 0 ][ 2 ] = view.z; v1[ 0 ][ 3 ] = 1.0f;
	v1[ 1 ][ 0 ] = 300.0f; v1[ 1 ][ 1 ] = 50.0f; v1[ 1 ][ 2 ] = 300.0f; v1[ 1 ][ 3 ] = 0.0f;
	memcpy( &v1[2][0], &mat, sizeof( mat ) );

	render.SetVertexConstArray( v1, 6 );
	D3DXMATRIX world, tmp;
	D3DXMatrixScaling( &world, scale, scale, scale );
	D3DXMatrixRotationX( &tmp, ax );
	world *= tmp;
	D3DXMatrixRotationY( &tmp, ay );
	world *= tmp;
	D3DXMatrixRotationZ( &tmp, az );
	world *= tmp;
	D3DXMatrixTranslation( &tmp, x, y, z );
	world *= tmp;
	render.DrawIndexModel( index, world );
	render.SetVertexConstArray();
}

void Weapon::SetPos( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

void Weapon::SetRot( float _ax, float _ay, float _az )
{
	ax = _ax;
	ay = _ay;
	az = _az;
}

void Weapon::SetPlayer( Player * p )
{
	player = p;
}