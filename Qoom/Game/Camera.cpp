#include "Camera.h"
#include "..\math\Quat.h"


extern Render render;

#define CURSOR_W	36
#define CURSOR_H	36

Camera::Camera( void )
{
	Clear();
}

void Camera::Clear( void )
{

	ZeroMemory( &view, sizeof( view ) );
	view._11 = view._22 = view._33 = view._44 = 1.0f;

	this->width = 0;
	this->height = 0;


	up.Zero();
	forward.Zero();
	right.Zero();


}

Camera::~Camera( void )
{
}

int Camera::Init( void )
{
	Vector3 r;
	eye.Zero();
	this->width = render.GetScreenWidth();
	this->height = render.GetScreenHeight();
	
	forward.x = 0.0f; forward.y = 0.0f; forward.z = 1.0f;
	up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;
	right.x = 1.0f; right.y = 0.0f; right.z = 0.0f;
	
	yaw = 0.0f;
	minYaw = -D3DX_PI / 5;
	maxYaw = +D3DX_PI / 5;

	lookAt = forward + eye;

	InitInterface();

	return 0;
}

void Camera::MoveX( float dx, float dy, float dz )
{
	Vector3 v;

	v = right;
	v.y = 0.0f;
	v.x *= dx;
	v.y *= dy;
	v.z *= dz;

	eye += v;

	lookAt = eye + forward;
	
}

void Camera::MoveY( float dx, float dy, float dz )
{
	Vector3 v;

	v = up;
	v.y = 0.0f;
	v.x *= dx;
	v.y *= dy;
	v.z *= dz;

	eye += v;

	lookAt = eye + forward;
	
}

void Camera::MoveZ( float dx, float dy, float dz )
{
	Vector3 v;

	v = forward;
	v.y = 0.0f;
	v.x *= dx;
	v.y *= dy;
	v.z *= dz;

	eye += v;

	lookAt = eye + forward;
	
}

void Camera::Move( float dx, float dy, float dz )
{
	Vector3 v;

	v = forward;
	v.y = 0.0f;
	v *= dz;
	eye += v;

	v = right;
	v.y = 0.0f;
	v *= dx;
	eye += v;

	v = up;
	v.x = 0.0f;
	v.z = 0.0f;
	v *= dy;
	eye += v;

	lookAt = eye + forward;
}

//static void matmulvect( Vector3 & vec, D3DXMATRIX & mat )
//{
//	vec.x = mat._11 * vec.x + mat._12 * vec.y + mat._13 * vec.z;
//	vec.y = mat._21 * vec.x + mat._22 * vec.y + mat._23 * vec.z;
//	vec.z = mat._31 * vec.x + mat._32 * vec.y + mat._33 * vec.z;
//}

//void static Vec3MulMat4( Vector3 & vec, const D3DXMATRIX & mat )
//{
//	D3DXVECTOR4 v4;
//	D3DXMATRIX tmp;
//	D3DXMatrixTranspose( &tmp, &mat );
//	v4.x = vec.x; v4.y = vec.y; v4.z = vec.z; v4.w = 1.0f;
//	D3DXVec4Transform( &v4, &v4, &tmp );
//
//	vec.x = v4.x / v4.w;
//	vec.y = v4.y / v4.w;
//	vec.z = v4.z / v4.w;
//}

void Camera::Rotate( float pitch, float _yaw, float roll )
{

	if ( ( this->yaw + _yaw ) > maxYaw ) {
		_yaw = maxYaw - this->yaw;
		this->yaw = maxYaw;
	} else if ( ( this->yaw + _yaw ) < minYaw ){
		_yaw = minYaw - this->yaw;
		this->yaw = minYaw;
	} else {
		this->yaw += _yaw;
	}


	Vector3 s_f = forward;
	Vector3 s_u = up;
	Vector3 s_r = right;

	Quat quat;

	quat.x = 0.0f; quat.y = s_u.y; quat.z = 0.0f; quat.w = pitch;
	quat.Normalize();
	forward = quat * forward;
	up = quat * up;
	right = quat * right;

	quat.x = s_r.x; quat.y = 0.0f; quat.z = s_r.z; quat.w = _yaw;
	quat.Normalize();
	forward = quat * forward;
	up = quat * up;
	right = quat * right;

	quat.x = s_f.x; quat.y = 0.0f; quat.z = s_f.z; quat.w = roll;
	quat.Normalize();
	forward = quat * forward;
	up = quat * up;
	right = quat * right;

	forward.Normalize();
	up.Normalize();
	right.Normalize();

	lookAt = eye + forward;

}

