#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "math_header.h"

class Vector2
{
public:
	float x, y;

	Vector2( void ) { x = y = 0.0f; };
	Vector2( float x, float y );
	~Vector2( void ) {}

	Vector2 & operator += ( const Vector2 & right );
	Vector2 & operator -= ( const Vector2 & right );
	Vector2 & operator *= ( const float right );
	Vector2 & operator /= ( const float right );
	
	friend Vector2 operator+ ( const Vector2 & left, const Vector2 & right );
	friend Vector2 operator- ( const Vector2 & left, const Vector2 & right );
	friend float operator* ( const Vector2 & left, const Vector2 & right );
	friend Vector2 operator* ( const Vector2 & left, const float right);
	friend Vector2 operator/ ( const Vector2 & left, const float right);

	bool operator==( const Vector2 & right ) const;
	bool operator!=( const Vector2 & right ) const;

	Vector2			operator-() const;
	Vector2 &		operator=( const Vector2 & right );	

	void Zero( void );
	void Normalize( void );
	float Length( void ) const;

};

inline void Vector2::Zero( void )
{
	x = y = 0.0f;
}

inline Vector2::Vector2( float x, float y )
{
	this->x = x;
	this->y = y;
}

inline Vector2 & Vector2::operator +=( const Vector2 & right )
{
	this->x += right.x;
	this->y += right.y;
	return *this;
}

inline Vector2 & Vector2::operator -=( const Vector2 & right )
{
	this->x -= right.x;
	this->y -= right.y;
	return *this;
}

inline Vector2 & Vector2::operator *=( const float right)
{
	this->x *= right;
	this->y *= right;
	return *this;
}

inline Vector2 & Vector2::operator /=( const float right)
{
	if ( right > MIN_FLOAT_VALUE ) {
		this->x /= right;
		this->y /= right;
	} else {
		x = y = 0.0f;
	}
	return *this;
}

inline Vector2 operator+ ( const Vector2 & left, const Vector2 & right )
{
	Vector2 ret;
	ret.x = left.x + right.x;
	ret.y = left.y + right.y;
	return ret;
}

inline Vector2 operator- ( const Vector2 & left, const Vector2 & right )
{
	Vector2 ret;
	ret.x = left.x - right.x;
	ret.y = left.y - right.y;
	return ret;
}

inline float operator* ( const Vector2 & left, const Vector2 & right )
{
	return ( left.x * right.x + left.y * right.y );
}

inline Vector2 operator* ( const Vector2 & left, const float right )
{
	Vector2 ret;
	ret.x = left.x * right;
	ret.y = left.y * right;
	return ret;
}

inline Vector2 operator/ ( const Vector2 & left, const float right )
{
	Vector2 ret;
	if ( right > MIN_FLOAT_VALUE ) {
		ret.x = left.x / right;
		ret.y = left.y / right;
	}
	return ret;
}

inline bool Vector2::operator==( const Vector2 & right ) const
{
	return ( x == right.x && y == right.y );
}

inline bool Vector2::operator!=( const Vector2 & right ) const
{
	return ( x != right.x || y != right.y );
}

inline Vector2 & Vector2::operator=( const Vector2 & right )
{
	x = right.x;
	y = right.y;
	return *this;
}

inline Vector2 Vector2::operator-() const 
{
	return Vector2( -x, -y );
}

inline float Vector2::Length( void ) const
{
	return sqrt( ( *this ) * ( *this ) );
}

inline void Vector2::Normalize( void )
{
	*this /= Length();
}

class Vector3
{
public:
	Vector3( void ) { x = 0.0f; y = 0.0f; z = 0.0f; }
	Vector3( float x, float y, float z ) { this->x = x; this->y = y; this->z = z; }
	~Vector3( void ) {}

	Vector3 & operator += ( const Vector3 & right );
	Vector3 & operator -= ( const Vector3 & right );
	Vector3 & operator *= ( const float right );
	Vector3 & operator /= ( const float right );
	
	friend Vector3 operator+ ( const Vector3 & left, const Vector3 & right );
	friend Vector3 operator- ( const Vector3 & left, const Vector3 & right );
	friend float operator* ( const Vector3 & left, const Vector3 & right );
	friend Vector3 operator* ( const Vector3 & left, const float right);
	friend Vector3 operator/ ( const Vector3 & left, const float right);

	bool operator==( const Vector3 & right ) const;
	bool operator!=( const Vector3 & right ) const;

	Vector3			operator-() const;
	Vector3 &		operator=( const Vector3 & right );	

	void	Normalize( void );
	void	Zero( void );
	float	Length( void ) const;
	Vector3 Cross( const Vector3 &vec );

	float x, y, z;
};



inline Vector3 & Vector3::operator +=( const Vector3 & right )
{
	this->x += right.x;
	this->y += right.y;
	this->z += right.z;
	return *this;
}

inline Vector3 & Vector3::operator -=( const Vector3 & right )
{
	this->x -= right.x;
	this->y -= right.y;
	this->z -= right.z;
	return *this;
}

inline Vector3 & Vector3::operator *=( const float right)
{
	this->x *= right;
	this->y *= right;
	this->z *= right;
	return *this;
}

inline Vector3 & Vector3::operator /=( const float right)
{
	if ( right > MIN_FLOAT_VALUE ) {
		this->x /= right;
		this->y /= right;
		this->z /= right;
	} else {
		x = y = z = 0.0f;
	}
	return *this;
}

inline Vector3 operator+ ( const Vector3 & left, const Vector3 & right )
{
	Vector3 ret;
	ret.x = left.x + right.x;
	ret.y = left.y + right.y;
	ret.z = left.z + right.z;
	return ret;
}

inline Vector3 operator- ( const Vector3 & left, const Vector3 & right )
{
	Vector3 ret;
	ret.x = left.x - right.x;
	ret.y = left.y - right.y;
	ret.z = left.z - right.z;
	return ret;
}

inline float operator* ( const Vector3 & left, const Vector3 & right )
{
	return ( left.x * right.x + left.y * right.y + left.z * right.z );
}

inline Vector3 operator* ( const Vector3 & left, const float right )
{
	Vector3 ret;
	ret.x = left.x * right;
	ret.y = left.y * right;
	ret.z = left.z * right;
	return ret;
}

inline Vector3 operator/ ( const Vector3 & left, const float right )
{
	Vector3 ret;
	if ( right > MIN_FLOAT_VALUE ) {
		ret.x = left.x / right;
		ret.y = left.y / right;
		ret.z = left.z / right;
	}
	return ret;
}

inline bool Vector3::operator==( const Vector3 & right ) const
{
	return ( x == right.x && y == right.y && z == right.z );
}

inline bool Vector3::operator!=( const Vector3 & right ) const
{
	return ( x != right.x || y != right.y || z != right.z );
}

inline Vector3 & Vector3::operator=( const Vector3 & right )
{
	x = right.x;
	y = right.y;
	z = right.z;
	return *this;
}

inline Vector3 Vector3::operator-() const 
{
	return Vector3( -x, -y, -z );
}

inline float Vector3::Length( void ) const
{
	return sqrt( ( *this ) * ( *this ) );
}

inline void Vector3::Normalize( void )
{
	*this /= Length();
}

inline void Vector3::Zero( void )
{
	x = y = z = 0.0f;
}

inline Vector3 Vector3::Cross( const Vector3 &vec )
{
	return Vector3( y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x );
}

#endif