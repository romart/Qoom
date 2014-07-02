#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "..\sys\sys_main_header.h"
#include "..\render\r_common.h"
#include "..\math\Angle.h"


class Camera
{
public:
	Camera( void );
	~Camera( void );

	void Clear( void );

	int Init( void );



	void SetWidth( int w ) { width = w; }
	void SetHeight( int h ) { height = h; }

	void SetEye( const Vector3 & _eye ) { this->eye = _eye; lookAt = _eye + forward; }

	void SetUp( const Vector3 & up ) { this->up = up; }
	void SetRight( const Vector3 & right ) { this->right = right; }
	void SetForward( const Vector3 & _forward ) { this->forward = _forward; lookAt = eye + _forward; }

	const Vector3 & GetEye( void ) const { return eye; }
	const Vector3 & GetForward( void ) const { return forward; }
	const Vector3 & GetRight( void ) const { return right; }
	const Vector3 & GetUp( void ) const { return up; }

	inline float GetYaw( void ) const { return yaw; }

	void MoveX( float dx, float dy, float dz );	//	bad code
	void MoveY( float dx, float dy, float dz );	//	bad code
	void MoveZ( float dx, float dy, float dz );	//	bad code

	void Move( float dx, float dy, float dz );

	void Rotate( float pitch, float yaw, float roll );

	void GetCameraMatrix( D3DXMATRIX & mat ) const;


	void DrawInterface( void );
	
	
private:
	void InitInterface( void );

	D3DXMATRIX	view;

	int width;
	int height;


	Vector3	eye;
	Vector3 lookAt;

	Vector3 up;
	Vector3 right;
	Vector3 forward;
	
	float yaw;
	
	float minYaw;
	float maxYaw;

	int	  cursor_index;

};

#endif