void Camera::GetCameraMatrix( D3DXMATRIX & mat ) const
{
	D3DXMatrixLookAtLH( &mat, (D3DXVECTOR3*)&eye, (D3DXVECTOR3*)&lookAt, (D3DXVECTOR3*)&up );
}


void Camera::InitInterface( void )
{
	surface_t cursor;
	WORD indexes[ 6 ] = { 0, 1, 2, 3, 0, 2 };
	vertex_t vertexes[ 4 ];

	vertexes[ 0 ].pos.x = (float)( -CURSOR_W / 2 );
	vertexes[ 0 ].pos.y = (float)( -CURSOR_H / 2 );
	vertexes[ 0 ].pos.z = forward.z;//render.GetZNear();
	vertexes[ 0 ].tex.x = 0.0f; vertexes[ 0 ].tex.y = 0.0f;
	vertexes[ 0 ].n.x = vertexes[ 0 ].n.y = 0.0f; vertexes[ 0 ].n.z = -1.0f;
	vertexes[ 0 ].b.x = vertexes[ 0 ].b.z = 0.0f; vertexes[ 0 ].b.y = 1.0f;
	vertexes[ 0 ].tan.z = vertexes[ 0 ].tan.y = 0.0f; vertexes[ 0 ].tan.x = 1.0f;

	vertexes[ 1 ].pos.x = (float)( - CURSOR_W / 2 );
	vertexes[ 1 ].pos.y = (float)( CURSOR_H / 2 );
	vertexes[ 1 ].pos.z = forward.z;//render.GetZNear();
	vertexes[ 1 ].tex.x = 0.0f; vertexes[ 1 ].tex.y = 1.0f;
	vertexes[ 1 ].n.x = vertexes[ 1 ].n.y = 0.0f; vertexes[ 1 ].n.z = -1.0f;
	vertexes[ 1 ].b.x = vertexes[ 1 ].b.z = 0.0f; vertexes[ 1 ].b.y = 1.0f;
	vertexes[ 1 ].tan.z = vertexes[ 1 ].tan.y = 0.0f; vertexes[ 1 ].tan.x = 1.0f;

	vertexes[ 2 ].pos.x = (float)( CURSOR_W / 2 );
	vertexes[ 2 ].pos.y = (float)( CURSOR_H / 2 );
	vertexes[ 2 ].pos.z = forward.z;//render.GetZNear();
	vertexes[ 2 ].tex.x = 1.0f; vertexes[ 2 ].tex.y = 1.0f;
	vertexes[ 2 ].n.x = vertexes[ 2 ].n.y = 0.0f; vertexes[ 2 ].n.z = -1.0f;
	vertexes[ 2 ].b.x = vertexes[ 2 ].b.z = 0.0f; vertexes[ 2 ].b.y = 1.0f;
	vertexes[ 2 ].tan.z = vertexes[ 2 ].tan.y = 0.0f; vertexes[ 2 ].tan.x = 1.0f;

	vertexes[ 3 ].pos.x = (float)( CURSOR_W / 2 );
	vertexes[ 3 ].pos.y = (float)( -CURSOR_H / 2 );
	vertexes[ 3 ].pos.z = forward.z;//render.GetZNear();
	vertexes[ 3 ].tex.x = 1.0f; vertexes[ 3 ].tex.y = 0.0f;
	vertexes[ 3 ].n.x = vertexes[ 3 ].n.y = 0.0f; vertexes[ 3 ].n.z = -1.0f;
	vertexes[ 3 ].b.x = vertexes[ 3 ].b.z = 0.0f; vertexes[ 3 ].b.y = 1.0f;
	vertexes[ 3 ].tan.z = vertexes[ 3 ].tan.y = 0.0f; vertexes[ 3 ].tan.x = 1.0f;


	strcpy( cursor.texfilename, "textures\\3_cursor2.tga" );

	cursor.numOfindexes = 6; cursor.numOftextures = 1; cursor.numOfTriangles = 2; cursor.numOfvertexes = 4;

	cursor.indexes = indexes;
	cursor.vertexes = vertexes;

	cursor.PShaderName = cursor.VShaderName = NULL;

	cursor_index = render.RegisterSurface( cursor );

}

void Camera::DrawInterface( void )
{
	render.Set2D();
	D3DXMATRIX mat;
	ZeroMemory( &mat, sizeof( mat ) );
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	render.DrawIndexSurface( cursor_index, mat );
}