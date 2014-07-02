#include "Angle.h"


void Angle::ToVectors( Vector3 & forward, Vector3 & up, Vector3 & right ) const
{
	float sr, sp, sy, cr, cp, cy;
	
	sy = sin( yaw ); cy = cos( yaw );
	sp = sin( pitch ); cp = cos( pitch );
	sr = sin( roll ); cr = cos( roll );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
	
	up.x = cr * sp * cy + -sr * -sy;
	up.y = cr * sp * sy + -sr * cy;
	up.z = cr * cp;

	right.x = -sr * sp * cy + cr * sy;
	right.y = -sr * sp * sy + -cr * cy;
	right.z = -sr * cp;
}