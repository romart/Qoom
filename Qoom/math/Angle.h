#ifndef __ANGLE_H__
#define __ANGLE_H__

#include "vector.h"
#define EPSILON	( 0.00001f )
#define EPSILON_ANGLE	( 0.01f )
class Angle
{
public:
	float			pitch;	//around x
	float			yaw;	//around y
	float			roll;	//around z
	

	Angle( void );
	Angle( float pitch, float yaw, float roll );
	~Angle( void );

	void Clear( void );
	void Set( float pitch, float yaw, float roll );

	Angle & operator +=( const Angle & right );
	Angle & operator -=( const Angle & right );

	Angle & operator *=( float r );
	Angle & operator /=( float r );

	//Angle & operator +=( const Angle & right );

	Angle	operator -() const;
	Angle &	operator =( const Angle & right );

	bool	operator ==( const Angle & right ) const;
	bool	operator !=( const Angle & right ) const;

	void ToVectors( Vector3 & forward, Vector3 & up,  Vector3 & right ) const;

};

inline Angle::Angle( void )
{
	Clear();
}

inline Angle::Angle( float _pitch, float _yaw, float _roll )
{
	Set( _pitch, _yaw, _roll );
}

inline Angle::~Angle( void )
{

}

inline void Angle::Set( float pitch, float yaw, float roll )
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

inline void Angle::Clear( void )
{
	pitch = yaw = roll = 0.0f;
}

inline Angle & Angle::operator +=( const Angle & right )
{
	pitch	+= right.pitch;
	yaw		+= right.yaw;
	roll	+= right.roll;
	return *this;
}

inline Angle & Angle::operator -=( const Angle & right )
{
	pitch	-= right.pitch;
	yaw		-= right.yaw;
	roll	-= right.roll;
	return *this;
}

inline Angle & Angle::operator *=( float right )
{
	pitch	*= right;
	yaw		*= right;
	roll	*= right;
	return *this;
}

inline Angle & Angle::operator /=( float right )
{
	pitch	/= right;
	yaw		/= right;
	roll	/= right;
	return *this;
}

inline Angle Angle::operator -( ) const
{
	return Angle( -pitch, -yaw, -roll );
}

inline Angle & Angle::operator =( const Angle & right )
{
	
	pitch = right.pitch;
	yaw = right.yaw;
	roll = right.roll;
	return *this;
}

inline bool Angle::operator ==(  const Angle & right ) const
{
	return ( pitch == right.pitch && yaw == right.yaw && roll == right.roll );
}

inline bool Angle::operator !=(  const Angle & right ) const
{
	return ( pitch != right.pitch || yaw != right.yaw || roll != right.roll );
}


#endif