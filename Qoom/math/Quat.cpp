#include "Quat.h"
#include "..\sys\sys_main_header.h"


D3DXMATRIX	Quat::ToMatrix4( void ) const
{
	D3DXMATRIX ret;

	float	wx, wy, wz;
	float	xx, yy, yz;
	float	xy, xz, zz;
	float	x2, y2, z2;

	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	ret._11 = 1.0f - ( yy + zz );
	ret._12 = xy - wz;
	ret._13 = xz + wy;
	ret._14 = 0.0f;

	ret._21 = xy + wz;
	ret._22 = 1.0f - ( xx + zz );
	ret._23 = yz - wx;
	ret._24 = 0.0f;

	ret._31 = xz - wy;
	ret._32 = yz + wx;
	ret._33 = 1.0f - ( xx + yy );
	ret._34 = 0.0f;

	ret._41 = 0.0f; ret._42 = 0.0f; ret._43 = 0.0f; ret._44 = 1.0f;

	return ret;
}

Vector3 Quat::ToAngularVelocity( void ) const
{
	Vector3 vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.Normalize();
	return vec * acos( w );
}

Angle Quat::ToAngles( void ) const
{
	Vector3 mat[ 3 ];
	float	wx, wy, wz;
	float	xx, yy, yz;
	float	xy, xz, zz;
	float	x2, y2, z2;

	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	mat[ 0 ].x = 1.0f - ( yy + zz );
	mat[ 0 ].y = xy - wz;
	mat[ 0 ].z = xz + wy;

	mat[ 1 ].x = xy + wz;
	mat[ 1 ].y = 1.0f - ( xx + zz );
	mat[ 1 ].z = yz - wx;

	mat[ 2 ].x = xz - wy;
	mat[ 2 ].y = yz + wx;
	mat[ 2 ].z = 1.0f - ( xx + yy );

	Angle angles;
	float s = sqrt( mat[0].x * mat[0].x + mat[0].y * mat[0].y );
	if ( s > 0.0f ) {
		angles.pitch = -atan2f( mat[0].z, s );
		angles.yaw = atan2f( mat[0].y, mat[0].x );
		angles.roll = atan2f( mat[1].z, mat[2].z );
	} else {
		angles.pitch = mat[0].z < 0.0f ? ( D3DX_PI / 2 ) : -( D3DX_PI / 2 );
		angles.yaw = -atan2f( mat[1].x, mat[1].y );
		angles.roll = 0.0f;
	}
	
	return angles;
}