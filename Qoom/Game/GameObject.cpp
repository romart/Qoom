#include "GameObject.h"
#include "Player.h"
#include "..\render\r_file.h"
#include "..\render\r_common.h"

#include "..\framework\log.h"

extern Render render;
extern Logger logger;

GameObject::GameObject( void )
{
	w = h = d = 0.0f;
	cm_active = false;
	r_active = false;
	index = -1;
	scale = 1.0f;
	this->pShader = this->vShader = NULL;
}

void GameObject::LoadFromFile( const char * _fileName )
{
	Model model;
	model.LoadModel( _fileName );
	model.SetVertexShader( vShader );
	model.SetPixelShader( pShader );
//	model.Parce();
	this->fileName = _fileName;
	CreateFromModel( model );
	return ;

}

int GameObject::CreateFromModel( Model & model )
{
	const md3Frame_t * frames = model.GetFramePtr();
	int numFrames = model.GetNumOfFrames();
	if ( numFrames < 1 ) {
		logger.Warning( "GameObject::CreateFromModel: incorect object from %s", fileName );
	} else {
		cm_active = true;
	}

	char o_name[ 16 ];
	memcpy( o_name, frames->name, sizeof( o_name ) );

	w = fabs( frames->bounds[ 0 ].x - frames->bounds[ 1 ].x );
	h = fabs( frames->bounds[ 0 ].y - frames->bounds[ 1 ].y );
	d = fabs( frames->bounds[ 0 ].z - frames->bounds[ 1 ].z );

	//pos = frames->localOrigin;
	box.SetMin( frames->bounds[ 0 ] );
	box.SetMax( frames->bounds[ 1 ] );
	sphere.SetRadius( frames->radius / 1.5f );


	model.Parce();

	index = render.RegisterModel( model );
	
	if ( index > 0 ) {
		r_active = true;
		logger.Info( "GameObject::CreateFromModel: register game object %s", o_name );
	} else {
		logger.Error( "GameObject::CreateFromModel: cannot load object from file", fileName );
	}
	return 0;
}

void GameObject::SetPos( const Vector3 & vec )
{
	box -= pos;
	sphere -= pos;
	pos = vec;
	box += pos;
	sphere += pos;
}

void GameObject::SetScale( float _scale )
{
	float iscale = 1 / scale;
	w *= iscale;
	h *= iscale;
	d *= iscale;

	sphere *= iscale;
	box *= iscale;

	scale = _scale;
	
	w *= scale;
	h *= scale;
	d *= scale;

	box *= scale;
	sphere *= scale;
}

void GameObject::DrawObject( const D3DXMATRIX & mat, const D3DXMATRIX & view )
{
	D3DXMATRIX tmp, world;
	world = mat;
	D3DXMatrixScaling( &tmp, scale, scale, scale );
	world *= tmp;
	D3DXMatrixTranslation( &tmp, pos.x, pos.y, pos.z );
	world *= tmp;
	vec4_t v1[ 6 ];
	Vector3 f = player->GetCamera().GetForward();
	v1[ 0 ][ 0 ] = f.x; v1[ 0 ][ 1 ] = f.y; v1[ 0 ][ 2 ] = f.z; v1[ 0 ][ 3 ] = 1.0f;
	v1[ 1 ][ 0 ] = 275.0f; v1[ 1 ][ 1 ] = 45.0f; v1[ 1 ][ 2 ] = 110.0f; v1[ 1 ][ 3 ] = 0.0f;
	memcpy( &v1[2][0], &mat, sizeof( mat ) );
	render.SetVertexConstArray( v1, 6 );
	world *= view;
	render.DrawIndexModel( index, world );
	render.SetVertexConstArray();
}

void GameObject::CreateCMObject( const Vector3 & _pos, float r )
{
	r_active = false;
	cm_active = true;
	sphere.SetCenter( _pos );
	sphere.SetRadius( r );
}