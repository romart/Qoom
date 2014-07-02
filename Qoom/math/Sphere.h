#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "Vector.h"

class Plane;

class Sphere
{
public:

	Sphere( void );
	Sphere( const Vector3 & center, float r = 0.0f );
	~Sphere( void ) {}

	void Clear( void );
	void SetCenter( const Vector3 & center );
	void SetRadius( float r );

	Sphere & operator +=( const Vector3 & vec ) { center += vec; return *this; }
	Sphere & operator -=( const Vector3 & vec )	{ center -= vec; return *this; }
	Sphere & operator *=( float scale ) { r *= scale; return *this; }
	Sphere & operator /=( float scale ) { r = scale > MIN_FLOAT_VALUE ? r / scale : 0.0f; return *this; }

	Sphere operator =( const Sphere & sphere ) { return Sphere( sphere.center, sphere.r ); }

	bool operator ==( const Sphere & sphere ) const { return ( center == sphere.center && r == sphere.r ); }
	bool operator !=( const Sphere & sphere ) const { return ( center != sphere.center || r != sphere.r ); }


	friend Sphere operator+ ( const Sphere & sphere, const Vector3 & vec );
	friend Sphere operator- ( const Sphere & sphere, const Vector3 & vec );

	Vector3 GetCenter( void ) const { return center; }
	float GetRadius( void ) const { return r; }

	float DistanseToPlane( const Plane & plane ) const;
	float DistanceToSphere( const Sphere & sphere ) const;

private:
	Vector3 center;
	float r;
};


inline Sphere::Sphere( void )
{
	Clear();
}

inline Sphere::Sphere( const Vector3 & _center, float _r )
{
	SetCenter( _center );
	SetRadius( _r );
}

inline void Sphere::Clear( void )
{
	center.Zero();
	r = 0.0f;
}

inline void Sphere::SetCenter( const Vector3 & _center )
{
	center = _center;
}


inline void Sphere::SetRadius( float _r )
{
	r = _r;
}

inline Sphere operator+( const Sphere & sphere, const Vector3 & vec )
{
	return Sphere( sphere.center + vec, sphere.r );
}

inline Sphere operator-( const Sphere & sphere, const Vector3 & vec )
{
	return Sphere( sphere.center - vec, sphere.r );
}


#endif