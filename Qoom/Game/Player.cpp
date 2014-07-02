#include "Player.h"
#include "..\math\Quat.h"
#include "..\framework\log.h"

extern Logger logger;

Player::Player( void )
{
	camera.Init();
	level = NULL;
	width = height = depth = 0.0f;
	weapon = NULL;
}


Player::~Player( void )
{

}

void Player::SetLevel( Level * _level )
{
	level = _level;
	level->SetPlayer( this );
}

void Player::SetPos( float _x, float _y, float _z )
{
	//box.SetMax( Vector3( _x + width / 2, _y, _z + depth / 2 ) ); 
	//box.SetMin( Vector3( _x - width / 2, _y - height, _z - depth / 2 ) );
	sphere.SetCenter( Vector3( _x, _y - sphere.GetRadius(), _z ) );
	camera.SetEye( Vector3( _x, _y, _z ) );
}

void Player::SetSize( float w, float h, float d )
{
	width	= w;
	height	= h;
	depth	= d;
}

static void RotateVec3Y( Vector3 & vec, float angle )
{
	D3DXMATRIX rot;
	D3DXVECTOR4 res, tmp( vec.x, vec.y, vec.z, 1.0f );
	D3DXMatrixRotationY( &rot, angle );
	D3DXVec4Transform( &res, &tmp, &rot );
	float s = vec.x > 0.0f ? 1.0f : -1.0f;
	vec.x = s * fabs( res.x / res.w );
	s = vec.y > 0.0f ? 1.0f : -1.0f;
	vec.y = s * fabs( res.y / res.w );
	s = vec.z > 0.0f ? 1.0f : -1.0f;
	vec.z = s * fabs( res.z / res.w );
}

