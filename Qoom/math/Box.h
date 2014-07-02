#ifndef __BOX_H__
#define __BOX_H__

#include "..\sys\sys_main_header.h"
#include "Vector.h"
#include "Sphere.h"
#include "Plane.h"

class Box
{
public:
	Box( void );
	Box( const Vector3 & mins, const Vector3 & maxs );

	~Box( void ) {}

	Box & operator+=( const Vector3 & right );
	Box & operator-=( const Vector3 & right );
	Box & operator*=( float s );
	Box & operator/=( float s );

	Box operator=( const Box & box );

	bool operator!=( const Box & box ) const;
	bool operator==( const Box & box ) const;

	friend Box operator+( const Box & left, const Vector3 & right );
	friend Box operator-( const Box & left, const Vector3 & right );
	friend Box operator*( const Box & left, float right );
	friend Box operator/( const Box & left, float right );

	void SetMin( const Vector3 & mins );
	void SetMax( const Vector3 & mins );

	Vector3 GetMin( void ) const;
	Vector3 GetMax( void ) const;

	//bool IntersectionSphere( const Sphere & sphere ) const { return false; }
	float DistaceToPlane( const Plane & plane ) const;
	float DistanceToBox( const Box & box ) const;
	Vector3 GetCenter( void ) const;

private:
	Vector3 mins, maxs;
};

inline Box::Box( void )
{
	mins.Zero();
	maxs.Zero();
}

inline Box::Box( const Vector3 & _mins, const Vector3 & _maxs )
{
	mins = _mins;
	maxs = _maxs;
}


inline Box & Box::operator += ( const Vector3 & right )
{
	mins += right;
	maxs += right;
	return *this;
}

inline Box & Box::operator -= ( const Vector3 & right )
{
	mins -= right;
	maxs -= right;
	return *this;
}

inline Box & Box::operator *= ( float s )
{
	mins *= s;
	maxs *= s;
	return *this;
}

inline Box & Box::operator /= ( float s )
{
	mins /= s;
	maxs /= s;
	return *this;
}

inline Box  operator + ( const Box & left, const Vector3 & right )
{
	return Box( left.mins + right, left.maxs + right );
}

inline Box  operator - ( const Box & left, const Vector3 & right )
{
	return Box( left.mins - right, left.maxs - right );
}

inline Box  operator * ( const Box & left, float right )
{
	return Box( left.mins * right, left.maxs * right );
}

inline Box  operator / ( const Box & left, float right )
{
	return Box( left.mins / right, left.maxs / right );
}

inline Box Box::operator=( const Box & right )
{
	return Box( right.mins, right.maxs );
}

inline bool Box::operator==( const Box & right ) const
{
	return ( mins == right.mins && maxs == right.maxs );
}

inline bool Box::operator!=( const Box & right ) const
{
	return ( mins != right.mins || maxs != right.maxs );
}


inline void Box::SetMin( const Vector3 & min )
{
	mins = min;
}

inline void Box::SetMax( const Vector3 & max )
{
	maxs = max;
}

inline Vector3 Box::GetMin( void ) const
{
	return mins;
}

inline Vector3 Box::GetMax( void ) const
{
	return maxs;
}

inline Vector3 Box::GetCenter( void ) const
{
	return ( ( mins + maxs ) / 2 );
}

inline float Box::DistaceToPlane( const Plane & plane ) const
{
	float r1 = plane.DistanceFromPoint( mins );
	float r2 = plane.DistanceFromPoint( maxs );
	float r = min( r1, r2 );

	return ( r1 <= ( mins - maxs ).Length() && r2 <= ( mins - maxs ).Length() ) ? 0.0f : r;
}

inline float Box::DistanceToBox( const Box & box ) const
{
	float r0 = ( box.maxs - box.mins ).Length();
	float r1 = ( box.maxs - mins ).Length();
	float r2 = ( box.maxs - maxs ).Length();
	float r3 = ( box.mins - mins ).Length();
	float r4 = ( box.mins - maxs ).Length();
	if ( ( r1 < r0 && r2 < r0 ) || ( r3 < r0 ) && ( r4 < r0 ) ) return 0.0f; 
	else return min( min( r1, r2 ), min( r3, r4 ) );
}


#endif