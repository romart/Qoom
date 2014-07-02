#include "Sphere.h"
#include "Plane.h"

float Sphere::DistanseToPlane( const Plane & plane ) const
{
	float distance = plane.Normal() * center + plane.Dist();

	if ( distance > r ) {
		return distance - r;
	}
	if ( distance < -r ) {
		return distance + r;
	} else {
		return 0.0f;
	}
}

float Sphere::DistanceToSphere( const Sphere & sphere ) const
{
	float dist = ( center - sphere.center ).Length();
	float rr   = r + sphere.r;

	return rr < dist ? dist - rr : 0.0f;

}