#ifndef __PLANE_H__
#define __PLANE_H__

#include "Vector.h"

class Sphere;

class Plane
{
public:
	Plane( void );
	Plane( float a, float b, float c, float d );
	Plane( const Vector3 & n, float d );
	~Plane( void ) {}

	void Clear( void );

	void SetNormal( const Vector3 & n ) { a = n.x; b = n.y; c = n.z; }
	void SetDist( float _d ) { d = _d; }

	Vector3 Normal( void ) const;
	float Dist( void ) const;

	float DistanceFromPoint( const Vector3 & point ) const;
	float DistanceFromPlane( const Plane & plane ) const;
	float DistanceFromSphere( const Sphere & sphere ) const;

	bool	LineIntersection( const Vector3 & start, const Vector3 & end ) const;
	bool	RayIntersection( const Vector3 & start, const Vector3 & dir, float & scale ) const;
	bool	PlaneIntersection( const Plane & plane, Vector3 & start, Vector3 & dir ) const;

	Plane	Tranlation( const Vector3 & vec ) const;		
	void	TranlationSelf( const Vector3 & vec );

private:
	float a;
	float b;
	float c;
	float d;
};

inline Plane::Plane( void )
{
	Clear();
}

inline Plane::Plane( float a, float b, float c, float d )
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

inline Plane::Plane( const Vector3 & n, float d )
{
	this->a = n.x;
	this->b = n.y;
	this->c = n.z;
	this->d = -d;
}

inline void Plane::Clear( void )
{
	a = 0.0f; b = 0.0f; c = 0.0f; d = 0.0f;
}

inline Vector3 Plane::Normal( void ) const
{
	return Vector3( a, b, c );
}

inline float Plane::Dist( void ) const 
{
	return -d;
}

inline float Plane::DistanceFromPoint( const Vector3 & point ) const
{
	float norm = Normal().Length();
	if ( norm > MIN_FLOAT_VALUE ) {
		return ( ( Normal() * point + d ) / norm );
	} else {
		return 0.0f;
	}
}

inline float Plane::DistanceFromPlane( const Plane & plane ) const
{
	if ( Normal() != plane.Normal() || Normal().Length() < MIN_FLOAT_VALUE ) {
		return 0.0f;
	}
	else {
		return ( abs( d - plane.d ) / Normal().Length() );
	}
}

inline bool	Plane::LineIntersection( const Vector3 & start, const Vector3 & end ) const
{
	float d1, d2, fraction;

	d1 = Normal() * start + d;
	d2 = Normal() * end + d;

	if ( d1 == d2 ) {
		return false;
	}
	if ( d1 > 0.0f && d2 > 0.0f ) {
		return false;
	}
	if ( d1 < 0.0f && d2 < 0.0f ) {
		return false;
	}

	fraction = ( d1 / ( d1 - d2 ) );

	return ( fraction >= 0.0f && fraction <= 1.0f );
}

inline bool	Plane::RayIntersection( const Vector3 & start, const Vector3 & dir, float & scale ) const
{
	float d1, d2;

	d1 = Normal() * start + d;
	d2 = Normal() * dir;

	if ( d2 == 0.0f ) {
		return false;
	}
	scale = -( d1 / d2 );
	return true;
}

inline bool	Plane::PlaneIntersection( const Plane & plane, Vector3 & start, Vector3 & dir ) const
{
	float n00, n01, n11, det, invDet, f0, f1;

	n00 = Normal() * Normal();
	n01 = Normal() * plane.Normal();
	n11 = plane.Normal() * plane.Normal();
	det = n00 * n11 - n01 * n01;

	if ( abs( det ) < MIN_FLOAT_VALUE ) {
		return false;
	}

	invDet = 1.0f / det;
	f0 = ( n01 * plane.d - n11 * d ) * invDet;
	f1 = ( n01 * d - n00 * plane.d ) * invDet;

	dir = Normal().Cross( plane.Normal() );
	start = Normal() * f0 + plane.Normal() * f1;
	return true;
}

inline Plane	Plane::Tranlation( const Vector3 & vec ) const
{
	return Plane(a, b, c, d - Normal() * vec );
}

inline void Plane::TranlationSelf( const Vector3 & vec )
{
	d -= ( Normal() * vec );
}


#endif