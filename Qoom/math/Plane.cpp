#include "Plane.h"
#include "Sphere.h"


float Plane::DistanceFromSphere( const Sphere & sphere ) const
{
	float distance = Normal() * sphere.GetCenter() + d;

	if ( distance > sphere.GetRadius() ) {
		return distance - sphere.GetRadius();
	}
	if ( distance < -sphere.GetRadius() ) {
		return distance + sphere.GetRadius();
	} else {
		return 0.0f;
	}
}
