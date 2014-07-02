#ifndef __QUAT_H__
#define __QUAT_H__

#include "Vector.h"
#include "Angle.h"
#include "..\sys\sys_main_header.h"
//class D3DXMATRIX;

class Quat
{
public:
	float x;
	float y;
	float z;
	float w;


	Quat( void ) { Clear(); }
	Quat( float x, float y, float z, float w );
	~Quat( void ) {};

	void Clear( void ) { x = y = z = w = 0.0f; }
	void Set( float x, float y, float z, float w );

	Quat		operator-() const { return Quat( -x, -y, -z, -w ); } ;
	Quat &		operator=( const Quat &a ) { x = a.x; y = a.y; z = a.z; w = a.w; };
	Quat &		operator+=( const Quat &a );
	Quat &		operator-=( const Quat &a );
	Vector3		operator*( const Vector3 &a ) const;
	Quat &		operator*=( const Quat &a );
	Quat &		operator*=( float a );

	bool		operator==(	const Quat &a ) const;
	bool		operator!=(	const Quat &a ) const;

	Quat		Inverse( void ) const;
	float		Length( void ) const;
	Quat &		Normalize( void );

	Angle		ToAngles( void ) const;
	D3DXMATRIX	ToMatrix4( void ) const;
	Vector3		ToAngularVelocity( void ) const;


	friend Quat	operator* ( const Quat & l, const Quat & r );

};



inline Quat::Quat( float _x, float _y, float _z, float _w )
{
	Set( _x, _y, _z, _w );
}


inline void Quat::Set( float _x, float _y, float _z, float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

inline Quat & Quat::operator += ( const Quat & a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}

inline Quat & Quat::operator -=( const Quat & a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}

inline Quat & Quat::operator *=( const Quat & a )
{
	x = w*a.x + x*a.w + y*a.z - z*a.y;
	y = w*a.y + y*a.w + z*a.x - x*a.z;
	z = w*a.z + z*a.w + x*a.y - y*a.x;
	w = w*a.w - x*a.x - y*a.y - z*a.z;
	return *this;
}

inline Quat & Quat::operator *=( float a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}

inline Vector3	Quat::operator*( const Vector3 &a ) const
{
	float xxzz = x*x - z*z;
	float wwyy = w*w - y*y;

	float xw2 = x*w*2.0f;
	float xy2 = x*y*2.0f;
	float xz2 = x*z*2.0f;
	float yw2 = y*w*2.0f;
	float yz2 = y*z*2.0f;
	float zw2 = z*w*2.0f;

	return Vector3(
		(xxzz + wwyy)*a.x		+ (xy2 + zw2)*a.y		+ (xz2 - yw2)*a.z,
		(xy2 - zw2)*a.x			+ (y*y+w*w-x*x-z*z)*a.y	+ (yz2 + xw2)*a.z,
		(xz2 + yw2)*a.x			+ (yz2 - xw2)*a.y		+ (wwyy - xxzz)*a.z
	);
}

inline Quat operator*( const Quat & l, const Quat & r )
{
	return Quat(	l.w*r.x + l.x*r.w + l.y*r.z - l.z*r.y,
					l.w*r.y + l.y*r.w + l.z*r.x - l.x*r.z,
					l.w*r.z + l.z*r.w + l.x*r.y - l.y*r.x,
					l.w*r.w - l.x*r.x - l.y*r.y - l.z*r.z );
}

inline bool	Quat::operator==( const Quat &a ) const
{
	return ( x == a.x && y == a.y && z == a.z && w == a.w );
}

inline bool	Quat::operator!=( const Quat &a ) const
{
	return ( x != a.x || y != a.y || z != a.z || w != a.w );
}

inline Quat Quat::Inverse( void ) const
{
	return Quat( -x, -y, -z, -w );
}

inline float Quat::Length( void ) const
{
	float len;

	len = x * x + y * y + z * z + w * w;
	return sqrt( len );
}

inline Quat& Quat::Normalize( void )
{
	float len;
	float ilength;

	len = this->Length();
	if ( len ) {
		ilength = 1 / len;
		x *= ilength;
		y *= ilength;
		z *= ilength;
		w *= ilength;
	}
	return *this;
}



#endif