void Player::Move( float dx, float dy, float dz )
{
	Vector3 f = camera.GetForward();
	Vector3 r = camera.GetRight();
	Vector3 u = camera.GetUp();
	Vector3 pos = sphere.GetCenter();

	Level & lvl = *level;

	int b_num = lvl.GetNumOfSurfaces();
	int o_num = lvl.GetNumOfGameObjects();


	u.x = u.z = 0.0f;
	r.y = f.y = 0.0f;

	Vector3 v( dx, dy, dz );
//	Vector3 v( dx, dy, dz );
	float angle = acos( f.z / f.Length() );
//	angle = fabs( angle ) < EPSILON_ANGLE ? 0.0f : angle;
	Vector3 rotPos = pos; 
	RotateVec3Y( rotPos, angle );
	for( int i = 0; i < b_num; ++i ) {
		if ( dx == 0.0f && dy == 0.0f && dz == 0.0f ) break;
		const g_surface & srf = lvl.GetIndexSurface( i );
		if ( !srf.isActive ) continue;
		
#ifdef BAD_CM

		float xMin = fabs( pos.x - srf.minB.x ); float xMax = fabs( pos.x - srf.maxB.x );
		float yMin = fabs( pos.y - srf.minB.y ); float yMax = fabs( pos.y - srf.maxB.y );
		float zMin = fabs( pos.z - srf.minB.z ); float zMax = fabs( pos.z - srf.maxB.z );

		float xDist = fabs( srf.minB.x - srf.maxB.x );
		float yDist = fabs( srf.minB.y - srf.maxB.y );
		float zDist = fabs( srf.minB.z - srf.maxB.z );

		Vector3 minV( xMin, yMin, zMin );
		Vector3 maxV( xMax, yMax, zMax );
		Vector3 disV( xDist, yDist, zDist );

		float angle = acos( Vector3( 0.0f, 0.0f, 1.0f ) * camera.GetForward() );

		minV = Quat( u.x, u.y, u.z, angle * 2 ) * minV;
		maxV = Quat( u.x, u.y, u.z, angle * 2 ) * maxV;
		disV = Quat( u.x, u.y, u.z, angle * 2 ) * disV;

		xMin = minV.x; yMin = minV.y; zMin = minV.z;
		xMax = maxV.x; yMax = maxV.y; zMax = maxV.z;
		xDist = disV.x; yDist = disV.y; zDist = disV.z;

		if ( xMin < xDist && xMax < xDist ) {
			float deltaX = fabs( ( sphere + r * dx ).DistanseToPlane( srf.plane ) );
			dx = deltaX < EPSILON_COL ? 0.0f : dx;
		}

		if ( yMin < yDist && yMax < yDist ) {
			float deltaY = fabs( ( sphere + u * dy ).DistanseToPlane( srf.plane ) );
			dy = deltaY < EPSILON_COL ? 0.0f : dy;
		}

		if ( zMin < zDist && zMax < zDist ) {
			float deltaZ = fabs( ( sphere + f * dz ).DistanseToPlane( srf.plane ) );
			dz = deltaZ < EPSILON_COL ? 0.0f : dz;
		}
#else

		float dMin = min( ( pos - srf.minB ).Length(), ( srf.maxB - pos ).Length() );
		float dMax = max( ( pos - srf.minB ).Length(), ( srf.maxB - pos ).Length() );
		float dist = ( srf.minB - srf.maxB ).Length();
		//float check_EPS = 2 * max( max( fabs( dx ), fabs( dy ) ), fabs( dz ) );

		if ( dMin < dist && dist > dMax ) {
		//if ( fabs( dMin - dist ) > check_EPS && fabs( dist - dMax ) > check_EPS ) {
			float deltaX = fabs( ( sphere + r * dx ).DistanseToPlane( srf.plane ) );
			dx = deltaX < EPSILON_COL_LVL ? 0.0f : dx;
			float deltaY = fabs( ( sphere + u * dy ).DistanseToPlane( srf.plane ) );
			dy = deltaY < EPSILON_COL_LVL ? 0.0f : dy;
			float deltaZ = fabs( ( sphere + f * dz ).DistanseToPlane( srf.plane ) );
			dz = deltaZ < EPSILON_COL_LVL ? 0.0f : dz;
		}
		//if ( dMin < dist && dist > dMax ) {
		//	if ( ( ( minB.x + rad ) < pos.x && pos.x < ( maxB.x - rad ) ) ) {
		//		float deltaZ = fabs( ( sphere + f * dz ).DistanseToPlane( srf.plane ) );
		//		dz = deltaZ < EPSILON_COL ? 0.0f : dz;
		//		float deltaY = fabs( ( sphere + u * dy ).DistanseToPlane( srf.plane ) );
		//		dy = deltaY < EPSILON_COL ? 0.0f : dy;
		//	}
		//	if ( ( minB.y + rad ) < pos.y && pos.y < ( maxB.y - rad ) ) {
		//		float deltaX = fabs( ( sphere + r * dx ).DistanseToPlane( srf.plane ) );
		//		dx = deltaX < EPSILON_COL ? 0.0f : dx;
		//		float deltaZ = fabs( ( sphere + f * dz ).DistanseToPlane( srf.plane ) );
		//		dz = deltaZ < EPSILON_COL ? 0.0f : dz;
		//	}
		//	if ( ( ( minB.z + rad ) < pos.z && pos.z < ( maxB.z - rad )  ) ) {
		//		float deltaX = fabs( ( sphere + r * dx ).DistanseToPlane( srf.plane ) );
		//		dx = deltaX < EPSILON_COL ? 0.0f : dx;
		//		float deltaY = fabs( ( sphere + u * dy ).DistanseToPlane( srf.plane ) );
		//		dy = deltaY < EPSILON_COL ? 0.0f : dy;
		//	}
		//}
#endif
	}

	for( int i = 0; i < o_num; ++i ) {
		if ( dx == 0.0f && dy == 0.0f && dz == 0.0f ) break;

		const GameObject & obj = lvl.GetIndexObject( i );

		if ( !obj.IsCmActive() ) continue;

		Vector3 v( dx, dy, dz );
		const Sphere & objsp = obj.GetSphere();

		float dist = sphere.DistanceToSphere( objsp );
		if ( dist < 2 * v.Length() ) {
			float deltaX = ( sphere + r * dx ).DistanceToSphere( objsp );
			dx = deltaX < EPSILON_COL_OBJ ? 0.0f : dx;
			float deltaY = ( sphere + u * dy ).DistanceToSphere( objsp );
			dy = deltaY < EPSILON_COL_OBJ ? 0.0f : dy;
			float deltaZ = ( sphere + f * dz ).DistanceToSphere( objsp );
			dz = deltaZ < EPSILON_COL_OBJ ? 0.0f : dz;
		}
	}

	camera.Move( dx, dy, dz );
	sphere += ( r * dx );
	sphere += ( u * dy );
	sphere += ( f * dz );
}

void Player::DrawView( void )
{
	assert( weapon );
	weapon->DrawWeapon();
	camera.DrawInterface();
}

void Player::Rotation( float pitch, float yaw, float roll )
{
	camera.Rotate( pitch, 0.0f, 0.0f );
	camera.Rotate( 0.0f, yaw, 0.0f );
	camera.Rotate( 0.0f, 0.0f, roll );
}